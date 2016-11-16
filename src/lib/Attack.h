/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * When a packet arrives, we know exactly what respond to that request because we have a specific callback function that does the job for us.
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
	Device * device = NULL;
	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>> setupType2Callback;

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getStringDescriptor(const usb_ctrlrequest, __u8 *);

public:
	Attack();
	~Attack();
	int parseSetupRequest(const usb_ctrlrequest, int *, __u8 *);
	void setDevice(Device * device) { this->device = device; }
};

#endif /* USBPROXY_ATTACK_H */
