#ifndef __BASE_FACTORY_H
#define __BASE_FACTORY_H

#include "Attack.h"
#include <string>
#include <map>

class Attack;

typedef Attack * (*factoryMethod)();

class AttackFactory {
private:
	static std::map<std::string, factoryMethod> registeredClasses;

public:
	static bool registerClass(const std::string &name, factoryMethod createMethod);
	static Attack * createInstance(const std::string &name);
};

#endif
