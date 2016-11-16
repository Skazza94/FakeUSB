/*
 * This file is not part of original USBProxy.
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHIDKeyboard.h"

AttackHIDKeyboard::AttackHIDKeyboard() : AttackHID() {}

AttackHIDKeyboard::~AttackHIDKeyboard() {}

/* Autoregisters the class into the AttackFactory */
const bool hasRegistered = AttackFactory::getInstance()->registerClass("keyboard", &AttackHIDKeyboard::createInstance);
