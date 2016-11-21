/*
 * CommandMove.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDMOVEANDCLICK_H_
#define SRC_LIB_COMMANDMOVEANDCLICK_H_

#include <algorithm>

#include "Command.h"
#include "KeyMap.h"

class CommandMoveAndClick : public Command {
private:
	std::list<__u8 *> * preparePayLoad(std::vector<std::string> *, __u16);

public:
	CommandMoveAndClick();
	virtual ~CommandMoveAndClick();

	std::vector<std::string> * parseParams(const std::string &);
	std::list<__u8 *> * execute(const std::string &, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandMoveAndClick(); }
};

#endif /* SRC_LIB_COMMANDMOVEANDCLICK_H_ */
