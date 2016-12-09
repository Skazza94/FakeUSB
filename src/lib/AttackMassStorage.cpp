/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for mass storages and handles the bulk transfer.
 *
 * Author: Skazza
 */

#include "AttackMassStorage.h"

AttackMassStorage::AttackMassStorage() : Attack(4) {
	this->setupType2Callback.insert(
		std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			MS_REQ_GetMaxLUN, std::bind(&AttackMassStorage::getMaxLUN, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_TEST_UNIT_READY, std::bind(&AttackMassStorage::scsiDefaultGoodResponse, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_START_STOP_UNIT, std::bind(&AttackMassStorage::scsiDefaultGoodResponse, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_INQUIRY, std::bind(&AttackMassStorage::scsiInquiry, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_REQUEST_SENSE, std::bind(&AttackMassStorage::scsiRequestSense, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_READ_CAPACITY_10, std::bind(&AttackMassStorage::scsiReadCapacity, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_MODE_SENSE_6, std::bind(&AttackMassStorage::scsiModeSense, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL, std::bind(&AttackMassStorage::scsiPAMediumRemoval, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_READ_10, std::bind(&AttackMassStorage::scsiRead10, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->deviceRequest2Callback.insert(
		std::pair<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>(
			SCSI_CMD_WRITE_10, std::bind(&AttackMassStorage::scsiWrite10, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
}

AttackMassStorage::~AttackMassStorage() {
	if(this->inquiryData) {
		delete(this->inquiryData);
		this->inquiryData = NULL;
	}

	if(this->senseData) {
		delete(this->senseData);
		this->senseData = NULL;
	}

	delete(this->virtualDrive);
}

void AttackMassStorage::loadAttack() {
	/* Load emulated device SCSI information from a file as an inquiry response */
	std::string deviceFolder = "/home/debian/AntiUSBProxy/config/" + this->cfg->get("Device") + "/";
	std::string scsiDeviceInfo = deviceFolder + "scsiDeviceInfo";
	FILE * scsiDeviceInfoFileHandler = fopen(scsiDeviceInfo.c_str(), "rb");

	if(scsiDeviceInfoFileHandler) {
		this->inquiryData = new SCSI_Inquiry_Response_t;
		fread(this->inquiryData, sizeof(SCSI_Inquiry_Response_t), 1, scsiDeviceInfoFileHandler);

		fclose(scsiDeviceInfoFileHandler);
	}

	/* Load emulated device SCSI sense data from a file as a request sense response */
	std::string scsiSenseData = deviceFolder + "scsiSenseData";
	FILE * scsiSenseDataFileHandler = fopen(scsiSenseData.c_str(), "rb");

	if(scsiSenseDataFileHandler) {
		this->senseData = new SCSI_Request_Sense_Response_t;
		fread(this->senseData, sizeof(SCSI_Request_Sense_Response_t), 1, scsiSenseDataFileHandler);

		fclose(scsiSenseDataFileHandler);
	}

	/* Open the virtual drive file for read/write */
	std::string virtualDriveFile = deviceFolder + "virtualDrive";
	this->virtualDrive = new VirtualDrive(virtualDriveFile);
}

/* Builds a CSW packet, with signature, CBW tag and the desired status */
__u8 * AttackMassStorage::buildCSWPacket(__u32 tag, __u8 status) {
	MS_CommandStatusWrapper_t * csw = new MS_CommandStatusWrapper_t;
	csw->Signature = MS_CSW_SIGNATURE;
	csw->Tag = tag;
	csw->DataTransferResidue = 0x00;
	csw->Status = status;

	return (__u8 *) csw;
}

void AttackMassStorage::parseDeviceRequest(__u16 maxPacketSize, __u8 * dataPtr, __u64 length, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	/* Process the CBW packets to handle commands */
    if ((length == 31) &&
    	(dataPtr[0x00] == 0x55) &&
    	(dataPtr[0x01] == 0x53) &&
    	(dataPtr[0x02] == 0x42) &&
    	(dataPtr[0x03] == 0x43)) {
    	__u16 opCode = dataPtr[0x0f]; /* Get the command OP code */

    	/* Find the callback for this OP code */
    	std::map<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>>::iterator it = this->deviceRequest2Callback.find(opCode);

    	/* If it's found, execute it! */
		if(it != this->deviceRequest2Callback.end())
			(*it).second(dataPtr, packetBuffer);

		return;
    }

    /* If it's not a CBW packet, a previous WRITE command has been submitted...
     * If we set the startingWriteLBA into the scsiWrite10 method, it will be surely different from 0xffffffffffffffff
     * (because we have a drive size < than 0xffffffffffffffff), so we write data in the virtual drive file.
     * Since we receive 512 byte blocks, we "iterate" in the writing operation, incrementing the written blocks (used as offset). */
    if(this->startingWriteLBA != 0xffffffffffffffff) {
    	this->virtualDrive->writeBlock(dataPtr, this->startingWriteLBA, length, this->writtenBlocks);
    	this->writtenBlocks++;

    	return;
    }
}

void AttackMassStorage::getNextPayload(std::list<std::pair<__u8 *, __u64>> ** payload, __u8 endpoint, __u16 maxPacketSize) {}

/* ~~ Setup Request Callbacks ~~ */
__u8 AttackMassStorage::getMaxLUN(const usb_ctrlrequest packet, __u8 * dataPtr) {
	__u8 * maxLUNPacket = (__u8 *) calloc(1, sizeof(__u8));
	memcpy(dataPtr, maxLUNPacket, 1);
	return 1;
}

/* ~~ SCSI Request Callbacks ~~ */
void AttackMassStorage::scsiDefaultGoodResponse(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiInquiry(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	__u8 * response = (__u8 *) malloc(sizeof(SCSI_Inquiry_Response_t));
	memcpy(response, this->inquiryData, sizeof(SCSI_Inquiry_Response_t));

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(response, sizeof(SCSI_Inquiry_Response_t)));
	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiRequestSense(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	__u8 * response = (__u8 *) malloc(sizeof(SCSI_Request_Sense_Response_t));
	memcpy(response, this->senseData, sizeof(SCSI_Request_Sense_Response_t));

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(response, sizeof(SCSI_Request_Sense_Response_t)));
	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiReadCapacity(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	/* We read the capacity of the emulated device as the file size of the virtual drive */
	if(this->virtualDrive) {
		__u32 LBA = this->virtualDrive->getLBA();
		__u32 blockSize = this->virtualDrive->getBlockSize();

		/* Little endian to big endian conversion */
		LBA = ((LBA >> 24) & 0xff) | ((LBA << 8) & 0xff0000) |
			  ((LBA >> 8) & 0xff00) | ((LBA << 24) & 0xff000000);
		blockSize = ((blockSize >> 24) & 0xff) | ((blockSize << 8) & 0xff0000) |
				   	((blockSize >> 8) & 0xff00) | ((blockSize << 24) & 0xff000000);

		__u8 * packet = (__u8 *) calloc(cbw->DataTransferLength, sizeof(__u8));
		memcpy(packet, &LBA, sizeof(__u32));
		packet += 4;
		memcpy(packet, &blockSize, sizeof(__u32));
		packet -= 4;

		(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(packet, cbw->DataTransferLength));
		(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));
	} else
		(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Fail), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiModeSense(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);
	MS_CommandStatusCodes_t status = MS_SCSI_COMMAND_Pass;

	if(cbw->SCSICommandData[0x02] == 0x3f) { /* We're getting mode pages, from a file */
		std::string scsiModePages = "/home/debian/AntiUSBProxy/config/" + this->cfg->get("Device") + "/scsiModePages";
		FILE * scsiModePagesFileHandler = fopen(scsiModePages.c_str(), "rb");

		if(scsiModePagesFileHandler) {
			__u64 responseSize = 0x24; /* Length of this is surely 36 + 1 = 0x24 */
			__u8 * response = (__u8 *) calloc(responseSize, sizeof(__u8));
			fread(response, responseSize, 1, scsiModePagesFileHandler);

			fclose(scsiModePagesFileHandler);

			(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(response, responseSize));
		} else
			status = MS_SCSI_COMMAND_Fail;
	} else /* Fail by default if we're not handling a request */
		status = MS_SCSI_COMMAND_Fail;

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, status), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiPAMediumRemoval(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiRead10(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	__u64 startingReadLBA = cbw->SCSICommandData[0x02] << 24 | cbw->SCSICommandData[0x03] << 16 | cbw->SCSICommandData[0x04] << 8 | cbw->SCSICommandData[0x05];
	__u64 blocksToRead = (cbw->SCSICommandData[0x07] << 8) | cbw->SCSICommandData[0x08];

	/* Send back the data in 512 byte packets */
	__u32 readBlocks = 0x00;
	while(readBlocks < blocksToRead) {
		__u64 blockSize = 0x00;
		__u8 * dataFromDrive = this->virtualDrive->readBlock(startingReadLBA, &blockSize, readBlocks);
		(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(dataFromDrive, blockSize));

		readBlocks++;
	}

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));

	delete(cbw);
}

void AttackMassStorage::scsiWrite10(__u8 * dataPtr, std::list<std::pair<__u8 *, __u64>> ** packetBuffer) {
	MS_CommandBlockWrapper_t * cbw = new MS_CommandBlockWrapper_t;
	memcpy(cbw, dataPtr, MS_CBW_SIZE);

	/* Set the startingWriteLBA and write length, waiting for data on this endpoint */
	this->startingWriteLBA = cbw->SCSICommandData[0x02] << 24 | cbw->SCSICommandData[0x03] << 16 | cbw->SCSICommandData[0x04] << 8 | cbw->SCSICommandData[0x05];
	this->writeLength = (cbw->SCSICommandData[0x07] << 8) | cbw->SCSICommandData[0x08];
	this->writtenBlocks = 0x00; /* Reset the written blocks */

	(*packetBuffer)->push_back(std::pair<__u8 *, __u64>(this->buildCSWPacket(cbw->Tag, MS_SCSI_COMMAND_Pass), MS_CSW_SIZE));
	delete(cbw);
}

/* Autoregisters the class into the AttackFactory */
const bool hasRegistered = AttackFactory::getInstance()->registerClass("mass-storage", &AttackMassStorage::createInstance);
