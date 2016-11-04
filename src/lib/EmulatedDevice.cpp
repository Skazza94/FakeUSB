/*
 * EmulatedDevice.cpp
 *
 *  Created on: 04 nov 2016
 *      Author: skazza
 */

#include "EmulatedDevice.h"

EmulatedDevice::EmulatedDevice(DeviceProxy * proxy) {
	this->deviceProxy = proxy;
	this->deviceProxy->setDevice(this);

	/* Get the device descriptor, configurations, interfaces, endpoints from a config file */
	fprintf(stderr, "Sto in EmulatedDevice!\n");
}
