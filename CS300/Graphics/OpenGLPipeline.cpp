//
//	OpenGLPipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright � 2023. All Rights reserved
//

#include <execution>
#include "OpenGLPipeline.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/InputManager.h"
#include "Graphics/Primitives/Light.h"
#include "Core/Singleton.h"
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
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers[0].Create();
			mShadowBuffers[0].CreateRenderTexture({mDimensions.x * 2, mDimensions.y * 2}, false);
			mShadowBuffers[1].Create();
			mShadowBuffers[1].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mShadowBuffers[2].Create();
			mShadowBuffers[2].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mShadowBuffers[3].Create();
			mShadowBuffers[3].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mShadowBuffers[4].Create();
			mShadowBuffers[4].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
		}

		// ------------------------------------------------------------------------
		/*! PreRender
		*
		*   Clears the whole FrameBuffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::PreRender() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		void renderQuad() {
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			static Primitives::Camera cam;
			std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes;

			auto f_flushobosoletes = [this , &obsoletes]() {
				std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> x) {
					std::vector<std::weak_ptr<Renderable>>& it = mGroupedRenderables.find(x.first)->second;
					it.erase(x.second);

					//If we don't have any other renderables, erase it
					if (!it.size()) mGroupedRenderables.erase(x.first);
					});
			};

			auto f_grouprender = [&obsoletes](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it, ShaderProgram * shader) {
				//For each renderable in shader program
				for (std::vector<std::weak_ptr<Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
					//If it isn't expired
					if (auto renderable = it2->lock()) {
						const std::shared_ptr<Object> parent = renderable->mParent.lock();
						glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->transform.mPostion) *
							glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.x, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::scale(glm::mat4(1.0f), parent->transform.mScale);
						shader->SetShaderUniform("uModel", &matrix);
						reinterpret_cast<ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
					}
					else {
						obsoletes.insert(std::make_pair(it.first, it2));
					}
				}
			};

			glCullFace(GL_FRONT);
			for(int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mShadowBuffers[i].Bind();
				mShadowBuffers[i].Clear(true);

				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 1.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0.0, -15, 50), glm::vec3(0, 1, 0));

				{
					const auto shadow = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
					shadow->Bind();
					shadow->SetShaderUniform("uTransform", &lightProjection);
					shadow->SetShaderUniform("uView", &lightView);

					std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), [this, &shadow, &obsoletes, &f_grouprender](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
						f_grouprender(it, shadow);
						});

					f_flushobosoletes();
				}

				mShadowBuffers[i].Unbind();

			}

			glCullFace(GL_BACK);
			glViewport(0, 0, mDimensions.x, mDimensions.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			std::vector<glm::mat4> shadow_matrices;

			for(int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mShadowBuffers[i].BindTexture(2 + i);
				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 1.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0.0, -15, 50), glm::vec3(0, 1, 0));
				glm::mat4 shadow_matrix = lightProjection * lightView;
				shadow_matrices.push_back(shadow_matrix);
			}

			#if 1

			{
				glm::mat4 view = cam.GetViewMatrix();
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), [this, &shadow_matrices, &obsoletes, &projection, &view, &f_grouprender](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
					Core::Graphics::ShaderProgram* shader = it.first->Get();

					shader->Bind();
					
					shader->SetShaderUniform("uCameraPos", &cam.GetPositionRef());
					shader->SetShaderUniform("uTransform", &projection);
					shader->SetShaderUniform("uView", &view);

					for(int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
						shader->SetShaderUniform("uShadowMatrix[" + std::to_string(i) + "]", shadow_matrices.data() + i);
					}

					
					UploadLightDataToGPU(it.first);
					auto tex = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/UV.jpg")->Get();
					tex->SetTextureType(Texture::TextureType::eDiffuse);
					tex->Bind();
					auto normals = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/BrickNormal.png")->Get();
					normals->SetTextureType(Texture::TextureType::eNormal);
					normals->Bind();
					f_grouprender(it, shader);

					});

				f_flushobosoletes();
			}

			#else

			static auto debugdepth = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DebugDepth.shader")->Get();
			debugdepth->Bind();
			renderQuad();
		
			#endif
		}

		// ------------------------------------------------------------------------
		/*! Upload Light Data to GPU
		*
		*   Uploads the light data to the shader
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::UploadLightDataToGPU(const AssetReference<Core::Graphics::ShaderProgram>& shader) {
			Core::Graphics::ShaderProgram* shadptr = shader.lock()->Get();

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

		// ------------------------------------------------------------------------
		/*! Set Dimension
		*
		*   Sets the Viewport Size on OpenGL
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::SetDimensions(const glm::lowp_u16vec2& dim) {
			glViewport(0, 0, dim.x, dim.y);
			mDimensions = dim;
		}
	}
}