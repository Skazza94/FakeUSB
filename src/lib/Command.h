/*
 * This abstract class represents a generic command. It has two abstract methods, first to parse passed parameters with regexp
 * and the second one to execute the parsed command. Works together with CommandFactory.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_COMMAND_H
#define USBPROXY_COMMAND_H

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

	virtual std::vector<std::string> * parseParams(const std::string &) = 0;
	virtual std::list<std::pair<__u8 *, __u64>> * execute(const std::string &, __u16) = 0;
};

#endif /* USBPROXY_COMMAND_H */
