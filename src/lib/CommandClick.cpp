/*
 * CommandClick.cpp
 *
 *  Created on: 19 nov 2016
 *      Author: dibbidouble
 */

#include "CommandClick.h"

CommandClick::CommandClick() : Command() {}

CommandClick::~CommandClick() {}

std::list<__u8 *> * CommandClick::preparePayLoad(std::string params, __u16 maxPacketSize) {
	std::list<__u8 *> * payLoad = new std::list<__u8 *>;

	__u8 * packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));

	packet[0x00] = findButton(params.at(0));
	payLoad->push_back(packet);
	
	packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	payLoad->push_back(packet);

	return payLoad;
}


std::vector<std::string> * CommandClick::parseParams(const std::string &paramString) {
	std::regex paramRegex("^(R|L|M)$", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(paramString, matches, paramRegex);

	if(!matches[1].str().empty()) {
		std::vector<std::string> * paramVector = new std::vector<std::string>;
		paramVector->push_back(matches[1].str());

		return paramVector;
	}

	return NULL;
}

std::list<__u8 *> * CommandClick::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<__u8 *> * payLoad = this->preparePayLoad(paramList->at(0), maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<__u8 *>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("CLICK", &CommandClick::createInstance);

