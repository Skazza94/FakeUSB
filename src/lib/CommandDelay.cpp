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

std::list<std::pair<__u8 *, __u64>> * CommandDelay::preparePayLoad(float msDelay, __u16 maxPacketSize) {
	std::list<std::pair<__u8 *, __u64>> * payLoad = new std::list<std::pair<__u8 *, __u64>>;
	float newDelay = msDelay / 100;

	while(newDelay > 0) {
		__u8 * packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
		payLoad->push_back(std::pair<__u8 *, __u64>(packet, maxPacketSize));

		newDelay--;
	}

	return payLoad;
}

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

std::list<std::pair<__u8 *, __u64>> * CommandDelay::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		float delay = std::stof(paramList->at(0));

		std::list<std::pair<__u8 *, __u64>> * payLoad = this->preparePayLoad(delay, maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<std::pair<__u8 *, __u64>>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("DELAY", &CommandDelay::createInstance);
