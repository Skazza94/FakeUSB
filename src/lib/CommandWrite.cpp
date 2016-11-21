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
	std::list<__u8 *> * tempPayload = new std::list<__u8 *>;

	for(unsigned int i = 0; i < stringToWrite.length(); ++i) {
		__u8 * packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		std::pair<__u8, __u8> firstAndThirdByte = findCharacter(stringToWrite[i]);

		packetPressed[0x00] = firstAndThirdByte.first;
		packetPressed[0x02] = firstAndThirdByte.second;

		tempPayload->push_back(packetPressed);
	}

	for(std::list<__u8 *>::iterator it = tempPayload->begin(); it != tempPayload->end(); ++it) {
		std::list<__u8 *>::iterator nextPacket = std::next(it, 1);
		__u8 * packetPressed = NULL;

		if(nextPacket != tempPayload->end()) {
			if((*it)[0x00] == 0x00 && (*nextPacket)[0x00] == 0x00) {
				packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
				packetPressed[0x02] = (*nextPacket)[0x02];
				packetPressed[0x03] = (*it)[0x02];
			}
		}

		payLoad->push_back(*it);
		if(packetPressed) {
			payLoad->push_back(packetPressed);

			__u8 thirdByte = packetPressed[0x03];
			packetPressed = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
			packetPressed[0x02] = thirdByte;
			payLoad->push_back(packetPressed);

			++it;
		}

		__u8 * packetReleased = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		payLoad->push_back(packetReleased);
	}

	delete(tempPayload);

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
