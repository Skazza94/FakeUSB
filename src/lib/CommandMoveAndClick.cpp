/*
 * CommandMove.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandMoveAndClick.h"

CommandMoveAndClick::CommandMoveAndClick() : Command() {}

CommandMoveAndClick::~CommandMoveAndClick() {}

std::list<__u8 *> * CommandMoveAndClick::preparePayLoad(std::vector<std::string> * params, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	__u8 button = findButton(params->at(0).at(0));
	int hMovement = stoi(params->at(1));
	int vMovement = stoi(params->at(2));
	bool isHPositive = (hMovement >= 0);
	bool isVPositive = (vMovement >= 0);

	hMovement = std::abs(hMovement);
	vMovement = std::abs(vMovement);

	int nPackets = std::max(hMovement, vMovement);

	for(int i = 0; i < nPackets; i += 0x7f) {
		__u8 * packetMovement = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		packetMovement[0x00] = button;

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

	__u8 * packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	packet[0x00] = button;
	payLoad->push_front(packet);

	packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	payLoad->push_back(packet);

	return payLoad;
}

std::vector<std::string> * CommandMoveAndClick::parseParams(const std::string &paramString) {
	std::regex paramRegex("^(R|L|M) (-?\\d+),((-)?\\d+)$", std::regex_constants::icase);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty() && !matches[2].str().empty() && !matches[3].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());
		paramVector->push_back(matches[2].str());
		paramVector->push_back(matches[3].str());

		return paramVector;
	}

	return NULL;
}


std::list<__u8 *> * CommandMoveAndClick::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList, maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("MOVE_AND_CLICK", &CommandMoveAndClick::createInstance);


