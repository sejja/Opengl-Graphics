#pragma once

#include "Core/ECSystem/Object.h"
#include "Core/ECSystem/Component.h"
#include "Scene.h"
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

	class Renderable : public Component {
	public:
		Renderable(std::weak_ptr<Object> parent) : Component(parent) {}
		virtual void Render() = 0;
	};

	template<GraphicsAPIS E>
	class ModelRenderer : public Renderable {
	};

	template<>
	class ModelRenderer<GraphicsAPIS::OpenGL> : public Renderable {
	public:
		ModelRenderer(std::weak_ptr<Object> parent): Renderable(parent) {}
		
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

		AssetReference<Graphics::ShaderProgram> GetShaderProgram() {
			return mShaderProgram;
		}

	private:
		Asset<Model> mModel;
		Asset<Graphics::ShaderProgram> mShaderProgram;
	};
}