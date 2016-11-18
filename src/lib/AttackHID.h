/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHID_H
#define USBPROXY_ATTACKHID_H

#include <string>

#include "Attack.h"
#include "DeviceProxy.h"
#include "ConfigParser.h"

class AttackHID : public Attack {
protected:
	AttackHID(__u32);

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~AttackHID();
};

#endif /* USBPROXY_ATTACKHID_H */
