/*
 * CommandPresskeys.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDPRESSKEYS_H_
#define SRC_LIB_COMMANDPRESSKEYS_H_

#include "Command.h"

class CommandPresskeys : public Command {
private:
	std::map<std::string, __u8> ascii2USBByte {
			{"t", 0x17},
			{"enter", 0x28},
			{"backspace", 0x2a},
			{"tab", 0x2b},
			{"f1", 0x3a},
			{"canc", 0x9c},
			{"ctrl", 0xe0},
			{"shift", 0xe1},
			{"alt", 0xe2},
		};

		std::list<__u8 *> * preparePayLoad(std::string, __u16);
public:
	CommandPresskeys();
	~CommandPresskeys();

	std::vector<std::string> * parseParams(std::string);
	std::list<__u8 *> * execute(std::string, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandPresskeys(); }
};

#endif /* SRC_LIB_COMMANDPRESSKEYS_H_ */
