/*
 * This file is not part of original USBProxy.
 * This class emulated a drive, mapping a whole file into memory, reading/writing blocks into this memory area.
 * When we finish our operations, the memory area is written back into the file.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_VIRTUALDRIVE_H
#define USBPROXY_VIRTUALDRIVE_H

#include <string>
#include <stdio.h>
#include <string.h>

#include "Criteria.h"

class VirtualDrive {
private:
	std::string driveLocation;
	const __u32 blockSize = 512;
	__u64 LBA = 0x00;
	__u64 realSize = 0x00;

	__u8 * driveContent = NULL;

	__u8 * readFile();
	void writeFile();

	void calculateSize();

public:
	VirtualDrive(std::string);
	~VirtualDrive();

	__u8 * readBlock(__u64, __u64 *, __u32);
	void writeBlock(__u8 *, __u64, __u64, __u32);
	__u32 getLBA();
	__u32 getBlockSize();
};

#endif /* USBPROXY_VIRTUALDRIVE_H */
