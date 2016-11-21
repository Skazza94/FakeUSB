/*
 * CommandPresskeys.h
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_COMMANDPRESSKEYS_H_
#define SRC_LIB_COMMANDPRESSKEYS_H_

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

#endif /* SRC_LIB_COMMANDPRESSKEYS_H_ */
