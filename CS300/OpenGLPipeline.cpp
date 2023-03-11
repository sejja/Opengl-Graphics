#include "OpenGLPipeline.h"
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Primitives/ShaderProgram.h"
#include "Renderables.h"
#include "Graphics/Camera.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/InputManager.h"

static Graphics::Texture texture("Content/Textures/UV.jpg");
Graphics::ShaderProgram* uv_program;

void OpenGLPipeline::Init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	texture.UploadToGPU();
	mProjectionMatrix = glm::perspective(1.f, 0.75f, 1.f, 100.f);
	uv_program = new Graphics::ShaderProgram(new Graphics::Shader("Content/Shaders/UV.frag", Graphics::Shader::EType::Fragment),
																	new Graphics::Shader("Content/Shaders/Transform.vert", Graphics::Shader::EType::Vertex));
}

void OpenGLPipeline::PreRender() {
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

void OpenGLPipeline::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static Graphics::ShaderProgram program(new Graphics::Shader("Content/Shaders/Textured.frag", Graphics::Shader::EType::Fragment),
															  new Graphics::Shader("Content/Shaders/Transform.vert", Graphics::Shader::EType::Vertex));
						
	static Camera cam;
					
	if (GInput->IsKeyDown('Z')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	if (GInput->IsKeyDown('V')) {
		uv_program->Bind();
	}
	else {
		program.Bind();
		texture.Bind();
	}

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
			auto pos = renderable->pos;
			// note that we're translating the scene in the reverse direction of where we want to move
			view = cam.GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
			program.SetShaderUniform("uTransform", &projection);
			program.SetShaderUniform("uView", &view);

			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), renderable->pos)*
				glm::rotate(glm::mat4(1.0f), renderable->rot.y, glm::vec3(1.0f, 0.0f, 0.0f))*
				glm::rotate(glm::mat4(1.0f), renderable->rot.x, glm::vec3(0.0f, 1.0f, 0.0f))*
				glm::scale(glm::mat4(1.0f), renderable->scale);

			program.SetShaderUniform("uModel", &matrix);
			auto r = reinterpret_cast<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get());
			//r->SetShaderProgram(program);
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

void OpenGLPipeline::AddRenderable(std::weak_ptr<Core::Renderable> renderer) {
	renderables.push_back(renderer);
}
