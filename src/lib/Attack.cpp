#include "Attack.h"

Attack::Attack(Device* device) {
	this->device = device;
	printf("Attack inizializzato\n");
	this->configuration_packets.insert(std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8*)>>(0x03, [=](const usb_ctrlrequest packet, __u8* dataptr) -> __u8 { return this->get_string(packet, dataptr); }));
}

Attack::~Attack() {
	// TODO Auto-generated destructor stub
}

int Attack::control_request(const usb_ctrlrequest setup_packet, int * nbytes, __u8 * dataptr) {
	__u8 descType = setup_packet.wValue >> 8;

	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8*)>>::iterator it = this->configuration_packets.find(descType);

	if(it != this->configuration_packets.end())
		*nbytes = (*it).second(setup_packet, dataptr);

	return 0;
}

__u8 Attack::get_string(const usb_ctrlrequest packet, __u8* dataptr) {
	__u8 descIndex = packet.wValue & 0xFF;

	const usb_string_descriptor * s = this->device->get_string(descIndex, packet.wIndex)->get_descriptor();
	memcpy(dataptr, s, s->bLength);

	return s->bLength;
}


