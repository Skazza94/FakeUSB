/*
 * CommandWrite.cpp
 *
 *  Created on: 18 nov 2016
 *      Author: skazza
 */

#include "CommandWrite.h"

CommandWrite::CommandWrite() : Command() {}

CommandWrite::~CommandWrite() {}

std::list<__u8 *> * CommandWrite::preparePayLoad(std::string stringToWrite, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	for(unsigned int i = 0; i < stringToWrite.length(); ++i) {
		__u8 * packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		std::pair<__u8, __u8> firstAndThirdByte = findCharacter(stringToWrite[i]);

		packetPressed[0x00] = firstAndThirdByte.first;
		packetPressed[0x02] = firstAndThirdByte.second;

		payLoad->push_back(packetPressed);

		__u8 * packetReleased = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		payLoad->push_back(packetReleased);
	}

	return payLoad;
}


std::vector<std::string> * CommandWrite::parseParams(const std::string &paramString) {
	std::regex paramRegex("^\"(.*)\"$", std::regex_constants::icase);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandWrite::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList->at(0), maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("WRITE", &CommandWrite::createInstance);
