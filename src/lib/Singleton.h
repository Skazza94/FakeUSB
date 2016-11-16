/*
 * This file is not part of original USBProxy.
 * A beautiful, simple and template based Singleton pattern class.
 *
 * Author: Skazza
 */

#ifndef USBPROXY_SINGLETON_H
#define USBPROXY_SINGLETON_H

#include <stdio.h>

template<typename T>
	class Singleton {
	protected:
		static T * instance;

	public:
		Singleton() {
			instance = static_cast<T *>(this);
		}

		~Singleton() {}

		static T * getInstance() {
			return instance;
		}
	};

template<typename T>
	T * Singleton<T>::instance = new T();

#endif /* USBPROXY_SINGLETON_H */
