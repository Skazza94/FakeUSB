/*
 * CommandDelay.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandDelay.h"

CommandDelay::CommandDelay() : Command() {}

CommandDelay::~CommandDelay() {}

std::vector<std::string> * CommandDelay::parseParams(std::string paramString) {
	std::regex paramRegex("\"(.*)\"", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandDelay::execute(std::string paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if (paramList) {
		int delay = stoi(paramList->at(0), nullptr, 10);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("DELAY", &CommandDelay::createInstance);

