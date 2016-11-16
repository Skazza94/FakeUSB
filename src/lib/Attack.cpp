/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * When a packet arrives, we know exactly what respond to that request because we have a specific callback function that does the job for us.
 *
 * Author: Skazza
 */

#include "Attack.h"

Attack::Attack() {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x03, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
}

Attack::~Attack() {}

int Attack::parseSetupRequest(const usb_ctrlrequest setupPacket, int * nBytes, __u8 * dataPtr) {
	__u8 descType = setupPacket.wValue >> 8;

	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator it = this->setupType2Callback.find(descType);

	if(it != this->setupType2Callback.end())
		*nBytes = (*it).second(setupPacket, dataPtr);

	return 0;
}

/* ~~ Setup Request Callbacks ~~ */
__u8 Attack::getStringDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	__u8 descIndex = packet.wValue & 0xFF;

	const usb_string_descriptor * stringDescriptor = this->device->get_string(descIndex, packet.wIndex)->get_descriptor();
	memcpy(dataPtr, stringDescriptor, stringDescriptor->bLength);

	return stringDescriptor->bLength;
}
