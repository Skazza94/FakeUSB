/*
 * This file is not part of original USBProxy.
 * This class emulated a drive, mapping a whole file into memory, reading/writing blocks into this memory area.
 * When we finish our operations, the memory area is written back into the file.
 *
 * Author: Skazza
 */

#include "VirtualDrive.h"

VirtualDrive::VirtualDrive(std::string location) {
	this->driveLocation = location;
	this->calculateSize();

	this->driveContent = this->readFile();

	std::thread t = std::thread(&VirtualDrive::updateContent, this); t.detach();
}

VirtualDrive::~VirtualDrive() {
	this->stopThread = true;
	this->writeFile();

	free(this->driveContent);
}

void VirtualDrive::calculateSize() {
	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "rb");

	fseek(virtualDrive, 0, SEEK_END);
	this->realSize = ftell(virtualDrive);
	this->LBA = (this->realSize / blockSize); /* The drive capacity in SCSI is calculated as file size * block size */

	fclose(virtualDrive);
}

__u8 * VirtualDrive::readBlock(__u64 LBAFrom, __u64 * nBlocks, __u32 offset) {
	/* Convert starting LBA and nBlocks multipling the block size (and adding offset if present) */
	LBAFrom = (LBAFrom * blockSize) + (offset * blockSize);
	(*nBlocks) = blockSize;

	if(!this->driveContent) {
		(*nBlocks) = 0;

		return NULL;
	}

	__u8 * dataFromDrive = (__u8 *) calloc(blockSize, sizeof(__u8)); /* Allocate buffer */

	__u64 blocksCount = 0; __u64 LBACounter = LBAFrom;
	/* Copy desired blocks from memory */
	while(blocksCount < blockSize && LBACounter <= this->realSize) { /* Bounds checking to avoid overflows. */
																	 /* We copy if blocksCount < blockSize or if the LBA we're reading is < max LBA */
		dataFromDrive[blocksCount] = this->driveContent[LBACounter];
		blocksCount++; LBACounter++;
	}

	return dataFromDrive;
}

void VirtualDrive::writeBlock(__u8 * data, __u64 LBAFrom, __u64 length, __u32 offset) {
	while(this->writeLock); /* While we're writing the file to flush edits, we cannot write until it's finished */

	LBAFrom = (LBAFrom * blockSize) + (offset * blockSize);

	if(!this->driveContent)
		return;

	__u64 blocksCount = 0; __u64 LBACounter = LBAFrom;
	/* Copy new blocks into memory */
	while(blocksCount < length && LBACounter <= this->realSize) { /* Bounds checking to avoid overflows. */
																  /* We copy if blocksCount < length or if the LBA we're reading is < max LBA */
		this->driveContent[LBACounter] = data[blocksCount];
		blocksCount++; LBACounter++;
	}

	this->writeCount++;
}

/* Each WRITE_THRESHOLD writes we flush the updates, writing them into the drive file */
void VirtualDrive::updateContent() {
	while(!this->stopThread) {
		if(this->writeCount > WRITE_THRESHOLD) {
			this->writeLock = true;
			this->writeFile();
			this->writeLock = false;

			this->writeCount = 0;
		}
	}

	return;
}

__u32 VirtualDrive::getLBA() {
	return this->LBA;
}

__u32 VirtualDrive::getBlockSize() {
	return this->blockSize;
}

__u8 * VirtualDrive::readFile() {
	__u8 * data = (__u8 *) calloc(this->realSize, sizeof(__u8));

	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "rb");
	if(!virtualDrive) /* If file is not opened, we do nothing */
		return NULL;

	rewind(virtualDrive);
	fread(data, sizeof(__u8), this->realSize, virtualDrive); /* Read the whole file (which is the drive to emulate) into a memory area */

	fclose(virtualDrive);
	return data;
}

void VirtualDrive::writeFile() {
	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "wb");
	if(!virtualDrive) /* If file is not opened, we do nothing */
		return;

	rewind(virtualDrive);
	fwrite(this->driveContent, sizeof(__u8), this->realSize, virtualDrive); /* Write back the memory area in the drive file  */

	fclose(virtualDrive);
}
