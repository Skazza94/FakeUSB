/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for mass storages and handles the bulk transfer.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_ATTACKMASSSTORAGE_H
#define USBPROXY_ATTACKMASSSTORAGE_H

#include <string>
#include <algorithm>

#include "Attack.h"
#include "VirtualDrive.h"
#include "MassStorageCommon.h" /* Magic header */

class AttackMassStorage : public Attack {
private:
	AttackMassStorage();

	SCSI_Inquiry_Response_t * inquiryData = NULL;
	SCSI_Request_Sense_Response_t * senseData = NULL;
	__u64 startingWriteLBA = 0xffffffffffffffff; /* Used when a WRITE CBW is submitted */
	__u64 writeLength = 0xffffffffffffffff; /* Used when a WRITE CBW is submitted */
	__u32 writtenBlocks = 0x00; /* Used when a WRITE CBW is submitted */
	VirtualDrive * virtualDrive = NULL;

	void loadAttack();
	__u8 * buildCSWPacket(__u32, __u8);

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getMaxLUN(const usb_ctrlrequest, __u8 *);
	/* ~~ SCSI Request Callbacks ~~ */
	void scsiDefaultGoodResponse(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiInquiry(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiRequestSense(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiReadCapacity(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiModeSense(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiPAMediumRemoval(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiRead10(__u8 *, std::list<std::pair<__u8 *, __u64>> **);
	void scsiWrite10(__u8 *, std::list<std::pair<__u8 *, __u64>> **);

public:
	virtual ~AttackMassStorage();

	void parseDeviceRequest(__u16, __u8 *, __u64, std::list<std::pair<__u8 *, __u64>> **);

	void getNextPayload(std::list<std::pair<__u8 *, __u64>> **, __u8, __u16);
	/* "Creator" method used by the Factory */
	static Attack * createInstance() { return new AttackMassStorage(); }
};

#endif /* USBPROXY_ATTACKMASSSTORAGE_H */
