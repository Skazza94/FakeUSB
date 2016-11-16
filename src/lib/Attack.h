/*
 * This class is cool. Really.
 * What we do here is have a map with the descriptor type field (stored in the setup packet) and a callback function to execute (to handle setup requests).
 * When a packet arrives, we know what send because we have a callback function that does the job for us.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_ATTACK_H
#define USBPROXY_ATTACK_H

#include <map>
#include <functional>
#include <string.h>
#include <linux/usb/ch9.h>

#include "Device.h"
#include "USBString.h"
#include "AttackFactory.h"

class Attack {
protected:
	Device * device;
	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>> setupType2Callback;

	__u8 getStringDescriptor(const usb_ctrlrequest, __u8 *);

public:
	Attack();
	~Attack();
	int parseSetupRequest(const usb_ctrlrequest, int *, __u8 *);
	void setDevice(Device * device) { this->device = device; }
};

#endif /* USBPROXY_ATTACK_H */
