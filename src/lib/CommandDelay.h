/*
 * CommandDelay.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDDELAY_H_
#define SRC_LIB_COMMANDDELAY_H_

#include <chrono>
#include <thread>

#include "Command.h"

class CommandDelay : public Command {
public:
	CommandDelay();
	~CommandDelay();

	std::vector<std::string> * parseParams(std::string);
	std::list<__u8 *> * execute(std::string, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandDelay(); }
};

#endif /* SRC_LIB_COMMANDDELAY_H_ */
