/*keyboard
 * This file is not part of original USBProxy.
 * Sub-class of AttackHID.
 * Handles a mouse emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHIDMOUSE_H
#define USBPROXY_ATTACKHIDMOUSE_H

#include "AttackHID.h"

class AttackHIDMouse : public AttackHID {
public:
	AttackHIDMouse();
	~AttackHIDMouse();

	/* "Creator" method used by the Factory */
	static Attack * createInstance() { return new AttackHIDMouse(); }
};

#endif /* USBPROXY_ATTACKHIDMOUSE_H */
