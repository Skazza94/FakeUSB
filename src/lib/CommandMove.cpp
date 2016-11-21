/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Moves the mouse of x pixels horizontally and y pixels vertically. 
 * Positive numbers are right/down movements, negative numbers are left/up movements. Params are x pixels and y pixels.
 *
 * Author: dibbidouble & Skazza
 */

#include "CommandMove.h"

CommandMove::CommandMove() : Command() {}

CommandMove::~CommandMove() {}

std::list<__u8 *> * CommandMove::preparePayLoad(std::vector<std::string> * params, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	int hMovement = stoi(params->at(0));
	int vMovement = stoi(params->at(1));
	bool isHPositive = (hMovement >= 0);
	bool isVPositive = (vMovement >= 0);

	hMovement = std::abs(hMovement);
	vMovement = std::abs(vMovement);

	int nPackets = std::max(hMovement, vMovement);

	for(int i = 0; i < nPackets; i += 0x7f) {
		__u8 * packetMovement = (__u8 *) calloc(maxPacketSize, sizeof(__u8));

		if(hMovement > 0) {
			int minHMovement = std::min(hMovement, 0x7f);
			packetMovement[0x01] = (isHPositive) ? minHMovement : (0xff - minHMovement + 0x01);
			hMovement -= minHMovement;
		}

		payLoad->push_back(packetMovement);
	}

	for(std::list<__u8 *>::iterator it = payLoad->begin(); it != payLoad->end(); ++it) {
		if(vMovement > 0) {
			int minVMovement = std::min(vMovement, 0x7f);
			(*it)[0x02] = (isVPositive) ? minVMovement : (0xff - minVMovement + 0x01);
			vMovement -= minVMovement;
		}
	}

	return payLoad;
}


std::vector<std::string> * CommandMove::parseParams(const std::string &paramString) {
	std::regex paramRegex("^(-?\\d+),(-?\\d+)$", std::regex_constants::icase);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty() && !matches[2].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());
		paramVector->push_back(matches[2].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandMove::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList, maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("MOVE", &CommandMove::createInstance);


