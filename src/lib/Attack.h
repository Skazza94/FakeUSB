#ifndef SRC_LIB_ATTACK_H_
#define SRC_LIB_ATTACK_H_

#include "Device.h"
#include "HexString.h"
#include "USBString.h"

#include <linux/usb/ch9.h>
#include <map>
#include <functional>
#include <string.h>

class Attack {
protected:
	Device * device = NULL;
	std::map<__u8, std::function<__u8(const usb_ctrlrequest, __u8*)>> configuration_packets;

public:
	Attack(Device*);
	virtual ~Attack();
	int control_request(const usb_ctrlrequest, int *, __u8 *);
	__u8 get_string(const usb_ctrlrequest, __u8*);
};

#endif /* SRC_LIB_ATTACK_H_ */
