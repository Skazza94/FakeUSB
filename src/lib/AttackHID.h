/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs, parses file commands and returns payloads.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHID_H
#define USBPROXY_ATTACKHID_H

#include <string>
#include <regex>
#include <fstream>
#include <iterator>
#include <tuple>
#include <sys/stat.h>

#include "Attack.h"
#include "ConfigParser.h"
#include "Command.h"

class AttackHID : public Attack {
protected:
	AttackHID(__u32);

	std::list<std::string> * attackCommands;
	std::tuple<std::string, __u8, std::string> parseCommand(const std::string &);
	void loadAttack();

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~AttackHID();

	void getNextPayload(std::list<std::pair<__u8 *, __u64>> **, __u8, __u16);
};

#endif /* USBPROXY_ATTACKHID_H */
