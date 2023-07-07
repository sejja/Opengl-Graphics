//
//	Model.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _MODEL__H_
#define _MODEL__H_

#include <string>
#include <glew.h>
#include <vector>
#include <tinyobjloader.h>
#include "Material.h"
#include "ShaderProgram.h"

namespace Core {
	namespace Graphics {
		class Model {
		public:
			struct ModelVertex {
				tinyobj::real_t pos[3];
				tinyobj::real_t normal[3];
				tinyobj::real_t uv[2];
				tinyobj::real_t tangent[3];
				tinyobj::real_t bitangent[3];
			};

			~Model() noexcept;
			void Clear() noexcept;
			void LoadFromFile(const std::string_view& inputfile);
			DONTDISCARD GLuint inline GetHandle() const noexcept;
			DONTDISCARD size_t inline GetVertexCount() const noexcept;
			void SetUniforms(ShaderProgram& s) const;

		private:
			void UploadToGPU(std::vector<float>& vertices, std::vector<int>& indexes);
			std::size_t mCount;
			GLuint mVAO, mVBO, mIBO;
			::Graphics::Material mMaterial;
		};

		// ------------------------------------------------------------------------
		/*! Get Handle
		*
		*   Returns the Vertex Array Object assigned by OpenGL
		*/ //----------------------------------------------------------------------
		GLuint Model::GetHandle() const noexcept {
			return mVAO;
		}

		// ------------------------------------------------------------------------
		/*! Get Vertex Count
		*
		*   Returns how many vertex does the model have
		*/ //----------------------------------------------------------------------
		std::size_t Model::GetVertexCount() const noexcept {
			return mCount;
		}
	}
}


#endif