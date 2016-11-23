/*
 * This file is not part of original USBProxy.
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHIDMM_H
#define USBPROXY_ATTACKHIDMM_H

#include "AttackHID.h"

class AttackHIDMultimedia : public AttackHID {
public:
	AttackHIDMultimedia();
	~AttackHIDMultimedia();

	/* "Creator" method used by the Factory */
	static Attack * createInstance() { return new AttackHIDMultimedia(); }
};

#endif /* USBPROXY_ATTACKHIDMM_H */
