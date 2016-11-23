/*
 * This file is not part of original USBProxy.
 * Sub-class of Attack. 
 * Adds few specific descriptor type fields + callbacks for HIDs, parses file commands and returns payloads.
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHID.h"

AttackHID::AttackHID(__u32 _delayTimer) : Attack(_delayTimer) {
	this->setupType2Callback.insert(
			std::pair<__u16, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
				0x2206,
				std::bind(&AttackHID::getHIDReportDescriptor, this, std::placeholders::_1, std::placeholders::_2)
			)
	);

	this->attackCommands = new std::list<std::string>;
}

AttackHID::~AttackHID() {
	delete(this->attackCommands);
}

std::tuple<std::string, __u8, std::string> AttackHID::parseCommand(const std::string &command) {
	std::regex commandRegex("^([A-Za-z_]+)\\(([0-9a-fA-F]{2})\\)(\\s)+(.*)$", std::regex_constants::icase);
	std::smatch matches; std::regex_search(command, matches, commandRegex);

	if(!matches[1].str().empty() && !matches[2].str().empty() && !matches[4].str().empty())
		return std::make_tuple(matches[1].str(), std::stoi(matches[2].str(), 0, 16), matches[4].str());


	return std::make_tuple("", 0x00, "");
}


void AttackHID::getNextPayload(std::list<__u8 *> ** payload, __u8 endpoint, __u16 maxPacketSize) {
	if(!this->attackCommands->empty()) {
		std::string commandString = this->attackCommands->front();
		std::string commandName, commandParams; __u8 ep;

		std::tie(commandName, ep, commandParams) = this->parseCommand(commandString);

		if(!commandName.empty()) {
			if(endpoint == ep) {
				fprintf(stderr, "Command: %s\n", commandString.c_str());

				this->attackCommands->pop_front();
				Command * command = CommandFactory::getInstance()->createInstance(commandName);

				if(command) {
					std::list<__u8 *> * newPayload = command->execute(commandParams, maxPacketSize);
					std::copy(newPayload->begin(), newPayload->end(), std::back_insert_iterator<std::list<__u8 *>>(**payload));

					delete(command);
					delete(newPayload);
				}
			}
		}
	}
}

void AttackHID::loadAttack() {
	std::ifstream attackFile(this->cfg->get("AttackFile"), std::ios::in);
	std::string line;

	if (attackFile.good()) {
		while (!attackFile.eof()) {
			std::getline(attackFile, line);

			if(!line.empty()) {
				if(line.at(0) != '#')
					this->attackCommands->push_back(line);
			}
		}
	}

	attackFile.close();
}

/* ~~ Setup Request Callbacks ~~ */
__u8 AttackHID::getHIDReportDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	__u8 activeConfigIndex = this->device->get_active_configuration()->get_descriptor()->bConfigurationValue - 1;
	std::string hidReportFolder = "/home/debian/AntiUSBProxy/config/" + this->cfg->get("Device") + "/config" + std::to_string(activeConfigIndex) + "/iface" + std::to_string(packet.wIndex) + "/";
	std::string deviceHIDReport = hidReportFolder + "hidReport";

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
