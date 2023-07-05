//
//	Texture.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#include "Texture.h"
#include <cstdlib>
#include "stb.h"
#include "../../Core/Allocator.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

namespace Graphics {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a Texture Class
	*/ // --------------------------------------------------------------------
	Texture::Texture()
		: mWidth(0), mHeight(0), mHandle(NULL), mType(TextureType::eDiffuse) {}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a Texture Class given a texture filename
	*/ // --------------------------------------------------------------------
	Texture::Texture(const std::string_view& filename)
		: mWidth(0), mHeight(0), mHandle(NULL) {
		LoadFromFile(filename);
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys a Texture Class
	*/ // --------------------------------------------------------------------
	Texture::~Texture() {
		//If we have had a valid handle
		if (mHandle) {
			glBindTexture(GL_TEXTURE_2D, mHandle);
			glDeleteTextures(1, &mHandle);
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}

	// ------------------------------------------------------------------------
	/*! Load from file
	*
	*   Loads a image from a file
	*/ // --------------------------------------------------------------------
	void Texture::LoadFromFile(const std::string_view& filename) {
		//If we passed a valid filename
		if (!filename.empty()) {
			stbi_set_flip_vertically_on_load(true);

			int x_, y_, n_, reqComp_ = 4;
			unsigned char* const tempPixels_ = stbi_load(filename.data(), &x_, &y_, &n_, reqComp_);

			//If we could load the image
			if (tempPixels_) {
				mWidth = static_cast<size_t>(x_);
				mHeight = static_cast<size_t>(y_);
				n_ = static_cast<int>(mWidth * mHeight * reqComp_);
				glGenTextures(1, &mHandle);
				glBindTexture(GL_TEXTURE_2D, mHandle);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(mWidth),
					static_cast<GLsizei>(mHeight), 0,
					GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const void*>(tempPixels_));
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(tempPixels_);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Bind
	*
	*   Binds the Texture to the OpenGL render pipeline
	*/ // --------------------------------------------------------------------
	void Texture::Bind() const {
		switch (mType) {
		default:
			glActiveTexture(GL_TEXTURE0);
			break;

		case TextureType::eNormal:
			glActiveTexture(GL_TEXTURE1);
			break;
		}

		glBindTexture(GL_TEXTURE_2D, mHandle);
	}
}