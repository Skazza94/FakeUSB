/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Simulates a keyboard shortcut or a keyboard button pressing, including "modifiers" on byte 0. 
 * Param is a string with buttons to press separated by a + sign. NO MORE THAN ONE NON-MODIFIER INTO THE STRING.
 * Ex: CTRL+ALT+t, NOT ALLOWED: CTRL+t+a
 *
 * Author: Skazza
 */

#ifndef USBPROXY_COMMANDPRESSKEYS_H
#define USBPROXY_COMMANDPRESSKEYS_H

#include "Command.h"
#include "KeyMap.h"

class CommandPressKeys : public Command {
private:
	std::list<__u8 *> * preparePayLoad(std::vector<std::string> *, __u16);

public:
	CommandPressKeys();
	~CommandPressKeys();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<__u8 *> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandPressKeys(); }
};

#endif /* USBPROXY_COMMANDPRESSKEYS_H */
