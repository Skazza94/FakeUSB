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

	/* Get the device descriptor, configurations, interfaces, endpoints from config files */
	std::string deviceConfigurationDir = "/home/debian/AntiUSBProxy/config/" + this->proxy->cfg->get("Device");
	std::string fileName = deviceConfigurationDir + "/device";

	FILE * fileHandler = fopen(fileName.c_str(), "rb");

	if(fileHandler) {
		fread(&descriptor, sizeof(descriptor), 1, fileHandler);

		configurations = (Configuration **) calloc(descriptor.bNumConfigurations, sizeof(*configurations));

		fclose(fileHandler);

		maxStringIdx = (descriptor.iManufacturer > maxStringIdx) ? descriptor.iManufacturer : maxStringIdx;
		maxStringIdx = (descriptor.iProduct > maxStringIdx) ? descriptor.iProduct : maxStringIdx;
		maxStringIdx = (descriptor.iSerialNumber > maxStringIdx) ? descriptor.iSerialNumber : maxStringIdx;
		strings = (USBString ***) calloc(maxStringIdx + 1, sizeof(*strings));

		for(__u8 i = 0; i < descriptor.bNumConfigurations; i++) {
			fileName = deviceConfigurationDir + "/config" + std::to_string(i) + "/config";
			fileHandler = fopen(fileName.c_str(), "rb");

			usb_config_descriptor * configDescriptor = (usb_config_descriptor *) malloc(sizeof(usb_config_descriptor));
			fread(configDescriptor, sizeof(*configDescriptor), 1, fileHandler);

			fclose(fileHandler);

			configurations[i] = new Configuration(this, configDescriptor);

			/* Load each interface of the device */
			for (__u8 j = 0; j < configurations[i]->get_descriptor()->bNumInterfaces; ++j) {
				fileName = deviceConfigurationDir + "/config" + std::to_string(i) + "/iface" + std::to_string(j) + "/iface";
				fileHandler = fopen(fileName.c_str(), "rb");

				usb_interface_descriptor * iFaceDesc = (usb_interface_descriptor *) malloc(sizeof(usb_interface_descriptor));
				fread(iFaceDesc, sizeof(*iFaceDesc), 1, fileHandler);

				Interface * iface = new Interface(configurations[i], iFaceDesc);

				__u8 hidDescCount = 0;
				__u8 genericDescCount = 0;

				fread(&hidDescCount, sizeof(hidDescCount), 1, fileHandler);
				fread(&genericDescCount, sizeof(genericDescCount), 1, fileHandler);

				fclose(fileHandler);

				if(hidDescCount) {
					fileName = deviceConfigurationDir + "/config" + std::to_string(i) + "/iface" + std::to_string(j) + "/hidDesc";
					fileHandler = fopen(fileName.c_str(), "rb");

					usb_hid_descriptor hidDesc;
					fread(&hidDesc, sizeof(hidDesc), 1, fileHandler);

					HID * hid = new HID(&hidDesc);
					iface->set_hid_descriptor(hid);

					fclose(fileHandler);
				}

				if(genericDescCount) iface->set_generic_descriptor_count(genericDescCount);

				__u8 k;
				for(k = 0; k < genericDescCount; ++k) {
					fileName = deviceConfigurationDir + "/config" + std::to_string(i) + "/iface" + std::to_string(j) + "/genDesc" + std::to_string(k);
					fileHandler = fopen(fileName.c_str(), "rb");

					/* Read length */
					__u8 length = 0;
					fread(&length, sizeof(length), 1, fileHandler);

					/* Read descriptor type */
					__u8 descriptorType = 0;
					fread(&descriptorType, sizeof(descriptorType), 1, fileHandler);
					/* Calculate real length */
					__u8 realLength = length - (2 * sizeof(__u8));

					GenericDescriptor * genericDesc = (GenericDescriptor *) calloc(realLength, 2);
					genericDesc->bLength = length;
					genericDesc->bDescriptorType = descriptorType;

					fread(&genericDesc->bData, realLength, 1, fileHandler);

					iface->add_generic_descriptor(genericDesc);

					fclose(fileHandler);
				}

				for(k = 0; k < iface->get_descriptor()->bNumEndpoints; ++k) {
					fileName = deviceConfigurationDir + "/config" + std::to_string(i) + "/iface" + std::to_string(j) + "/ep" + std::to_string(k);
					fileHandler = fopen(fileName.c_str(), "rb");

					usb_endpoint_descriptor * epDesc = (usb_endpoint_descriptor *) malloc(USB_DT_ENDPOINT_SIZE);
					fread(epDesc, USB_DT_ENDPOINT_SIZE, 1, fileHandler);

					Endpoint * ep = new Endpoint(iface, epDesc);
					iface->add_endpoint(ep);

					fclose(fileHandler);
				}

				configurations[i]->add_interface(iface);
			}
		}

		deviceConfigurationIndex = 1;
		deviceState = USB_STATE_CONFIGURED;

		/* First string is language configuration */
		fileName = deviceConfigurationDir + "/stringConf";
		fileHandler = fopen(fileName.c_str(), "rb");

		USBString * setupString = new USBString(readStringDescriptorFromFile(fileHandler), 0, 0);
		add_string(setupString);

		fclose(fileHandler);

		if (descriptor.iManufacturer) {
			fileName = deviceConfigurationDir + "/manufacturer";
			fileHandler = fopen(fileName.c_str(), "rb");
			this->addStringFromFile(fileHandler, descriptor.iManufacturer);

			fclose(fileHandler);
		}

		if (descriptor.iProduct) {
			fileName = deviceConfigurationDir + "/product";
			fileHandler = fopen(fileName.c_str(), "rb");
			this->addStringFromFile(fileHandler, descriptor.iProduct);

			fclose(fileHandler);
		}

		if (descriptor.iSerialNumber) {
			fileName = deviceConfigurationDir + "/serialNumber";
			fileHandler = fopen(fileName.c_str(), "rb");
			this->addStringFromFile(fileHandler, descriptor.iSerialNumber);

			fclose(fileHandler);
		}

		highspeed = false;
		qualifier = NULL;
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
