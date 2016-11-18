/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs.
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHID.h"

AttackHID::AttackHID(__u32 _delayTimer) : Attack(_delayTimer) {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x22, std::bind(&AttackHID::getHIDReportDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);

	this->attackCommands = new std::list<std::string>;
}

AttackHID::~AttackHID() {
	delete(this->attackCommands);
}

std::pair<std::string, std::string> * AttackHID::parseCommand(std::string command) {
	std::regex commandRegex("^([A-Za-z_]+) (.*)$", std::regex_constants::icase | std::regex::extended);
	std::smatch matches; std::regex_search(command, matches, commandRegex);

	if(!matches[1].str().empty() && !matches[2].str().empty()) {
		std::string extractedCmd = matches[1].str();
		std::transform(extractedCmd.begin(), extractedCmd.end(), extractedCmd.begin(), ::toupper);
		return new std::pair<std::string, std::string>(extractedCmd, matches[2].str());
	}

	return NULL;
}

std::list<__u8 *> * AttackHID::getNextPayload(__u8 endpoint, __u16 maxPacketSize) {
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

void AttackHID::loadAttack() {
	/* TODO: Load some endpoints criteria in some way... */

	this->attackCommands->push_back("WRITE \"hello world\"");
}

/* ~~ Setup Request Callbacks ~~ */
__u8 AttackHID::getHIDReportDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	std::string deviceHIDReport = "/home/debian/AntiUSBProxy/config/" + this->deviceProxy->cfg->get("Device") + "HIDReport/iface" + std::to_string(packet.wIndex);

	FILE * hidReportFileHandler = fopen(deviceHIDReport.c_str(), "rb");

	if(hidReportFileHandler) {
		fseek(hidReportFileHandler, 0, SEEK_END);
		long int structSize = ftell(hidReportFileHandler);
		rewind(hidReportFileHandler);

		fread(dataPtr, structSize, 1, hidReportFileHandler);

		fclose(hidReportFileHandler);
		return structSize;
	}

	return 0;
}
