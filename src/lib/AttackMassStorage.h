/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for mass storages and handles the bulk transfer.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_ATTACKMASSSTORAGE_H
#define USBPROXY_ATTACKMASSSTORAGE_H

#include <string>

#include "Attack.h"

class AttackMassStorage : public Attack {
protected:
	AttackMassStorage();

	void loadAttack() { }

	/* ~~ Setup Request Callbacks ~~ */
//	__u8 getHIDReportDescriptor(const usb_ctrlrequest, __u8 *);

public:
	virtual ~AttackMassStorage();

	void getNextPayload(std::list<__u8 *> **, __u8, __u16);
	/* "Creator" method used by the Factory */
	static Attack * createInstance() { return new AttackMassStorage(); }
};

#endif /* USBPROXY_ATTACKMASSSTORAGE_H */
