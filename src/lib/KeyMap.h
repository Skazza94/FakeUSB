/*
 * KeyMap.h
 *
 *  Created on: 21 nov 2016
 *      Author: skazza
 */

#ifndef USBPROXY_KEYMAP_H
#define USBPROXY_KEYMAP_H

#include "Criteria.h"
#include <map>
#include <string>

std::pair<__u8, __u8> findCharacter(char);
std::pair<__u8, __u8> findKey(std::string);
__u8 findButton(char button);

#endif /* USBPROXY_KEYMAP_H */
