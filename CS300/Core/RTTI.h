//
//	RTTI.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 24/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _RTTI__H_
#define _RTTI__H_

#include "CommonDefines.h"

namespace RTTI {
	// ------------------------------------------------------------------------
	/*! Is A
	*
	*   Is "other" a <<type>>
	*/ // ---------------------------------------------------------------------
	template<typename TYPE, typename OTHERTYPE>
	DONTDISCARD constexpr inline bool IsA(OTHERTYPE* other) noexcept {
		return dynamic_cast<TYPE*>(other);
	}
}

#endif