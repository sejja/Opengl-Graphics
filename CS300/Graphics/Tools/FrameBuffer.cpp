//
//	FrameBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/07/23
//	Copyright © 2023. All Rights reserved
//

#include "FrameBuffer.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs a FrameBuffer of said dimensions
		*/ //----------------------------------------------------------------------
		FrameBuffer::FrameBuffer() {
			
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Releases the FrameBuffer resources from the GPU
		*/ //----------------------------------------------------------------------
		FrameBuffer::~FrameBuffer() {
			glDeleteTextures(1, &mTexture);
			glDeleteBuffers(1, &mHandle);
		}
		
		void FrameBuffer::Create() {
			glGenFramebuffers(1, &mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Create Render Texture
		*
		*   Allocates a new rendertarget associated with this render texture
		*/ //----------------------------------------------------------------------
		void FrameBuffer::CreateRenderTexture(glm::lowp_u16vec2 dimensions, bool readable) {
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);

			//If it's not readable
			if (!readable) {
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			Unbind();

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw FrameBufferException("FrameBuffer is Incomplete");
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Sets this FrameBuffer as the current target for drawing operations
		*/ //----------------------------------------------------------------------
		void FrameBuffer::Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Unbind
		*
		*   Unbinds this buffer
		*/ //----------------------------------------------------------------------
		void FrameBuffer::Unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		}

		// ------------------------------------------------------------------------
		/*! Bind Texture
		*
		*   Binds the framebuffer render texture
		*/ //----------------------------------------------------------------------
		void FrameBuffer::BindTexture() {
			glBindTexture(GL_TEXTURE_2D, mTexture);
		}
		
		// ------------------------------------------------------------------------
		/*! Clear
		*
		*   Clears the FrameBuffer memory
		*/ //----------------------------------------------------------------------
		void FrameBuffer::Clear(bool depthOnly) {
			if(depthOnly) glClear(GL_DEPTH_BUFFER_BIT);
			else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}