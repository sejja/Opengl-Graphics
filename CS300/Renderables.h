#pragma once

#include <glm/glm.hpp>
#include "Core/Pipeline.h"
#include <glew.h>
#include <tinyobjloader.h>
#include <iostream>
#include "Graphics/Primitives/Model.h"
#include "Core/ResourceManager.h"

namespace Core {
	struct Vertex {
		glm::vec3 mPosition;
		glm::vec2 mUVs;
	};
	
	class Renderable {
	public:
		virtual void Render() = 0;
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
		
		void Render() override {
			glBindVertexArray(mModel->Get()->mVAO);
			glDrawArrays(GL_TRIANGLES, 0, mModel->Get()->mCount);
		}

	private:
		Asset<Model> mModel;
	};
}