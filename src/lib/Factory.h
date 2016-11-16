/*
 * This file is not part of original USBProxy.
 * Implements Abstract Factory pattern.
 * Each "T" sub-class autoregisters itself into this Factory, passing a string to identify it and the pointer to a static
 * function defined into the sub-class that returns new instances of that class. Everything is stored into a map.
 * When we need a new instance, we pass the string, search for the "creator" function associated and call it.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_FACTORY_H
#define USBPROXY_FACTORY_H

#include <stdio.h>
#include <utility>
#include <string>
#include <map>

#include "Singleton.h"

/* Defines the "creator" function pointer type */
template<typename T>
	typedef T * (*ctor)();

template<typename T>
	class Factory : public Singleton<Factory> {
	private:
		std::map<std::string, ctor> registeredClasses = {};

	public:
		bool registerClass(const std::string &name, ctor createMethod);
		T * createInstance(const std::string &name);
	};

#endif /* USBPROXY_FACTORY_H */
