/*
 * This file is not part of original USBProxy.
 * Sub-class of Command. 
 * Does a mouse click. Parameter is the button to click (Left, Right, Middle).
 *
 * Author: dibbidouble & Skazza
 */

#include "CommandClick.h"

CommandClick::CommandClick() : Command() {}

CommandClick::~CommandClick() {}

std::list<std::pair<__u8 *, __u64>> * CommandClick::preparePayLoad(std::string params, __u16 maxPacketSize) {
	std::list<std::pair<__u8 *, __u64>> * payLoad = new std::list<std::pair<__u8 *, __u64>>;

	__u8 * packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	packet[0x00] = findButton(params.at(0));
	payLoad->push_back(std::pair<__u8 *, __u64>(packet, maxPacketSize));
	
	packet = (__u8 *) calloc(maxPacketSize, sizeof(__u8));
	payLoad->push_back(std::pair<__u8 *, __u64>(packet, maxPacketSize));

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

std::list<std::pair<__u8 *, __u64>> * CommandClick::execute(const std::string &paramString, __u16 maxPacketSize) {
	std::vector<std::string> * paramList = this->parseParams(paramString);

	if(paramList) {
		std::list<std::pair<__u8 *, __u64>> * payLoad = this->preparePayLoad(paramList->at(0), maxPacketSize);
		delete(paramList);

		return payLoad;
	}

	return new std::list<std::pair<__u8 *, __u64>>;
}

/* Autoregisters the class into the CommandFactory */
const bool hasRegistered = CommandFactory::getInstance()->registerClass("CLICK", &CommandClick::createInstance);

