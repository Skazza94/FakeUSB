/*
 * Command.h
 *
 *  Created on: 18 nov 2016
 *      Author: skazza
 */

#ifndef SRC_LIB_COMMAND_H_
#define SRC_LIB_COMMAND_H_

#include <list>
#include <string>
#include <regex>

#include "Criteria.h"
#include "CommandFactory.hpp"

class Command {
protected:
	Command();

public:
	virtual ~Command();

	virtual std::vector<std::string> * parseParams(std::string) = 0;
	virtual std::list<__u8 *> * execute(std::string, __u16) = 0;
};

#endif /* SRC_LIB_COMMAND_H_ */
