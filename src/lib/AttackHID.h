/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHID_H
#define USBPROXY_ATTACKHID_H

#include <sstream>
#include <string>

#include "Attack.h"

class AttackHID : public Attack {
protected:
	/* ~~ Setup Request Callbacks ~~ */
	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	AttackHID();
	~AttackHID();
};

#endif /* USBPROXY_ATTACKHID_H */
