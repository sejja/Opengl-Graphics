//
//	ShaderProgram.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021. All Rights reserved
//

#ifndef _SHADER_PROGRAM__H_
#define _SHADER_PROGRAM__H_

#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Color.h"
#include "CommonDefines.h"
#include "Core/ResourceManager.h"

namespace Core {
	namespace Graphics {
		class ShaderProgram {
		public:
			ShaderProgram();
			ShaderProgram(const AssetReference<Shader>& vertexShader, const AssetReference<Shader>& fragmentShader);
			void Bind();
			AssetReference<Shader> GetShader(const Shader::EType shaderType) const;
			void SetShader(const AssetReference<Shader>& pShader, const bool link = false);
			void SetShaderUniform(const std::string_view& name, const int value);
			void SetShaderUniform(const std::string_view& name, const bool value);
			void SetShaderUniform(const std::string_view& name, int* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, float* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, glm::mat4* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, glm::mat3* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, glm::vec2* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, glm::vec3* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, glm::vec4* value, const int count = 1);
			void SetShaderUniform(const std::string_view& name, Color* value, const int count = 1);
		private:
			void Link();
			void AttachShader(const Shader::EType which);
			GLuint getUniformLocatiion(const std::string_view& id);

			std::unordered_map<std::string_view, GLuint> mUniformLocations;
			unsigned int mHandle = NULL;
			Asset<Shader> Shaders[2];
		};
	}
}

#endif
