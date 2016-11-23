/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for mass storages and handles the bulk transfer.
 *
 * Author: Skazza
 */

#include "AttackMassStorage.h"

AttackMassStorage::AttackMassStorage() : Attack(4) {
//	this->setupType2Callback.insert(
//			std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
//					0x22, std::bind(&AttackHID::getHIDReportDescriptor, this, std::placeholders::_1, std::placeholders::_2)
//			)
//	);
}

AttackMassStorage::~AttackMassStorage() {}

void AttackMassStorage::getNextPayload(std::list<__u8 *> ** payload, __u8 endpoint, __u16 maxPacketSize) {

}

///* ~~ Setup Request Callbacks ~~ */
//__u8 AttackHID::getHIDReportDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
//	std::string deviceHIDReport = "/home/debian/AntiUSBProxy/config/" + this->cfg->get("Device") + "HIDReport/iface" + std::to_string(packet.wIndex);
//
//	FILE * hidReportFileHandler = fopen(deviceHIDReport.c_str(), "rb");
//
//	if(hidReportFileHandler) {
//		fseek(hidReportFileHandler, 0, SEEK_END);
//		long int structSize = ftell(hidReportFileHandler);
//		rewind(hidReportFileHandler);
//
//		fread(dataPtr, structSize, 1, hidReportFileHandler);
//
//		fclose(hidReportFileHandler);
//		return structSize;
//	}
//
//	return 0;
//}

/* Autoregisters the class into the AttackFactory */
const bool hasRegistered = AttackFactory::getInstance()->registerClass("mass-storage", &AttackMassStorage::createInstance);
