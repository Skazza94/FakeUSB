/*
 * This file is not part of original USBProxy.
 * Sub-class of AttackHID.
 * Handles a keyboard emulation, making fake packets and sending them to the host! :P
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHIDMultimedia.h"

AttackHIDMultimedia::AttackHIDMultimedia() : AttackHID(4) {}

AttackHIDMultimedia::~AttackHIDMultimedia() {}

/* Autoregisters the class into the AttackFactory */
const bool hasRegistered = AttackFactory::getInstance()->registerClass("multimedia", &AttackHIDMultimedia::createInstance);
