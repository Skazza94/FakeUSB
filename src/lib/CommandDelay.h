/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Sleeps the thread for N ms before proceeding with command execution. Parameter is the number of ms to wait.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_COMMANDDELAY_H
#define USBPROXY_COMMANDDELAY_H

#include <chrono>
#include <thread>

#include "Command.h"

class CommandDelay : public Command {
private:
	std::list<__u8 *> * preparePayLoad(float, __u16);

public:
	CommandDelay();
	~CommandDelay();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<__u8 *> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandDelay(); }
};

#endif /* USBPROXY_COMMANDDELAY_H */
