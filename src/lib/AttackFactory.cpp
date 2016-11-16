#include "AttackFactory.h"

#include <utility>
#include <cstdio>

bool AttackFactory::registerClass(const std::string &name, factoryMethod createMethod) {
  	std::pair<std::map<std::string, factoryMethod>::iterator, bool> registeredPair = AttackFactory::registeredClasses.insert(std::make_pair(name.c_str(), createMethod));
	return registeredPair.second;
}

Attack * AttackFactory::createInstance(const std::string &name) {
  	std::map<std::string, factoryMethod>::iterator registeredPair = AttackFactory::registeredClasses.find(name);
	if(registeredPair != AttackFactory::registeredClasses.end())
		return registeredPair->second();

	return NULL;
}

// initialise the registered names map
std::map<std::string, factoryMethod> AttackFactory::registeredClasses = { };
