/*
 * This file is not part of original USBProxy.
 * This class is cool. Really. What we do here is:
 * - For Setup Requests: store a map with the descriptor type field (took from the setup packet) as key and a callback function to execute as value.
 * When a packet arrives, we know exactly what respond to that request because we have a specific callback function that does the job for us.
 *
 * Author: Skazza
 */

#include "Attack.h"

Attack::Attack(__u32 _delayTimer) {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x03, std::bind(&Attack::getStringDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	DELAY_TIMER = _delayTimer * 1000000;
	this->attackCommands = new std::list<std::string>;
}

Attack::~Attack() {
	delete(this->attackCommands);
}

int Attack::parseSetupRequest(const usb_ctrlrequest setupPacket, int * nBytes, __u8 * dataPtr) {
	__u8 descType = setupPacket.wValue >> 8;
	__u8 bRequest = setupPacket.bRequest;

	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator itDesc = this->setupType2Callback.find(descType);
	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>::iterator itRequest = this->setupType2Callback.find(bRequest);

	if(itDesc != this->setupType2Callback.end())
		*nBytes = (*itDesc).second(setupPacket, dataPtr);
	else if(itRequest != this->setupType2Callback.end())
		*nBytes = (*itRequest).second(setupPacket, dataPtr);

	return 0;
}

std::pair<std::string, std::string> * Attack::parseCommand(std::string command) {
	std::regex commandRegex("^([A-Z]+) (.*)$", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(command, matches, commandRegex);

	if(!matches[1].str().empty() && !matches[2].str().empty())
		return new std::pair<std::string, std::string>(matches[1].str(), matches[2].str());

	return NULL;
}

void Attack::loadAttack() {
	this->attackCommands->push_back("WRITE \"hello world\"");
}

std::list<__u8 *> * Attack::getNextPayload(__u8 endpoint, __u16 maxPacketSize) {
	if(endpoint == 0x81) {
		if(!this->attackCommands->empty()) {
			std::string commandString = this->attackCommands->front();
			this->attackCommands->pop_front();

			fprintf(stderr, "Command: %s\n", commandString.c_str());

			std::pair<std::string, std::string> * commandAndParams = this->parseCommand(commandString);

			if(commandAndParams) {
				Command * command = CommandFactory::getInstance()->createInstance(commandAndParams->first);
				std::list<__u8 *> * payload = new std::list<__u8 *>;

				if(command) payload = command->execute(commandAndParams->second, maxPacketSize);

				delete(commandAndParams);

				return payload;
			}
		}
	}

	return new std::list<__u8 *>;
}

void Attack::startAttack() {
	this->loadAttack();
	this->canAttack = true;
}

/* ~~ Setup Request Callbacks ~~ */
__u8 Attack::getStringDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	__u8 descIndex = packet.wValue & 0xFF;

	const usb_string_descriptor * stringDescriptor = this->device->get_string(descIndex, packet.wIndex)->get_descriptor();
	memcpy(dataPtr, stringDescriptor, stringDescriptor->bLength);

	return stringDescriptor->bLength;
}
