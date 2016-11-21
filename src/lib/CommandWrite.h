/*
 * CommandWrite.h
 *
 *  Created on: 18 nov 2016
 *      Author: skazza
 */

#ifndef SRC_LIB_COMMANDWRITE_H_
#define SRC_LIB_COMMANDWRITE_H_

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

#endif /* SRC_LIB_COMMANDWRITE_H_ */
