/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Simulates a keyboard shortcut or a keyboard button pressing, including "modifiers" on byte 0. 
 * Param is a string with buttons to press separated by a + sign. NO MORE THAN ONE NON-MODIFIER INTO THE STRING.
 * Ex: CTRL+ALT+t, NOT ALLOWED: CTRL+t+a
 *
 * Author: Skazza
 */

#include "CommandPressKeys.h"

CommandPressKeys::CommandPressKeys() : Command() {}

CommandPressKeys::~CommandPressKeys() {}

std::list<__u8 *> * CommandPressKeys::preparePayLoad(std::vector<std::string> * params, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	__u8 * packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	for(std::vector<std::string>::iterator it = params->begin(); it != params->end(); ++it) {
		std::pair<__u8, __u8> firstAndThirdByte = findKey((*it));

		if(firstAndThirdByte.first == 0x00 && firstAndThirdByte.second == 0x00)
			firstAndThirdByte = findCharacter((*it).at(0));

		packetPressed[0x00] += firstAndThirdByte.first;
		packetPressed[0x02] = firstAndThirdByte.second;
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

	if(!paramVector->empty())
		return paramVector;
	else {
		delete(paramVector);
		return NULL;
	}
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

