/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * When a packet arrives, we know exactly what respond to that request because we have a specific callback function that does the job for us.
 *
 * Author: Skazza
 */

#include "Attack.h"

Attack::Attack(__u32 _delayTimer) {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x03, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	DELAY_TIMER = _delayTimer * 1000000;
}

Attack::~Attack() {}

int Attack::parseSetupRequest(const usb_ctrlrequest setupPacket, int * nBytes, __u8 * dataPtr) {
	__u8 descType = setupPacket.wValue >> 8;
	__u8 bRequest = setupPacket.bRequest;

	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator itDesc = this->setupType2Callback.find(descType);
	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator itRequest = this->setupType2Callback.find(bRequest);

	if(itDesc != this->setupType2Callback.end())
		*nBytes = (*itDesc).second(setupPacket, dataPtr);
	else if(itRequest != this->setupType2Callback.end())
		*nBytes = (*itRequest).second(setupPacket, dataPtr);

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
	memcpy(dataPtr, stringDescriptor, stringDescriptor->bLength);

	return stringDescriptor->bLength;
}
