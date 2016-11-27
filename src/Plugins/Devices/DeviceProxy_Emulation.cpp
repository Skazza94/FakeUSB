/*
 * This file is not part of original USBProxy.
 * This DeviceProxy doesn't actually do nothing. It doesn't communicate with a real device (like libUSB one).
 * What we do is redirect all requests and packets to the Attack class, which knows how to parse them.
 *
 * Author: Skazza
 */

#include "DeviceProxy_Emulation.h"

DeviceProxy_Emulation::DeviceProxy_Emulation(ConfigParser * cfg) {
	this->cfg = cfg;
	this->endPoint2packetBuffer = new std::map<__u8, std::list<std::pair<__u8 *, __u64>> *>;
}

DeviceProxy_Emulation::~DeviceProxy_Emulation() {
	delete(this->endPoint2packetBuffer);
}

int DeviceProxy_Emulation::connect(int timeout) {
	connected = true;
	return 0;
}

void DeviceProxy_Emulation::disconnect() {
	connected = false;
}

void DeviceProxy_Emulation::reset() {
	return;
}

bool DeviceProxy_Emulation::is_connected() {
	return connected;
}

int DeviceProxy_Emulation::control_request(const usb_ctrlrequest * setup_packet, int * nbytes, __u8 * dataptr, int timeout) {
	return this->attack->parseSetupRequest(*setup_packet, nbytes, dataptr);
}

void DeviceProxy_Emulation::send_data(__u8 endpoint, __u8 attributes, __u16 maxPacketSize, __u8 * dataptr, __u64 length) {
	__u8 inEp = this->attack->getInEpForOutEp(endpoint);

	if(inEp != 0xff) { /* A valid IN endpoint (where send data) has been found */
		std::list<std::pair<__u8 *, __u64>> * packetBuffer = this->getPacketBufferForEndpoint(inEp);
		this->attack->parseDeviceRequest(maxPacketSize, dataptr, length, &packetBuffer);

		this->setPacketBufferForEndpoint(packetBuffer, inEp);
	}
}

void DeviceProxy_Emulation::receive_data(__u8 endpoint, __u8 attributes, __u16 maxPacketSize, __u8 ** dataptr, __u64 * length, int timeout) {
	*length = 0;

	if(!this->attack->canStartAttack())
		return;

	std::list<std::pair<__u8 *, __u64>> * packetBuffer = this->getPacketBufferForEndpoint(endpoint);

	if(packetBuffer->empty())
		this->attack->getNextPayload(&packetBuffer, endpoint, maxPacketSize);

	if(!packetBuffer->empty()) {
		std::pair<__u8 *, __u64> dataAndLength = packetBuffer->front();

		(*dataptr) = (__u8 *) calloc(dataAndLength.second, sizeof(__u8));
		memcpy((*dataptr), dataAndLength.first, dataAndLength.second);

		*length = dataAndLength.second;
		packetBuffer->pop_front(); /* Popped after to avoid memcpy strange behaviours */
	}

	this->setPacketBufferForEndpoint(packetBuffer, endpoint);
}

void DeviceProxy_Emulation::setConfig(Configuration * fs_cfg, Configuration * hs_cfg, bool hs) {
	/* Clean buffers on configuration change */
	this->endPoint2packetBuffer->clear();

	/* Reload the attack on configuration change */
	this->attack->startAttack();
}

bool DeviceProxy_Emulation::is_highspeed() {
	return false;
}

void DeviceProxy_Emulation::set_endpoint_interface(__u8 endpoint, __u8 interface) {
	return;
}

void DeviceProxy_Emulation::claim_interface(__u8 interface) {
	return;
}

void DeviceProxy_Emulation::release_interface(__u8 interface) {
	return;
}

__u8 DeviceProxy_Emulation::get_address() {
	return 0;
}

std::list<std::pair<__u8 *, __u64>> * DeviceProxy_Emulation::getPacketBufferForEndpoint(__u8 endpoint) {
	std::map<__u8, std::list<std::pair<__u8 *, __u64>> *>::iterator it = this->endPoint2packetBuffer->find(endpoint);
	return (it != this->endPoint2packetBuffer->end()) ? (*it).second : new std::list<std::pair<__u8 *, __u64>>;
}

void DeviceProxy_Emulation::setPacketBufferForEndpoint(std::list<std::pair<__u8 *, __u64>> * packetBuffer, __u8 endpoint) {
	std::map<__u8, std::list<std::pair<__u8 *, __u64>> *>::iterator it = this->endPoint2packetBuffer->find(endpoint);

	if(it != this->endPoint2packetBuffer->end())
		it->second = packetBuffer;
	else
		this->endPoint2packetBuffer->insert(std::pair<__u8, std::list<std::pair<__u8 *, __u64>> *>(endpoint, packetBuffer));
}

static DeviceProxy_Emulation * proxy;

extern "C" {
	DeviceProxy * get_deviceproxy_plugin(ConfigParser * cfg) {
		proxy = new DeviceProxy_Emulation(cfg);
		return (DeviceProxy *) proxy;
	}

	void destroy_plugin() {
		delete proxy;
	}
}
