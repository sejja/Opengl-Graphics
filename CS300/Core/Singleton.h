//
//	Singleton.h
//	Good Neighbours
//
//	Created by Diego Revilla 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SINGLETON__H_
#define _SINGLETON__H_

template<typename T>
class Global {
public:
	static T& Instance();

	Global(const Global&) = delete;
	Global& operator= (const Global) = delete;

protected:
	Global() {}
};

template<typename T>
T& Global<T>::Instance() {
	static T instance;
	return instance;
}

#endif