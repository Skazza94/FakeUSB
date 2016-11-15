 /*
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_ATTACKHIDKB_H
#define USBPROXY_ATTACKHIDKB_H

#include "AttackHID.h"

class AttackHIDKeyboard : public AttackHID {
public:
	AttackHIDKeyboard(Device *);
	~AttackHIDKeyboard();
};

#endif /* USBPROXY_ATTACKHIDKB_H */
