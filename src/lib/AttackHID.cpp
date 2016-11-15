/*
 * AttackHID.cpp
 *
 *  Created on: 15 nov 2016
 *      Author: dibbidouble
 */

#include "AttackHID.h"

AttackHID::AttackHID(Device* device) : Attack(device) {
	this->device_type = "keyboard";
//	this->device_proxy = this->device->get_DeviceProxy();
	this->configuration_packets.insert(std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8*)>>(0x22, [=](const usb_ctrlrequest packet, __u8* dataptr) -> __u8 { return this->get_HIDreport(packet, dataptr); }));
}

AttackHID::~AttackHID() {
	// TODO Auto-generated destructor stub
}

__u8 AttackHID::get_HIDreport(const usb_ctrlrequest packet, __u8* dataptr) {
	std::ostringstream deviceConfiguration; deviceConfiguration << "/home/debian/AntiUSBProxy/config/HIDreport/" << this->device_type << "HIDreport";

	__u8 dim =0;

	FILE * configFileHandler = fopen(deviceConfiguration.str().c_str(), "rb");

	if(configFileHandler) {
		struct stat info;
		stat(deviceConfiguration.str().c_str(), &info);
		dim = info.st_size;
		fread(dataptr, dim, 1, configFileHandler);
	}

	return dim;
}
