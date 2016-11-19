/*
 * CommandPresskeys.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandPresskeys.h"

CommandPresskeys::CommandPresskeys() : Command() {}

CommandPresskeys::~CommandPresskeys() {}

std::list<__u8 *> * CommandPresskeys::preparePayLoad(std::string commandToExecute, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	std::string command = commandToExecute;

	std::string delimiter = "+";
	std::string key;
	__u8 * packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	size_t pos = 0;
	int i = 2;
	while ((pos=command.find(delimiter)) != std::string::npos) {
		key = command.substr(0, pos);
		packetPressed[i] = this->ascii2USBByte[key];
		command.erase(0, pos + delimiter.length());
		i++;
	}
	packetPressed[i] = this->ascii2USBByte[command];
	payLoad->push_back(packetPressed);
	__u8 * packetReleased = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	payLoad->push_back(packetReleased);

	return payLoad;
}


std::vector<std::string> * CommandPresskeys::parseParams(std::string paramString) {
	std::regex paramRegex("\"(.*)\"", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandPresskeys::execute(std::string paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList->at(0), maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("PRESSKEYS", &CommandPresskeys::createInstance);

