/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs.
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHID.h"

AttackHID::AttackHID(__u32 _delayTimer) : Attack(_delayTimer) {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x22, std::bind(&AttackHID::getHIDReportDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
}

AttackHID::~AttackHID() {}

/* ~~ Setup Request Callbacks ~~ */
__u8 AttackHID::getHIDReportDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	std::string deviceHIDReport = "/home/debian/AntiUSBProxy/config/" + this->deviceProxy->cfg->get("Device") + "HIDReport/iface" + std::to_string(packet.wIndex);

	FILE * hidReportFileHandler = fopen(deviceHIDReport.c_str(), "rb");

	if(hidReportFileHandler) {
		fseek(hidReportFileHandler, 0, SEEK_END);
		long int structSize = ftell(hidReportFileHandler);
		rewind(hidReportFileHandler);

		fread(dataPtr, structSize, 1, hidReportFileHandler);

		fclose(hidReportFileHandler);
		return structSize;
	}

	return 0;
}
