//
//	Texture.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#ifndef _TEXTURE__H_
#define _TEXTURE__H_

#include "../../CommonDefines.h"
#include "../../Dependencies/GLEW/Include/glew.h"
#include <string>

namespace Graphics {
	class Texture {
	public:
		enum TextureType {
			eDiffuse,
			eNormal,
			eSpecular
		};

		Texture();
		Texture(const std::string_view& filename);
		~Texture();
		void LoadFromFile(const std::string_view& filename);
		inline void SetTextureType(const TextureType type);
		void Bind() const;
		
	private:
		size_t mWidth;
		size_t mHeight;
		GLuint mHandle;
		TextureType mType;
	};

	// ------------------------------------------------------------------------
	/*! Set Texture Type
	*
	*   Sets the Type of a certain texture
	*/ // --------------------------------------------------------------------
	void Texture::SetTextureType(const TextureType type) {
		mType = type;
	}
}

#endif
