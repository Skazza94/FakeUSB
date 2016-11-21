/*
 * CommandClick.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDCLICK_H_
#define SRC_LIB_COMMANDCLICK_H_

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

#endif /* SRC_LIB_COMMANDCLICK_H_ */
