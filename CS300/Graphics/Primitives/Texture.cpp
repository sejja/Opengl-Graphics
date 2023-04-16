//
//	Texture.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include "Texture.h"
#include <cstdlib>
#include "stb.h"
#include "../../Core/Allocator.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

namespace Graphics {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a Texture Class
	*/ // --------------------------------------------------------------------
	Texture::Texture()
		: Width(0), Height(0), mPixels(nullptr), Handle(NULL) {}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a Texture Class given a texture filename
	*/ // --------------------------------------------------------------------
	Texture::Texture(const std::string_view& filename)
		: Width(0), Height(0), mPixels(nullptr), Handle(NULL) {
		LoadFromFile(filename);
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys a Texture Class
	*/ // --------------------------------------------------------------------
	Texture::~Texture() {
		//If we have had a valid handle
		if (Handle) {
			glBindTexture(GL_TEXTURE_2D, Handle);
			glDeleteTextures(1, &Handle);
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
			unsigned char* tempPixels_ = stbi_load(filename.data(), &x_, &y_, &n_,
				reqComp_);

			//If we could load the image
			if (tempPixels_) {
				Width = static_cast<size_t>(x_);
				Height = static_cast<size_t>(y_);
				n_ = static_cast<int>(Width * Height * reqComp_);
				mPixels = Allocator<unsigned char>::allocate(n_);
				memcpy(mPixels, tempPixels_, n_);
				stbi_image_free(tempPixels_);
				glGenTextures(1, &Handle);
				glBindTexture(GL_TEXTURE_2D, Handle);
				Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					static_cast<GLsizei>(Width),
					static_cast<GLsizei>(Height), 0,
					GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<void*>(mPixels));
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				Allocator<unsigned char>::deallocate(mPixels);
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Bind
	*
	*   Binds the Texture to the OpenGL render pipeline
	*/ // --------------------------------------------------------------------
	void Texture::Bind() const {
#ifdef _DEBUG
		//If we have a valid handle
		if (Handle)
#endif
			glBindTexture(GL_TEXTURE_2D, Handle);
	}
}