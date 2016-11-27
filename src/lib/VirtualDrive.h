/*
 * VirtualDrive.h
 *
 *  Created on: 27 nov 2016
 *      Author: skazza
 */

#ifndef USBPROXY_VIRTUALDRIVE_H
#define USBPROXY_VIRTUALDRIVE_H

#include <string>
#include <stdio.h>

#include "Criteria.h"

class VirtualDrive {
private:
	std::string driveLocation;
	__u32 blockSize = 512;
	__u32 LBA = 0x00;

	void calculateSize();

public:
	VirtualDrive(std::string);
	~VirtualDrive();

	__u8 * read(__u64, __u64 *);
	void write(__u8 *, __u64, __u64);
	__u32 getLBA();
	__u32 getBlockSize();
};

#endif /* USBPROXY_VIRTUALDRIVE_H */
