//
//	ShaderProgram.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <glew.h>
#include <Windows.h>
#include <fstream>
#include "Core/ResourceManager.h"
#include "ShaderProgram.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Shader Program Class
		*/ // --------------------------------------------------------------------
		ShaderProgram::ShaderProgram() :
			mHandle(glCreateProgram()) {}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader Program class given a Vertex and Fragment Shader
		*/ // --------------------------------------------------------------------
		ShaderProgram::ShaderProgram(const AssetReference<Shader>& vertexShader, const AssetReference<Shader>& fragmentShader)
			: ShaderProgram() {

			//If the shaders are not expired
			if (!vertexShader.expired() || !fragmentShader.expired()) {
				Shaders[0] = vertexShader.lock();
				Shaders[1] = fragmentShader.lock();
				Link();
			}
		}

		// ------------------------------------------------------------------------
		/*! Link
		*
		*   Links both Vertex and Fragment Shaders to our Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Link() {
			//If we have a Vertex Shader
			if (Shaders[0])
				glAttachShader(mHandle, Shaders[0]->Get()->mHandle);

			//If we have a Fragment Shader
			if (Shaders[1])
				glAttachShader(mHandle, Shaders[1]->Get()->mHandle);

			GLint status;

			glLinkProgram(mHandle);
			glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

#ifdef _DEBUG
			if (status == GL_FALSE) {
				glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &status);

				if (status > 0) {
					char* log = reinterpret_cast<char*>(malloc(status));
					GLsizei written;

					glGetProgramInfoLog(mHandle, status, &written, log);
					MessageBoxA(NULL, log, "Shader Linking Error",
						MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
					free(log);
				}
			}
#endif
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the Shader Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Bind() {
			glUseProgram(mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Get Shader
		*
		*   Gets a Shader from our Shader Program
		*/ // --------------------------------------------------------------------
		AssetReference<Shader> ShaderProgram::GetShader(const Shader::EType shaderType) const {
			return Shaders[static_cast<unsigned char>(shaderType)];
		}

		void ShaderProgram::SetShaderUniform(const std::string_view& name, const int value) {
			int loc = glGetUniformLocation(mHandle, name.data());

#ifdef _DEBUG_ 
			if (loc >= 0)
#endif
				glUniform1i(loc, value);
		}

		void ShaderProgram::SetShaderUniform(const std::string_view& name, const bool value) {
			int loc = glGetUniformLocation(mHandle, name.data());

#ifdef _DEBUG_ 
			if (loc >= 0)
#endif
				glUniform1i(loc, value);
		}

		// ------------------------------------------------------------------------
		/*! Attach Shader
		*
		*   Attach one of the shaders in this program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::AttachShader(Shader::EType shaderType) {
			//If we have a valid shader
			if (Shaders[static_cast<unsigned char>(shaderType)])
				glAttachShader(mHandle, Shaders[
					static_cast<unsigned char>(shaderType)]->Get()->mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Get Uniform Location
		*
		*  Get the location of an Uniform within a shader
		*/ // --------------------------------------------------------------------
		GLuint ShaderProgram::getUniformLocatiion(const std::string_view& id) {
			const std::unordered_map<std::string_view, GLuint>::local_iterator idx = mUniformLocations.find(id);

			return idx == mUniformLocations.end() ? (mUniformLocations[id] = glGetUniformLocation(mHandle, id.data())) : idx->second;
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of integers
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, int* value, const int count) {
			glUniform1iv(getUniformLocatiion(name), count, value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of floats
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, float* value, const int count) {
			glUniform1fv(getUniformLocatiion(name), count, value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Matrices 4x4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::mat4* value, const int count) {
#ifdef _DEBUG
			int loc = getUniformLocatiion(name);

			//If we could retrieve the uniform location
			if (loc != -1)
				glUniformMatrix4fv(loc, count, GL_FALSE, reinterpret_cast<float*>(value));
#else
			glUniformMatrix4fv(getUniformLocatiion(name), count, GL_FALSE,
				reinterpret_cast<float*>(value));
#endif
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of matrices 3x3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::mat3* value, const int count) {
			glUniformMatrix4fv(getUniformLocatiion(name), count, GL_FALSE,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 2
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec2* value, const int count) {
			glUniform2fv(getUniformLocatiion(name), count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec3* value, const int count) {
			glUniform3fv(getUniformLocatiion(name), count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec4* value, const int count) {
			int loc = getUniformLocatiion(name);
			glUniform4fv(loc, count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of a Color
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, Color* value, const int count) {
			glUniform4fv(getUniformLocatiion(name), count,
				reinterpret_cast<float*>(value));
		}
	}
}