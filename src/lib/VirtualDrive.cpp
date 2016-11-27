/*
 * VirtualDrive.cpp
 *
 *  Created on: 27 nov 2016
 *      Author: skazza
 */

#include "VirtualDrive.h"

#include "HexString.h"

VirtualDrive::VirtualDrive(std::string location) {
	this->driveLocation = location;

	this->calculateSize();
}

VirtualDrive::~VirtualDrive() {}

void VirtualDrive::calculateSize() {
	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "rb");

	fseek(virtualDrive, 0, SEEK_END);
	this->LBA = (ftell(virtualDrive) / blockSize) - 1; /* The drive capacity in SCSI is calculated as file size * block size - 1 */
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 /* we subtract 1 to round the result. */

	fclose(virtualDrive);
}

__u8 * VirtualDrive::read(__u64 LBAFrom, __u64 * nBlocks) {
	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "rb");

	if(!virtualDrive) { /* If file is not opened, we return nothing */
		(*nBlocks) = 0;

		return NULL;
	}

	/* Convert starting LBA and nBlocks multipling the valid block size */
	LBAFrom *= blockSize;
	(*nBlocks) *= blockSize;

	rewind(virtualDrive); /* Move to the start, just in case */
	fseek(virtualDrive, LBAFrom, SEEK_CUR); /* Move to the desired LBA */

	/* Read data and return them */
	__u8 * dataFromDrive = (__u8 *) calloc((*nBlocks), sizeof(__u8));
	fread(dataFromDrive, sizeof(__u8), (*nBlocks), virtualDrive);

	fclose(virtualDrive);
	return dataFromDrive;
}

void VirtualDrive::write(__u8 * data, __u64 LBAFrom, __u64 length) {
	/* Add a queue with a thread here, to avoid double writes on the same block */

	FILE * virtualDrive = fopen(this->driveLocation.c_str(), "rb+");

	if(!virtualDrive) /* If file is not opened, we do nothing */
		return;

	LBAFrom *= blockSize;

	char* hex=hex_string(&LBAFrom,sizeof(__u64));
	printf("__________LBA From: %s\n", hex);
	free(hex);

	rewind(virtualDrive); /* Move to the start, just in case */
	fseek(virtualDrive, LBAFrom, SEEK_CUR); /* Move to the desired LBA */

	for(__u64 i = 0; i < length; i++)
		fwrite(&data[i], sizeof(__u8), 1, virtualDrive); /* Write data into the virtual file */

	fclose(virtualDrive);
}

__u32 VirtualDrive::getLBA() {
	return this->LBA;
}

__u32 VirtualDrive::getBlockSize() {
	return this->blockSize;
}
