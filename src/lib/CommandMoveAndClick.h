/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Moves the mouse of x pixels horizontally and y pixels vertically while clicking a button. 
 * Positive numbers are right/down movements, negative numbers are left/up movements. Params are button to click (Left, Right, Middle), x pixels and y pixels.
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_COMMANDMOVEANDCLICK_H
#define USBPROXY_COMMANDMOVEANDCLICK_H

#include <algorithm>

#include "Command.h"
#include "KeyMap.h"

class CommandMoveAndClick : public Command {
private:
	std::list<std::pair<__u8 *, __u64>> * preparePayLoad(std::vector<std::string> *, __u16);

public:
	CommandMoveAndClick();
	virtual ~CommandMoveAndClick();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<std::pair<__u8 *, __u64>> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandMoveAndClick(); }
};

#endif /* USBPROXY_COMMANDMOVEANDCLICK_H */
