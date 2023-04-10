//
//	ShaderProgram.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _SHADER_PROGRAM__H_
#define _SHADER_PROGRAM__H_

#include "Shader.h"
#include <glm/glm.hpp>
#include "Color.h"
#include "CommonDefines.h"
#include <string>
#include "Core/ResourceManager.h"

namespace Graphics {
	class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(AssetReference<Shader> vertexShader, AssetReference<Shader> fragmentShader);
		void Bind();
		void Unbind();
		void Load();
		AssetReference<Shader> GetShader(Shader::EType shaderType);
		void SetShader(AssetReference<Shader> pShader, bool link = false);
		void SetShaderUniform(const char* name, int value);
		void SetShaderUniform(const char* name, bool value);
		void SetShaderUniform(const char* name, int* value, int count = 1);
		void SetShaderUniform(const char* name, float* value, int count = 1);
		void SetShaderUniform(const char* name, glm::mat4* value, int count = 1);
		void SetShaderUniform(const char* name, glm::mat3* value, int count = 1);
		void SetShaderUniform(const char* name, glm::vec2* value, int count = 1);
		void SetShaderUniform(const char* name, glm::vec3* value, int count = 1);
		void SetShaderUniform(const char* name, glm::vec4* value, int count = 1);
		void SetShaderUniform(const char* name, Color* value, int count = 1);
		DONTDISCARD unsigned int GetOpenGLHandle();

	private:
		void Link();
		void AttachShader(const Shader::EType which);
		void DetachShader(const Shader::EType which);

		unsigned int Handle = NULL;
		Asset<Shader> Shaders[2];
	};
}

#endif
