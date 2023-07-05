//
//	OpenGLInfo.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/07/24
//	Copyright © 2023. All Rights reserved
//

#ifndef _OPENGL_INFO__H_
#define _OPENGL_INFO__H_

#include <glew.h>

namespace Core {
	namespace Graphics {
		struct OpenGLInfo {
		#pragma region //Constructor
			OpenGLInfo() noexcept;
		#pragma endregion

		#pragma region //Data
			GLuint mBindedBuffer;
		#pragma endregion
		};
	}
}

#endif