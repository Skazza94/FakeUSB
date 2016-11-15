/*
 * AttackHID.h
 *
 *  Created on: 15 nov 2016
 *      Author: dibbidouble
 */

#ifndef SRC_LIB_ATTACKHID_H_
#define SRC_LIB_ATTACKHID_H_

#include "Attack.h"
//#include "DeviceProxy.h"

#include <sstream>
#include <sys/stat.h>
#include <string>

class AttackHID : public Attack{
protected:
	std::string device_type;
//	DeviceProxy* device_proxy;
public:
	AttackHID(Device*);
	virtual ~AttackHID();
	__u8 get_HIDreport(const usb_ctrlrequest, __u8*);
};

#endif /* SRC_LIB_ATTACKHID_H_ */
