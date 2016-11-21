/*
 * Utility functions to convert ASCII characters of IT keyboard and Mouse Buttons into USB Bytes.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_KEYMAP_H
#define USBPROXY_KEYMAP_H

#include <map>
#include <string>

#include "Criteria.h"

std::pair<__u8, __u8> findCharacter(char);
std::pair<__u8, __u8> findKey(std::string);
__u8 findButton(char button);

#endif /* USBPROXY_KEYMAP_H */
