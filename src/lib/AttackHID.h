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
#include <regex>
#include <algorithm>

#include "Attack.h"
#include "DeviceProxy.h"
#include "ConfigParser.h"
#include "Command.h"

class AttackHID : public Attack {
protected:
	AttackHID(__u32);

	std::list<std::string> * attackCommands;
	std::pair<std::string, std::string> * parseCommand(std::string);
	void loadAttack();

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~AttackHID();

	std::list<__u8 *> * getNextPayload(__u8, __u16);
};

#endif /* USBPROXY_ATTACKHID_H */
