 /*
 * Sub-class of Attack. Adds few specific descriptor type fields + callbacks for HIDs.
 * We also have a string that stores the device type which we're emulating. This is used in sub-classes.
 *
 * Author: dibbidouble & Skazza
 */

#include "AttackHID.h"

AttackHID::AttackHID(Device * device) : Attack(device) {
	this->setupType2Callback.insert(
		std::pair<__u8, std::function<__u8(const usb_ctrlrequest, __u8 *)>>(
			0x22, std::bind(&AttackHID::getHIDReportDescriptor, this, std::placeholders::_1, std::placeholders::_2)
		)
	);
}

AttackHID::~AttackHID() {}

__u8 AttackHID::getHIDReportDescriptor(const usb_ctrlrequest packet, __u8 * dataPtr) {
	std::ostringstream deviceHIDReport; deviceHIDReport << "/home/debian/AntiUSBProxy/config/HIDReport/" << this->device_type << "HIDReport";

	FILE * hidReportFileHandler = fopen(deviceHIDReport.str().c_str(), "rb");

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
