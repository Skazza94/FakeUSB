/*
 * This file is not part of original USBProxy.
 * Implements Factory pattern for Attack classes inheriting from the Abstract Factory.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_ATTACKFACTORY_H
#define USBPROXY_ATTACKFACTORY_H

#include "Attack.h"
#include "Factory.hpp"

class Attack;

class AttackFactory : public Factory<Attack> {
	public:
		AttackFactory() : Factory() {}
		~AttackFactory() {}
};

#endif /* USBPROXY_ATTACKFACTORY_H */
