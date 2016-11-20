/*
 * CommandMove.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDMOVE_H_
#define SRC_LIB_COMMANDMOVE_H_

#include "Command.h"

class CommandMove : public Command {
private:
	std::list<__u8 *> * preparePayLoad(std::string, __u16);
public:
	CommandMove();
	virtual ~CommandMove();

	std::vector<std::string> * parseParams(std::string);
	std::list<__u8 *> * execute(std::string, __u16);

	/* "Creator" method used by the Factory */
	static Command * createInstance() { return new CommandMove(); }
};

#endif /* SRC_LIB_COMMANDMOVE_H_ */
