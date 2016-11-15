/*
 * This file is not part of original USBProxy.
 * Emulates a real Device, loading the config file obtained pasting in a sequential file all the USB structs.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_EMULATEDDEVICE_H
#define USBPROXY_EMULATEDDEVICE_H

#include <stdio.h>
#include <sstream>
#include <string.h>

#include "Device.h"
#include "DeviceProxy.h"
#include "Configuration.h"
#include "Interface.h"
#include "HID.h"
#include "Endpoint.h"
#include "USBString.h"

#include "HexString.h"

class Configuration;
class Interface;
class HID;
class Endpoint;
class USBString;

class EmulatedDevice : public Device {
private:
	usb_string_descriptor * readStringDescriptorFromFile(FILE *);
	void addStringFromFile(FILE *, __u8);

public:
	EmulatedDevice(DeviceProxy *);
};



#endif /* USBPROXY_EMULATEDDEVICE_H */
