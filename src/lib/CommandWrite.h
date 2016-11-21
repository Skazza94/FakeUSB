/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Writes a string as a keyboard will do. Characters without modifiers are sent in a single packet to speed up typing.
 * Param is a double-quote delimited string with the text to write.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_COMMANDWRITE_H
#define USBPROXY_COMMANDWRITE_H

#include <iterator>

#include "Command.h"
#include "KeyMap.h"

class CommandWrite : public Command {
private:
	std::list<__u8 *> * preparePayLoad(std::string, __u16);

public:
	CommandWrite();
	~CommandWrite();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<__u8 *> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandWrite(); }
};

#endif /* USBPROXY_COMMANDWRITE_H */
