/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Sleeps the thread for N ms before proceeding with command execution. Parameter is the number of ms to wait.
 *
 * Author: dibbidouble & Skazza
 */

#include "CommandDelay.h"

CommandDelay::CommandDelay() : Command() {}

CommandDelay::~CommandDelay() {}

std::vector<std::string> * CommandDelay::parseParams(const std::string &paramString) {
	std::regex paramRegex("^(\\d)+$", std::regex_constants::icase);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[0].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[0].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandDelay::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if (paramList) {
		int delay = stoi(paramList->at(0));

		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("DELAY", &CommandDelay::createInstance);

