#pragma once

#include <glm/glm.hpp>
#include "Core/Pipeline.h"
#include <glew.h>
#include <tinyobjloader.h>
#include <iostream>
#include "Graphics/Primitives/Model.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"

namespace Core {
	struct Vertex {
		glm::vec3 mPosition;
		glm::vec2 mUVs;
	};
	
	class Renderable {
	public:
		virtual void Render() = 0;
		glm::vec3 pos, scale, rot;
	};

	template<GraphicsAPIS E>
	class ModelRenderer : public Renderable {
	};

	template<>
	class ModelRenderer<GraphicsAPIS::OpenGL> : public Renderable {
	public:
		void SetMesh(AssetReference<Model> model) {
			mModel = model.lock();
		}

		void SetShaderProgram(const AssetReference<Graphics::ShaderProgram>& s) {
			mShaderProgram = s.lock();
		}
		
		void Render() override {
			mModel->Get()->set_uniforms(*mShaderProgram->Get());
			glBindVertexArray(mModel->Get()->GetVao());
			glDrawArrays(GL_TRIANGLES, 0, mModel->Get()->GetVertexCount());
		}
	private:
		Asset<Model> mModel;
		Asset<Graphics::ShaderProgram> mShaderProgram;
	};
}