/*
 * This file is not part of original USBProxy.
 * Sub-class of AttackHID.
 * Handles a mouse emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHIDMouse.h"

AttackHIDMouse::AttackHIDMouse() : AttackHID(4) {}

AttackHIDMouse::~AttackHIDMouse() {}

/* Autoregisters the class into the AttackFactory */
const bool hasRegistered = AttackFactory::getInstance()->registerClass("mouse", &AttackHIDMouse::createInstance);
