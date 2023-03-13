#include "Scene.h"
#include "Behaviors/AnimationComponent.h"
#include "Renderables.h"
#include "Core/Pipeline.h"

void Scene::CreateScene(const char* file) {
	parser.LoadDataFromFile(file);
	
	for (auto& x : parser.objects) {
		//Object obj;
		std::shared_ptr<Object> obj = std::make_shared<Object>();
		obj->transform.mPostion = x.pos;
		obj->transform.mRotation = glm::radians(x.rot);
		obj->transform.mScale = x.sca;
		std::unique_ptr<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::make_unique<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj);
		renderer->SetMesh(GContent->GetResource<Model>(x.mesh.c_str()));
		renderer->SetShaderProgram(GContent->GetResource<Graphics::ShaderProgram>("Content/Shaders/Textured.shader"));
		obj->components.emplace_back(std::move(renderer));
		objects.emplace_back(obj);
	}

	int i = 0;
	
	for (auto& x : parser.lights) {
		std::shared_ptr<Object> obj = std::make_shared<Object>();
		obj->transform.mPostion = x.pos;
		obj->transform.mScale = { 1.f, 1.f, 1.f };
		obj->transform.mRotation = glm::vec3(0.f, 0.f, 0.f);
		std::shared_ptr<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj);
		renderer->SetMesh(GContent->GetResource<Model>("Content/Meshes/sphere_20_averaged.obj"));
		renderer->SetShaderProgram( GContent->GetResource<Graphics::ShaderProgram>("Content/Shaders/White.shader"));
		obj->components.emplace_back(std::move(renderer));

		switch (i % 4) {
		case 0: {
			std::shared_ptr<Behaviors::AnimationComponent> comp = std::make_shared<Behaviors::AnimationComponent>(obj);
			obj->components.emplace_back(std::move(comp));
		}
			break;

		case 1: {
			std::shared_ptr<Behaviors::AnimationComponent2> comp = std::make_shared<Behaviors::AnimationComponent2>(obj);
			obj->components.emplace_back(std::move(comp));
		}
			break;

		case 2: {
			std::shared_ptr<Behaviors::AnimationComponent3> comp = std::make_shared<Behaviors::AnimationComponent3>(obj);
			obj->components.emplace_back(std::move(comp));
		}
			break;
		
		default: {
			std::shared_ptr<Behaviors::AnimationComponent4> comp = std::make_shared<Behaviors::AnimationComponent4>(obj);
			obj->components.emplace_back(std::move(comp));
		}
			break;
		}

		i++;
		
		objects.emplace_back(obj);
	}
}
