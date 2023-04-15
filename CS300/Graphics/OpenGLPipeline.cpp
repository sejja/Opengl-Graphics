#include "OpenGLPipeline.h"
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Primitives/Renderables.h"
#include "Graphics/Camera.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/InputManager.h"
#include "Graphics/Primitives/Light.h"

static Graphics::Texture texture("Content/Textures/UV.jpg");

void OpenGLPipeline::Init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	texture.UploadToGPU();
}

void OpenGLPipeline::PreRender() {
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

void OpenGLPipeline::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto uv = GContent->GetResource<Graphics::ShaderProgram>("Content/Shaders/UVs.shader")->Get();
	static Camera cam;
					
	if (GInput->IsKeyDown('Z')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	if (GInput->IsKeyDown('F')) {
		uv->Bind();
	}
	else {
		texture.Bind();
	}

	auto shader  = GContent->GetResource<Graphics::ShaderProgram>("Content/Shaders/UVs.shader")->Get();

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 1.0f, -20.0f));
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	cam.Update();
	std::vector<std::vector<std::weak_ptr<Core::Renderable>>::iterator> obsoletes;
	
	for (auto it = renderables.begin(); it != renderables.end(); ++it) {
		if (auto renderable = it->lock()) {
			glm::mat4 view = glm::mat4(1.0f);

			auto parent = renderable->mParent.lock();
			
			auto pos = parent->transform.mPostion;
			// note that we're translating the scene in the reverse direction of where we want to move
			view = cam.GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
			Asset<Graphics::ShaderProgram> shader = std::dynamic_pointer_cast<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(renderable)->GetShaderProgram().lock();
			shader->Get()->Bind();
			shader->Get()->SetShaderUniform("uTransform", &projection);
			shader->Get()->SetShaderUniform("uView", &view);
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->transform.mPostion)*
				glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.y, glm::vec3(1.0f, 0.0f, 0.0f))*
				glm::rotate(glm::mat4(1.0f), parent->transform.mRotation.x, glm::vec3(0.0f, 1.0f, 0.0f))*
				glm::scale(glm::mat4(1.0f), parent->transform.mScale);
			shader->Get()->SetShaderUniform("uModel", &matrix);		
			shader->Get()->SetShaderUniform("uCameraPos", &cam.mPosition);
		
			UploadLightDataToGPU(shader);
			
			
			auto r = reinterpret_cast<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get());
			r->Render();
		}
		else {
			obsoletes.push_back(it);
		}
	}

	for (auto& x : obsoletes) {
		renderables.erase(x);
	}
}

void OpenGLPipeline::PostRender() {
}

void OpenGLPipeline::Shutdown() {
}

void OpenGLPipeline::SetDimensions(const glm::lowp_u16vec2& dim) {
	glViewport(0, 0, dim.x, dim.y);
}

void OpenGLPipeline::AddRenderable(const std::weak_ptr<Core::Renderable>& renderer) {
	renderables.push_back(renderer);
}

void OpenGLPipeline::UploadLightDataToGPU(const AssetReference<Graphics::ShaderProgram>& shader) {
	auto shadptr = shader.lock()->Get();
	
	for (size_t i = 1; i <= Graphics::Primitives::Light::sLightReg; i++) {
		auto pos = Graphics::Primitives::Light::sLightData[i - 1].mPosition;
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].pos").c_str(), &pos);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].dir").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mDirection);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].amb").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mAmbient);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].dif").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mDiffuse);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].spe").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mSpecular);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].att").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mAttenuation);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].cosIn").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mInner);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].cosOut").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mOutter);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].fallOff").c_str(), &Graphics::Primitives::Light::sLightData[i - 1].mFallOff);
		shadptr->SetShaderUniform(("uLight[" + std::to_string(i - 1) + "].type").c_str(), static_cast<int>(Graphics::Primitives::Light::sLightData[i - 1].mType));
	}

	shadptr->SetShaderUniform("uLightCount", static_cast<int>(Graphics::Primitives::Light::sLightReg));
}
