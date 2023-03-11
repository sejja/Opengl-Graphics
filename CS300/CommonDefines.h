//
//	CommonDefines.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _COMMONDEFINES__H_
#define _COMMONDEFINES__H_

#include <memory>
#include <functional>

#define DONTDISCARD	[[nodiscard]]

namespace std {
	template<typename T>
	using deleted_pointer = std::unique_ptr<T, void(*)(T*)>;
}

#endif