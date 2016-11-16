 /*
 * Sub-class of Attack. Adds few specific descriptor type fields + callbacks for HIDs.
 * We also have a string that stores the device type which we're emulating. This is used in sub-classes.
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
	std::string deviceType;
	
	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	AttackHID();
	~AttackHID();
};

#endif /* USBPROXY_ATTACKHID_H */
