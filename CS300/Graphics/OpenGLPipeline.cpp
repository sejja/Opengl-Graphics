//
//	OpenGLPipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright © 2023. All Rights reserved
//

#include <execution>
#include "OpenGLPipeline.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/InputManager.h"
#include "Graphics/Primitives/Light.h"
#include "Global.h"
#include "Graphics/Primitives/Texture.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Init
		*
		*   Sets the Initial OpenGL Settings to Draw
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Init() {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glClearColor(0.f, 0.f, 0.f, 0.f);
		}

		// ------------------------------------------------------------------------
		/*! PreRender
		*
		*   Clears the whole FrameBuffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::PreRender() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			static Primitives::Camera cam;

			//If we want to see the Wireframe
			if (Singleton<InputManager>::Instance().IsKeyDown('Z'))
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT, GL_FILL);

			glm::mat4 view = cam.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
			std::unordered_multimap<Asset<::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> obsoletes;

			std::for_each(std::execution::unseq, mRenderables.begin(), mRenderables.end(), [this, &obsoletes, &projection, &view](const std::pair<Asset<::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>>& it) {
				::Graphics::ShaderProgram* shader = it.first->Get();

				//If we want to see the UV Channels, toogle to them
				if (Singleton<InputManager>::Instance().IsKeyDown('V')) {
					shader = Singleton<ResourceManager>::Instance().GetResource<::Graphics::ShaderProgram>("Content/Shaders/UVs.shader")->Get();
					shader->Bind();
				} else {
					shader->Bind();
					shader->SetShaderUniform("uCameraPos", &cam.mPosition);
					UploadLightDataToGPU(it.first);
				}

				shader->SetShaderUniform("uTransform", &projection);
				shader->SetShaderUniform("uView", &view);

				//For each renderable in shader program
				for (std::vector<std::weak_ptr<Core::Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
					//If it isn't expired
					if (auto renderable = it2->lock()) {
						const std::shared_ptr<Object> parent = renderable->mParent.lock();
						glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->transform.mPostion) *
							glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.x, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::scale(glm::mat4(1.0f), parent->transform.mScale);
						shader->SetShaderUniform("uModel", &matrix);
						reinterpret_cast<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
					} else {
						obsoletes.insert(std::make_pair(it.first, it2));
					}
				}
			});
			
			std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this](std::pair<const Asset<::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> x) {
				std::vector<std::weak_ptr<Core::Renderable>>& it = mRenderables.find(x.first)->second;
				it.erase(x.second);
			
				//If we don't have any other renderables, erase it
				if (!it.size()) mRenderables.erase(x.first);
				});
		}

		// ------------------------------------------------------------------------
		/*! Upload Light Data to GPU
		*
		*   Uploads the light data to the shader
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::UploadLightDataToGPU(const AssetReference<::Graphics::ShaderProgram>& shader) {
			::Graphics::ShaderProgram* shadptr = shader.lock()->Get();

			for (size_t i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				const std::string id = "uLight[" + std::to_string(i);
			
				shadptr->SetShaderUniform((id + "].pos").c_str(), &::Graphics::Primitives::Light::sLightData[i].mPosition);
				shadptr->SetShaderUniform((id + "].dir").c_str(), &::Graphics::Primitives::Light::sLightData[i].mDirection);
				shadptr->SetShaderUniform((id + "].amb").c_str(), &::Graphics::Primitives::Light::sLightData[i].mAmbient);
				shadptr->SetShaderUniform((id + "].dif").c_str(), &::Graphics::Primitives::Light::sLightData[i].mDiffuse);
				shadptr->SetShaderUniform((id + "].spe").c_str(), &::Graphics::Primitives::Light::sLightData[i].mSpecular);
				shadptr->SetShaderUniform((id + "].att").c_str(), &::Graphics::Primitives::Light::sLightData[i].mAttenuation);
				shadptr->SetShaderUniform((id + "].cosIn").c_str(), &::Graphics::Primitives::Light::sLightData[i].mInner);
				shadptr->SetShaderUniform((id + "].cosOut").c_str(), &::Graphics::Primitives::Light::sLightData[i].mOutter);
				shadptr->SetShaderUniform((id + "].fallOff").c_str(), &::Graphics::Primitives::Light::sLightData[i].mFallOff);
				shadptr->SetShaderUniform((id + "].type").c_str(), static_cast<int>(::Graphics::Primitives::Light::sLightData[i].mType));
			}

			shadptr->SetShaderUniform("uLightCount", static_cast<int>(::Graphics::Primitives::Light::sLightReg));
		}
	}
}