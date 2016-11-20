/*
 * CommandMove.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandMove.h"

CommandMove::CommandMove() : Command() {}

CommandMove::~CommandMove() {}

std::list<__u8 *> * CommandMove::preparePayLoad(std::string commandForMovement, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	std::string command = commandForMovement;

	std::string delimiter = ",";
	size_t pos = command.find(delimiter);

	int hMovement = stoi(command.substr(0, pos), nullptr, 10);
	command.erase(0, pos + delimiter.length());
	int vMovement = stoi(command, nullptr, 10);

	int total_movement = abs(hMovement) + abs(vMovement);
	int i;

	for (i=0 ; i<total_movement ; i++) {
		__u8 * packetMovement = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		__u8 * packetReleased = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		if (hMovement!=0) {				//horizontal_movement
			if (hMovement>0)
				packetMovement[1] = 0x01;
			else
				packetMovement[1] = 0xff;
			payLoad->push_back(packetMovement);
			payLoad->push_back(packetReleased);
		}
		packetMovement[1] = 0x00;
		if (vMovement!=0) {				//horizontal_movement
			if (vMovement>0)
				packetMovement[2] = 0x01;
			else
				packetMovement[2] = 0xff;
			payLoad->push_back(packetMovement);
			payLoad->push_back(packetReleased);
		}
	}

	return payLoad;
}


std::vector<std::string> * CommandMove::parseParams(std::string paramString) {
	std::regex paramRegex("\"(.*)\"", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandMove::execute(std::string paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList->at(0), maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("MOVE", &CommandMove::createInstance);


