 /*
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHIDKeyboard.h"

AttackHIDKeyboard::AttackHIDKeyboard(Device * device) : AttackHID(device) {
	this->deviceType = "keyboard";
}

AttackHIDKeyboard::~AttackHIDKeyboard() {}