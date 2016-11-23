/*
 * This file is not part of original USBProxy.
 * Implements Factory pattern for Command classes inheriting from the Abstract Factory.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_COMMANDFACTORY_H
#define USBPROXY_COMMANDFACTORY_H

#include "Command.h"
#include "Factory.hpp"

class Command;

class CommandFactory : public Factory<Command> {
	public:
		CommandFactory() : Factory() {}
		~CommandFactory() {}
};

#endif /* USBPROXY_COMMANDFACTORY_H */
