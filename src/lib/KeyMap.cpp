/*
 * Utility functions to convert ASCII characters of IT keyboard and Mouse Buttons into USB Bytes.
 *
 * Author: Skazza
 */

#include "KeyMap.h"

std::map<char, __u8> mouseButton2Byte {
	{'L', 0x01},
	{'R', 0x02},
	{'M', 0x04}
};

std::map<char, __u8> ascii2Byte {
	{'a', 0x04},
	{'b', 0x05},
	{'c', 0x06},
	{'d', 0x07},
	{'e', 0x08},
	{'f', 0x09},
	{'g', 0x0a},
	{'h', 0x0b},
	{'i', 0x0c},
	{'j', 0x0d},
	{'k', 0x0e},
	{'l', 0x0f},
	{'m', 0x10},
	{'n', 0x11},
	{'o', 0x12},
	{'p', 0x13},
	{'q', 0x14},
	{'r', 0x15},
	{'s', 0x16},
	{'t', 0x17},
	{'u', 0x18},
	{'v', 0x19},
	{'w', 0x1a},
	{'x', 0x1b},
	{'y', 0x1c},
	{'z', 0x1d},
	{'1', 0x1e},
	{'2', 0x1f},
	{'3', 0x20},
	{'4', 0x21},
	{'5', 0x22},
	{'6', 0x23},
	{'7', 0x24},
	{'8', 0x25},
	{'9', 0x26},
	{'0', 0x27},
	{' ', 0x2c},
	{'<', 0x64},
	{'è', 0x2f},
	{'+', 0x30},
	{'ù', 0x32},
	{'ò', 0x33},
	{'à', 0x34},
	{',', 0x36},
	{'.', 0x37},
	{'-', 0x38},
	{'\'', 0x2d},
	{'ì', 0x2e},
	{'\\', 0x35},
};

std::map<char, __u8> ascii2ShiftByte {
	{'A', 0x04},
	{'B', 0x05},
	{'C', 0x06},
	{'D', 0x07},
	{'E', 0x08},
	{'F', 0x09},
	{'G', 0x0a},
	{'H', 0x0b},
	{'I', 0x0c},
	{'J', 0x0d},
	{'K', 0x0e},
	{'L', 0x0f},
	{'M', 0x10},
	{'N', 0x11},
	{'O', 0x12},
	{'P', 0x13},
	{'Q', 0x14},
	{'R', 0x15},
	{'S', 0x16},
	{'T', 0x17},
	{'U', 0x18},
	{'V', 0x19},
	{'W', 0x1a},
	{'X', 0x1b},
	{'Y', 0x1c},
	{'Z', 0x1d},
	{'>', 0x64},
	{'é', 0x2f},
	{'*', 0x30},
	{'§', 0x32},
	{'ç', 0x33},
	{'°', 0x34},
	{';', 0x36},
	{':', 0x37},
	{'_', 0x38},
	{'\?', 0x2d},
	{'^', 0x2e},
	{'|', 0x35},
	{'!', 0x1e},
	{'\"', 0x1f},
	{'£', 0x20},
	{'$', 0x21},
	{'%', 0x22},
	{'&', 0x23},
	{'/', 0x24},
	{'(', 0x25},
	{')', 0x26},
	{'=', 0x27},
};

std::map<char, __u8> ascii2AltGRByte {
	{L'€', 0x08},
	{'[', 0x2f},
	{']', 0x30},
	{'@', 0x33},
	{'#', 0x34},
	{'`', 0x2d},
	{'~', 0x2e},
};

std::map<char, __u8> ascii2AltGRShiftByte {
	{'{', 0x2f},
	{'}', 0x30}
};

std::map<std::string, __u8> key2USBByte {
	{"F1", 0x3a},
	{"F2", 0x3b},
	{"F3", 0x3c},
	{"F4", 0x3d},
	{"F5", 0x3e},
	{"F6", 0x3f},
	{"F7", 0x40},
	{"F8", 0x41},
	{"F9", 0x42},
	{"F10", 0x43},
	{"F11", 0x44},
	{"F12", 0x45},
	{"STAMP", 0x46},
	{"SLOCK", 0x47},
	{"PAUSE", 0x48},
	{"INS", 0x49},
	{"HOME", 0x4a},
	{"PGUP", 0x4b},
	{"DEL", 0x4c},
	{"END", 0x4d},
	{"PGDOWN", 0x4e},
	{"NUMLOCK", 0x53},
	{"ENTER", 0x28},
	{"BKSPACE", 0x2a},
	{"ESC", 0x29},
	{"TAB", 0x2b},
	{"CAPSL", 0x39},
	{"CONTEXTUAL", 0x65}
};

std::map<std::string, __u8> mod2USBByte {
	{"WIN", 0x08},
	{"CTRL", 0x01},
	{"SHIFT", 0x02},
	{"ALT", 0x04},
	{"ALTGR", 0x40}
};

std::pair<__u8, __u8> findCharacter(char character) {
	std::pair<__u8, __u8> firstAndThirdByte;
	firstAndThirdByte.first = firstAndThirdByte.second = 0x00;

	std::map<char, __u8>::iterator it = ascii2Byte.find(character);

	if(it == ascii2Byte.end()) {
		it = ascii2ShiftByte.find(character);

		if(it != ascii2ShiftByte.end())
			firstAndThirdByte.first = 0x02;
		else {
			it = ascii2AltGRByte.find(character);

			if(it != ascii2AltGRByte.end())
				firstAndThirdByte.first = 0x40;
			else {
				it = ascii2AltGRShiftByte.find(character);

				if(it != ascii2AltGRShiftByte.end())
					firstAndThirdByte.first = 0x60;
			}
		}
	}

	firstAndThirdByte.second = (*it).second;

	return firstAndThirdByte;
}

std::pair<__u8, __u8> findKey(std::string key) {
	std::pair<__u8, __u8> firstAndThirdByte;
	firstAndThirdByte.first = firstAndThirdByte.second = 0x00;

	std::map<std::string, __u8>::iterator it = key2USBByte.find(key);

	if(it == key2USBByte.end()) {
		it = mod2USBByte.find(key);

		if(it != mod2USBByte.end())
			firstAndThirdByte.first = (*it).second;
	} else {
		firstAndThirdByte.second = (*it).second;
	}

	return firstAndThirdByte;
}

__u8 findButton(char button) {
	std::map<char, __u8>::iterator it = mouseButton2Byte.find(button);

	if(it != mouseButton2Byte.end())
		return (*it).second;

	return 0x00;
}
