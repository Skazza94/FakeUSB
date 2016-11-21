/*
 * This file is not part of original USBProxy.
 * Emulates a real Device, loading the config file obtained pasting in a sequential file all the USB structs.
 *
 * Author: Skazza
 */

#include "EmulatedDevice.h"

EmulatedDevice::EmulatedDevice(DeviceProxy * proxy) {
	this->proxy = proxy;
	this->proxy->setDevice(this);

	/* Get the device descriptor, configurations, interfaces, endpoints from a config file */
	std::ostringstream deviceConfiguration; deviceConfiguration << "/home/debian/AntiUSBProxy/config/DeviceConfig/" << this->proxy->cfg->get("Device") << "Config";

	FILE * configFileHandler = fopen(deviceConfiguration.str().c_str(), "rb");

	if(configFileHandler) {
		fread(&descriptor, sizeof(descriptor), 1, configFileHandler);

		configurations = (Configuration **) calloc(descriptor.bNumConfigurations, sizeof(*configurations));

		maxStringIdx = (descriptor.iManufacturer > maxStringIdx) ? descriptor.iManufacturer : maxStringIdx;
		maxStringIdx = (descriptor.iProduct > maxStringIdx) ? descriptor.iProduct : maxStringIdx;
		maxStringIdx = (descriptor.iSerialNumber > maxStringIdx) ? descriptor.iSerialNumber : maxStringIdx;
		strings = (USBString ***) calloc(maxStringIdx + 1, sizeof(*strings));

		for(__u8 i = 0; i < descriptor.bNumConfigurations; i++) {
			usb_config_descriptor * configDescriptor = (usb_config_descriptor *) malloc(sizeof(usb_config_descriptor));
			fread(configDescriptor, sizeof(*configDescriptor), 1, configFileHandler);

			configurations[i] = new Configuration(this, configDescriptor);

			/* Load each interface of the device */
			for (__u8 j = 0; j < configurations[i]->get_descriptor()->bNumInterfaces; ++j) {
				usb_interface_descriptor * iFaceDesc = (usb_interface_descriptor *) malloc(sizeof(usb_interface_descriptor));
				fread(iFaceDesc, sizeof(*iFaceDesc), 1, configFileHandler);

				Interface * iface = new Interface(configurations[i], iFaceDesc);

				__u8 hidDescCount = 0;
				__u8 genericDescCount = 0;

				fread(&hidDescCount, sizeof(hidDescCount), 1, configFileHandler);
				fread(&genericDescCount, sizeof(genericDescCount), 1, configFileHandler);

				if(hidDescCount) {
					usb_hid_descriptor hidDesc;
					fread(&hidDesc, sizeof(hidDesc), 1, configFileHandler);

					HID * hid = new HID(&hidDesc);
					iface->set_hid_descriptor(hid);
				}

				if(genericDescCount) iface->set_generic_descriptor_count(genericDescCount);

				__u8 k;
				for(k = 0; k < genericDescCount; ++k) {
					/* Read length */
					__u8 length = 0;
					fread(&length, sizeof(length), 1, configFileHandler);

					/* Read descriptor type */
					__u8 descriptorType = 0;
					fread(&descriptorType, sizeof(descriptorType), 1, configFileHandler);
					/* Calculate real length */
					__u8 realLength = length - (2 * sizeof(__u8));

					GenericDescriptor * genericDesc = (GenericDescriptor *) calloc(realLength, 2);
					genericDesc->bLength = length;
					genericDesc->bDescriptorType = descriptorType;

					fread(&genericDesc->bData, realLength, 1, configFileHandler);

					iface->add_generic_descriptor(genericDesc);
				}

				for(k = 0; k < iface->get_descriptor()->bNumEndpoints; ++k) {
					usb_endpoint_descriptor * epDesc = (usb_endpoint_descriptor *) malloc(USB_DT_ENDPOINT_SIZE);
					fread(epDesc, USB_DT_ENDPOINT_SIZE, 1, configFileHandler);

					Endpoint * ep = new Endpoint(iface, epDesc);
					iface->add_endpoint(ep);
				}

				configurations[i]->add_interface(iface);
			}
		}

		deviceConfigurationIndex = 1;
		deviceState = USB_STATE_CONFIGURED;

		/* First string is language configuration */
		USBString * setupString = new USBString(readStringDescriptorFromFile(configFileHandler), 0, 0);
		add_string(setupString);

		if (descriptor.iManufacturer)
			this->addStringFromFile(configFileHandler, descriptor.iManufacturer);

		if (descriptor.iProduct)
			this->addStringFromFile(configFileHandler, descriptor.iProduct);

		if (descriptor.iSerialNumber)
			this->addStringFromFile(configFileHandler, descriptor.iSerialNumber);

		highspeed = true;
		qualifier = NULL;

		fclose(configFileHandler);
	}
}

usb_string_descriptor * EmulatedDevice::readStringDescriptorFromFile(FILE * handler) {
	/* Read the struct length */
	__u8 structLength = 0;
	fread(&structLength, sizeof(structLength), 1, handler);

	/* Read the descriptor type */
	__u8 descriptorType = 0;
	fread(&descriptorType, sizeof(descriptorType), 1, handler);

	/* structLength - 2 * size of __u8 because the structLength value also has the size of structLength (__u8) and descriptorType (__u8);
	 * so we remove them. */
	__u8 realLength = structLength - (2 * sizeof(__u8));

	usb_string_descriptor * stringDescArray = (usb_string_descriptor *) calloc(realLength, 2);

	/* Init first two fields that we've already read. */
	stringDescArray->bLength = structLength;
	stringDescArray->bDescriptorType = descriptorType;

	/* Read the values to put into the array */
	fread(&stringDescArray->wData, realLength, 1, handler);

	return stringDescArray;
}

void EmulatedDevice::addStringFromFile(FILE * handler, __u8 index) {
	const usb_string_descriptor * p = strings[0][0]->get_descriptor();
	usb_string_descriptor * stringRead = readStringDescriptorFromFile(handler);

	/* Add the read string in every language available */
	for(__u8 k = 0; k < p->bLength - (2 * sizeof(__u8)); ++k) {
		USBString * usbString = new USBString(stringRead, index, p->wData[k]);
		add_string(usbString);
	}
}
