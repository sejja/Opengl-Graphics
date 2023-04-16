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
	ShaderProgram::ShaderProgram(AssetReference<Shader> vertexShader, AssetReference<Shader> fragmentShader)
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
			glGetProgramiv(Handle, GL_INFO_LOG_LENGTH, &status);

			if (status > 0) {
				char* log = reinterpret_cast<char*>(malloc(status));
				GLsizei written;

				glGetProgramInfoLog(Handle, status, &written, log);
				MessageBoxA(NULL, log, "Shader Linking Error",
					MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
				free(log);
			}
		}
#endif

		DetachShader(static_cast<Shader::EType>(2));
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
	/*! Unbind
	*
	*   Unbinds the Shader Program
	*/ // --------------------------------------------------------------------
	void ShaderProgram::Unbind() {
		glUseProgram(NULL);
	}

	// ------------------------------------------------------------------------
	/*! Load
	*
	*   Loads a Shader Program
	*/ // --------------------------------------------------------------------
	void ShaderProgram::Load() {
		//If we have a vertex shader
		if (Shaders[0]) {
			Shaders[0]->Get()->LoadSource();
			Shaders[0]->Get()->Compile("vertex shader");
		}

		//If we have a fragment shader
		if (Shaders[1]) {
			Shaders[1]->Get()->LoadSource();
			Shaders[1]->Get()->Compile("fragment shader");
		}

		Link();
	}

	// ------------------------------------------------------------------------
	/*! Get Shader
	*
	*   Gets a Shader from our Shader Program
	*/ // --------------------------------------------------------------------
	AssetReference<Shader> ShaderProgram::GetShader(Shader::EType shaderType) {
		return Shaders[static_cast<unsigned char>(shaderType)];
	}

	// ------------------------------------------------------------------------
	/*! Set Shader
	*
	*   Sets a shader in our program
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShader(AssetReference<Shader>pShader, bool link) {
		//If we have a valid pointer to our shader
		if (!pShader.expired()) {
		
			Asset<Shader> p = pShader.lock();

			DetachShader(p->Get()->ShaderType);
			Shaders[static_cast<unsigned char>(p->Get()->ShaderType)] = std::move(p);

			//If we are requested to link it
			if (link)
				Link();
		}
	}

	void ShaderProgram::SetShaderUniform(const char* name, int value) {
		int loc = glGetUniformLocation(mHandle, name);

		if (loc >= 0) {
			glUniform1i(loc, value);
		}
	}

	void ShaderProgram::SetShaderUniform(const char* name, bool value) {
		int loc = glGetUniformLocation(mHandle, name);

		if (loc >= 0) {
			glUniform1i(loc, value);
		}
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
	/*! Detach Shader
	*
	*   Detach one of the shaders is this program
	*/ // --------------------------------------------------------------------
	void ShaderProgram::DetachShader(Shader::EType shaderType) {
		// specific shadertype ->set if necessary
		if (static_cast<unsigned char>(shaderType) != 2)
			if (Shaders[static_cast<unsigned char>(shaderType)]) {
				glDetachShader(mHandle, Shaders[static_cast<unsigned char>(shaderType)]->Get()->mHandle);
			}
			else {
				if (Shaders[0])
					glDetachShader(mHandle, Shaders[0]->Get()->mHandle);

				if (Shaders[1])
					glDetachShader(mHandle, Shaders[1]->Get()->mHandle);
			}
	}

	// ------------------------------------------------------------------------
	/*! Get Open GL Handler
	*
	*   Gets the OpenGL Handle of this program
	*/ // --------------------------------------------------------------------
	unsigned int ShaderProgram::GetOpenGLHandle() {
		return mHandle;
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of integers
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, int* value, int count) {
		glUniform1iv(glGetUniformLocation(mHandle, name), count, value);
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of floats
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, float* value, int count) {
		glUniform1fv(glGetUniformLocation(mHandle, name), count, value);
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of Matrices 4x4
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, glm::mat4* value, int count) {
#ifdef _DEBUG
		int loc = glGetUniformLocation(Handle, name);

		//If we could retrieve the uniform location
		if (loc != -1)
			glUniformMatrix4fv(loc, count, GL_FALSE, reinterpret_cast<float*>(value));
#else
		glUniformMatrix4fv(glGetUniformLocation(mHandle, name), count, GL_FALSE,
			reinterpret_cast<float*>(value));
#endif
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of matrices 3x3
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, glm::mat3* value, int count) {
		glUniformMatrix4fv(glGetUniformLocation(mHandle, name), count, GL_FALSE,
			reinterpret_cast<float*>(value));
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of Vectors of size 2
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, glm::vec2* value, int count) {
		glUniform2fv(glGetUniformLocation(mHandle, name), count,
			reinterpret_cast<float*>(value));
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of Vectors of size 3
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, glm::vec3* value, int count) {
		glUniform3fv(glGetUniformLocation(mHandle, name), count,
			reinterpret_cast<float*>(value));
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of Vectors of size 4
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, glm::vec4* value, int count) {
		int loc = glGetUniformLocation(mHandle, name);
		glUniform4fv(loc, count,
			reinterpret_cast<float*>(value));
	}

	// ------------------------------------------------------------------------
	/*! Set Shader Uniform
	*
	*   Sets uniform values in form of a Color
	*/ // --------------------------------------------------------------------
	void ShaderProgram::SetShaderUniform(const char* name, Color* value, int count) {
		glUniform4fv(glGetUniformLocation(mHandle, name), count,
			reinterpret_cast<float*>(value));
	}
}