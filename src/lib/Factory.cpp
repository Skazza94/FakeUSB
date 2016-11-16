/*
 * This file is not part of original USBProxy.
 * Implements Abstract Factory pattern.
 * Each "T" sub-class autoregisters itself into this Factory, passing a string to identify it and the pointer to a static
 * function defined into the sub-class that returns new instances of that class. Everything is stored into a map.
 * When we need a new instance, we pass the string, search for the "creator" function associated and call it.
 *
 * Author: Skazza
 */

#include "Factory.h"

bool Factory::registerClass(const std::string &name, ctor createMethod) {
  	std::pair<std::map<std::string, ctor>::iterator, bool> registeredPair = this->registeredClasses.insert(std::make_pair(name, createMethod));
	return registeredPair.second;
}

template<typename T>
	T * Factory::createInstance(const std::string &name) {
		std::map<std::string, ctor>::iterator it = this->registeredClasses.find(name);
		if(it != this->registeredClasses.end())
			return (*it).second();

		return NULL;
	}
