/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * - For Device Requests: store a map with an __u16 (what it means depends from the USB protocol used) as key and a callback function to execute as value.
 * When a packet arrives, we know exactly what respond to that request because we have a specific callback function that does the job for us.
 *
 * Author: Skazza
 */

#include "Attack.h"

Attack::Attack(__u32 _delayTimer) {
	this->setupType2Callback.insert(
		std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x0306, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
	this->setupType2Callback.insert(
		std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x0307, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
	this->setupType2Callback.insert(
		std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x0308, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
	this->setupType2Callback.insert(
		std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x0309, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	DELAY_TIMER = _delayTimer * 1000000;
}

Attack::~Attack() {}

int Attack::parseSetupRequest(const usb_ctrlrequest setupPacket, int * nBytes, __u8 * dataPtr) {
	*nBytes = 0;
	
	__u16 searchValue = setupPacket.wValue + setupPacket.bRequest;
	std::map<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator it = this->setupType2Callback.find(searchValue);

	if(it != this->setupType2Callback.end())
		*nBytes = (*it).second(setupPacket, dataPtr);

	return 0;
}

void Attack::startAttack() {
	this->loadAttack();
	this->canAttack = true;
}

/* ~~ Setup Request Callbacks ~~ */
__u8 Attack::getStringDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	__u8 descIndex = packet.wValue & 0xFF;

	const usb_string_descriptor * stringDescriptor = this->device->get_string(descIndex, packet.wIndex)->get_descriptor();
	__le16 packetSize = (packet.wLength < stringDescriptor->bLength) ? packet.wLength : stringDescriptor->bLength;

	memcpy(dataPtr, stringDescriptor, packetSize);

	return packetSize;
}
