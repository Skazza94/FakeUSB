/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Does a mouse click. Parameter is the button to click (Left, Right, Middle).
 *
 * Author: dibbidouble & Skazza
 */

#ifndef USBPROXY_COMMANDCLICK_H
#define USBPROXY_COMMANDCLICK_H

#include "Command.h"
#include "KeyMap.h"

class CommandClick : public Command {
private:
	std::list<__u8 *> * preparePayLoad(std::string, __u16);

public:
	CommandClick();
	virtual ~CommandClick();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<__u8 *> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandClick(); }
};

#endif /* USBPROXY_COMMANDCLICK_H */
