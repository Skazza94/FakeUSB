/*
 * CommandPresskeys.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandPressKeys.h"

CommandPressKeys::CommandPressKeys() : Command() {}

CommandPressKeys::~CommandPressKeys() {}

std::list<__u8 *> * CommandPressKeys::preparePayLoad(std::vector<std::string> * params, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	__u8 * packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	int bytePos = 0x02;
	for(std::vector<std::string>::iterator it = params->begin(); it != params->end(); ++it) {
		packetPressed[bytePos] = this->ascii2USBByte[(*it)];

		bytePos++;
	}
	payLoad->push_back(packetPressed);

	__u8 * packetReleased = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	payLoad->push_back(packetReleased);

	return payLoad;
}


std::vector<std::string> * CommandPressKeys::parseParams(const std::string &paramString) {
	/* Matches alphanumeric chars and + sign (if present) one time */
	std::regex paramRegex("([A-Za-z0-9]*)(\\+)?", std::regex_constants::icase);
	std::smatch matches;
	std::vector<std::string> * paramVector = new std::vector<std::string>;
	std::string tokenizer = paramString;

	/* Iterates and matches the above regex for the whole string */
	while(!tokenizer.empty()) {
		 std::regex_search(tokenizer, matches, paramRegex);
		 paramVector->push_back(matches[1].str());

		 tokenizer = tokenizer.substr(matches[0].str().length());
	}

	return (!paramVector->empty()) ? paramVector : NULL;
}

std::list<__u8 *> * CommandPressKeys::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList, maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("PRESS_KEYS", &CommandPressKeys::createInstance);

