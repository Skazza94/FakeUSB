/*
 * CommandClick.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDCLICK_H_
#define SRC_LIB_COMMANDCLICK_H_

#include "Command.h"

class CommandClick : public Command {
private:
	std::map<char, __u8> ascii2USBByte {
		{'l', 0x01},
		{'r', 0x02},
		{'m', 0x04}
	};

	std::list<__u8 *> * preparePayLoad(std::string, __u16);
public:
	CommandClick();
	virtual ~CommandClick();

	std::vector<std::string> * parseParams(std::string);
	std::list<__u8 *> * execute(std::string, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandClick(); }
};

#endif /* SRC_LIB_COMMANDCLICK_H_ */
