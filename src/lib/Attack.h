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
#include <list>
#include <functional>
#include <string.h>
#include <linux/usb/ch9.h>

#include "AttackFactory.hpp"
#include "Device.h"
#include "USBString.h"
#include "ConfigParser.h"

class Attack {
protected:
	Attack(__u32);

	Device * device = NULL;
	ConfigParser * cfg = NULL;
	/* This boolean tells when the Manager has initialized all the endpoint threads. */
	bool canAttack = false;
	/* How many seconds wait before start sending packets from fake device.
	 * This is needed because we have to wait the completion of the device setup.
	 * Needs to be tweaked manually for each Attack sub-class.
	 */
	__u32 DELAY_TIMER = 0;

	virtual void loadAttack() = 0;

	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>> setupType2Callback;

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getStringDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~Attack();

	int parseSetupRequest(const usb_ctrlrequest, int *, __u8 *);
	void setDevice(Device * device) { this->device = device; }
	void setCfgParser(ConfigParser * cfg) { this->cfg = cfg; }

	virtual std::list<__u8 *> * getNextPayload(__u8, __u16) = 0;

	void startAttack();
	bool canStartAttack() { if(DELAY_TIMER) DELAY_TIMER--; return this->canAttack && (DELAY_TIMER == 0); }
};

#endif /* USBPROXY_ATTACK_H */
