/*
 * EmulatedDevice.h
 *
 *  Created on: 04 nov 2016
 *      Author: skazza
 */

#ifndef USBPROXY_EMULATEDDEVICE_H
#define USBPROXY_EMULATEDDEVICE_H

#include <stdio.h>

#include "Device.h"

class EmulatedDevice : public Device {
private:
	DeviceProxy * deviceProxy;

public:
	EmulatedDevice(DeviceProxy *);
};



#endif /* USBPROXY_EMULATEDDEVICE_H */
