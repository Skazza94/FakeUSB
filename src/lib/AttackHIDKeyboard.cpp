 /*
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHIDKeyboard.h"

AttackHIDKeyboard::AttackHIDKeyboard() : AttackHID() {
	this->deviceType = "keyboard";
}

AttackHIDKeyboard::~AttackHIDKeyboard() {}

const bool hasRegistered = AttackFactory::registerClass("keyboard", &AttackHIDKeyboard::createInstance);
