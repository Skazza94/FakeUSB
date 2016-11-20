/*
 * CommandWrite.h
 *
 *  Created on: 18 nov 2016
 *      Author: skazza
 */

#ifndef SRC_LIB_COMMANDWRITE_H_
#define SRC_LIB_COMMANDWRITE_H_

#include "Command.h"

class CommandWrite : public Command {
private:
	std::map<char, __u8> ascii2USBByte {
		{'a', 0x04},
		{'b', 0x05},
		{'c', 0x06},
		{'d', 0x07},
		{'e', 0x08},
		{'f', 0x09},
		{'g', 0x0a},
		{'h', 0x0b},
		{'i', 0x0c},
		{'j', 0x0d},
		{'k', 0x0e},
		{'l', 0x0f},
		{'m', 0x10},
		{'n', 0x11},
		{'o', 0x12},
		{'p', 0x13},
		{'q', 0x14},
		{'r', 0x15},
		{'s', 0x16},
		{'t', 0x17},
		{'u', 0x18},
		{'v', 0x19},
		{'w', 0x1a},
		{'x', 0x1b},
		{'y', 0x1c},
		{'z', 0x1d},
		{'1', 0x1e},
		{'2', 0x1f},
		{'3', 0x20},
		{'4', 0x21},
		{'5', 0x22},
		{'6', 0x23},
		{'7', 0x24},
		{'8', 0x25},
		{'9', 0x26},
		{'0', 0x27},
		{' ', 0x2c}
	};

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
