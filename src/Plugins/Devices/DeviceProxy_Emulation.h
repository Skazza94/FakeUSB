/*
 * This file is not part of original USBProxy.
 * This DeviceProxy doesn't actually do nothing. It doesn't communicate with a real device (like libUSB one).
 * What we do is redirect all requests and packets to the Attack class, which knows how to parse them.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_DEVICEPROXYEMULATION_H
#define USBPROXY_DEVICEPROXYEMULATION_H

#include <string.h>
#include <list>

#include "Criteria.h"
#include "DeviceProxy.h"

class DeviceProxy_Emulation: public DeviceProxy {
private:
	bool connected = false;
	std::map<__u8, std::list<__u8 *> *> * endPoint2packetBuffer;

public:
	DeviceProxy_Emulation(ConfigParser *);
	~DeviceProxy_Emulation();

	int connect(int timeout = 250);
	void disconnect();
	void reset();
	bool is_connected();

	int control_request(const usb_ctrlrequest *, int *, __u8 *, int timeout = 500);
	void send_data(__u8, __u8, __u16, __u8 *, int);
	void receive_data(__u8, __u8, __u16, __u8 **, int *, int timeout = 500);
	void setConfig(Configuration *, Configuration *, bool);
	bool is_highspeed();
	void set_endpoint_interface(__u8, __u8);
	void claim_interface(__u8);
	void release_interface(__u8);

	std::list<__u8 *> * getPacketBufferForEndpoint(__u8);
	void setPacketBufferForEndpoint(std::list<__u8 *> *, __u8);

	__u8 get_address();
	char* toString() {return (char*)"Emulated Device";}
};

#endif /* USBPROXY_DEVICEPROXYEMULATION_H */
