/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * - For Device Requests: store a map with an __u16 (what it means depends from the USB protocol used) as key and a callback function to execute as value.
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
#include "Configuration.h"
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

	/* We need this method because we need to map each IN endpoint with an OUT endpoint on our emulated device */
	void mapOutEpToInEp();
	/* This map maps in endpoints to out endpoints, so we know on which endpoint send data */
	std::map<__u8, __u8> outEp2InEp;

	/* This map handles setup requests */
	std::map<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>> setupType2Callback;
	/* This map handles specific device requests */
	std::map<__u16, std::function<void(__u8 *, std::list<std::pair<__u8 *, __u64>> **)>> deviceRequest2Callback;

	/* ~~ Setup Request Callbacks ~~ */
	__u8 getStringDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~Attack();

	/* Needed because constructor is no-arg for Factory */
	void setDevice(Device * device) { this->device = device; }
	void setCfgParser(ConfigParser * cfg) { this->cfg = cfg; }

	int parseSetupRequest(const usb_ctrlrequest, int *, __u8 *);
	virtual void parseDeviceRequest(__u16, __u8 *, __u64, std::list<std::pair<__u8 *, __u64>> **) { }

	virtual void getNextPayload(std::list<std::pair<__u8 *, __u64>> **, __u8, __u16) = 0;

	__u8 getInEpForOutEp(__u8);

	void startAttack();
	bool canStartAttack() { if(DELAY_TIMER) DELAY_TIMER--; return this->canAttack && (DELAY_TIMER == 0); }
};

#endif /* USBPROXY_ATTACK_H */
