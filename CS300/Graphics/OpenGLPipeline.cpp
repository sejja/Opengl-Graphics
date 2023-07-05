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
		#if 1
			mShadowBuffer.Create();
			mShadowBuffer.CreateRenderTexture(mDimensions, false);
		#endif
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

			glm::vec3 up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[0].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[0].mPosition));

			float near_plane = 0.1f, far_plane = 10;
			glm::mat4 lightProjection = glm::perspective(glm::radians(40.f * 2.0f), 1.0f, 0.1f, 200.f);
			glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[0].mPosition,
				glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

			{

				#if 1
				// 1. first render to depth map
				mShadowBuffer.Bind();
				glViewport(0, 0, mDimensions.x, mDimensions.y);
				glClearDepth(1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(2.f, 4.f);
				
				{
					//If we want to see the Wireframe
					if (Singleton<InputManager>::Instance().IsKeyDown('Z'))
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					else
						glPolygonMode(GL_FRONT, GL_FILL);

					glm::mat4 view = cam.GetViewMatrix();
					glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
					std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> obsoletes;

					std::for_each(std::execution::unseq, mRenderables.begin(), mRenderables.end(), [this, &obsoletes, &lightProjection, &lightView](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>>& it) {
						auto shadow = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
						shadow->Bind();

						shadow->SetShaderUniform("uTransform", &lightProjection);
						shadow->SetShaderUniform("uView", &lightView);

						//For each renderable in shader program
						for (std::vector<std::weak_ptr<Core::Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
							//If it isn't expired
							if (auto renderable = it2->lock()) {
								const std::shared_ptr<Object> parent = renderable->mParent.lock();
								glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->transform.mPostion) *
									glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
									glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.x, glm::vec3(0.0f, 1.0f, 0.0f)) *
									glm::scale(glm::mat4(1.0f), parent->transform.mScale);
								shadow->SetShaderUniform("uModel", &matrix);
								reinterpret_cast<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
							}
							else {
								obsoletes.insert(std::make_pair(it.first, it2));
							}
						}
						});

					std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> x) {
						std::vector<std::weak_ptr<Core::Renderable>>& it = mRenderables.find(x.first)->second;
						it.erase(x.second);

						//If we don't have any other renderables, erase it
						if (!it.size()) mRenderables.erase(x.first);
						});
				}

				glDisable(GL_POLYGON_OFFSET_FILL);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				// 2. then render scene as normal with shadow mapping (using depth map)
				glViewport(0, 0, mDimensions.x, mDimensions.y);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glActiveTexture(GL_TEXTURE2);
				mShadowBuffer.BindTexture();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				#endif
				
				{
					//If we want to see the Wireframe
					if (Singleton<InputManager>::Instance().IsKeyDown('Z'))
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					else
						glPolygonMode(GL_FRONT, GL_FILL);

					glm::mat4 biasMatrix(
						0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0
					);

					glm::mat4 view = cam.GetViewMatrix();
					glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
					glm::mat4 shadow_matrix =  lightProjection * lightView;
					std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> obsoletes;

					std::for_each(std::execution::unseq, mRenderables.begin(), mRenderables.end(), [this, &shadow_matrix, &obsoletes, &projection, &view](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>>& it) {
						Core::Graphics::ShaderProgram* shader = it.first->Get();

						//If we want to see the UV Channels, toogle to them
						if (Singleton<InputManager>::Instance().IsKeyDown('V')) {
							shader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/UVs.shader")->Get();
							shader->Bind();
						}
						else {
							shader->Bind();
							shader->SetShaderUniform("uCameraPos", &cam.GetPositionRef());
							UploadLightDataToGPU(it.first);
							auto tex = Singleton<ResourceManager>::Instance().GetResource<::Graphics::Texture>("Content/Textures/UV.jpg")->Get();
							tex->SetTextureType(::Graphics::Texture::TextureType::eDiffuse);
							tex->Bind();
							auto normals = Singleton<ResourceManager>::Instance().GetResource<::Graphics::Texture>("Content/Textures/BrickNormal.png")->Get();
							normals->SetTextureType(::Graphics::Texture::TextureType::eNormal);
							normals->Bind();
						}

						shader->SetShaderUniform("uTransform", &projection);
						shader->SetShaderUniform("uView", &view);
						shader->SetShaderUniform("uShadowMatrix", &shadow_matrix);

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
							}
							else {
								obsoletes.insert(std::make_pair(it.first, it2));
							}
						}
						});

					std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Core::Renderable>>::const_iterator> x) {
						std::vector<std::weak_ptr<Core::Renderable>>& it = mRenderables.find(x.first)->second;
						it.erase(x.second);

						//If we don't have any other renderables, erase it
						if (!it.size()) mRenderables.erase(x.first);
						});
				}
			}
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