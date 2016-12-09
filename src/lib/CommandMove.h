/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Moves the mouse of x pixels horizontally and y pixels vertically. 
 * Positive numbers are right/down movements, negative numbers are left/up movements. Params are x pixels and y pixels.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_COMMANDMOVE_H
#define USBPROXY_COMMANDMOVE_H

#include <algorithm>

#include "Command.h"

class CommandMove : public Command {
private:
	std::list<std::pair<__u8 *, __u64>> * preparePayLoad(std::vector<std::string> *, __u16);

public:
	CommandMove();
	virtual ~CommandMove();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<std::pair<__u8 *, __u64>> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandMove(); }
};

#endif /* USBPROXY_COMMANDMOVE_H */
