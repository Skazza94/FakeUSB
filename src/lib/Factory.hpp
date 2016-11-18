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

#include "Singleton.hpp"

template<typename T>
	class Factory : public Singleton<Factory<T>> {
	protected:
		std::map<std::string, T * (*)()> * registeredClasses = NULL;

	public:
		Factory() {
			this->registeredClasses = new std::map<std::string, T * (*)()>;
		}

		bool registerClass(const std::string &name, T * (*createMethod)()) {
			this->registeredClasses->insert(std::pair<std::string, T * (*)()>(name, createMethod));

			return true;
		}

		T * createInstance(const std::string &name) {
			auto it = this->registeredClasses->find(name);

			if(it != this->registeredClasses->end())
				return (*it).second();

			return NULL;
		}
	};

#endif /* USBPROXY_FACTORY_H */
