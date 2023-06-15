//
//	Scene.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/03/23
//	Copyright © 2023. All Rights reserved
//

#include "Scene.h"
#include "Behaviors/AnimationComponent.h"
#include "Core/Pipeline.h"
#include "Graphics/Primitives/Light.h"
#include "Global.h"

CS300Parser Scene::mParser;

// ------------------------------------------------------------------------
/*! Create Scene
*
*   Creates a scene from a level file
*/ // ---------------------------------------------------------------------
void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Object>& obj)> upload) {
	mParser.LoadDataFromFile(file.data());
	
	std::for_each(std::execution::unseq, mParser.mObjects.begin(), mParser.mObjects.end(), [this, &upload](const CS300Parser::Transform& x) {
		std::shared_ptr<Object> obj = std::move(std::make_shared<Object>());
		obj->transform.mPostion = x.pos;
		obj->transform.mRotation = glm::radians(x.rot);
		obj->transform.mScale = x.sca;
		std::unique_ptr<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_unique<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj));
		renderer->SetMesh(Singleton<ResourceManager>::Instance().GetResource<Model>(x.mesh.c_str()));
		renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Textured.shader"));
		obj->components.emplace_back(std::move(renderer));
		upload(obj);
		mObjects.emplace_back(std::move(obj));
	});

	int i = 0;
	
	std::for_each(std::execution::seq, mParser.lights.begin(), mParser.lights.end(), [this, &i, &upload](const CS300Parser::Light& x) {
		std::shared_ptr<Object> obj = std::move(std::make_shared<Object>());
		obj->transform.mPostion = x.pos;
		obj->transform.mScale = { 1.f, 1.f, 1.f };
		obj->transform.mRotation = glm::vec3(0.f, 0.f, 0.f);
		std::shared_ptr<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_shared<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj));
		std::shared_ptr<Graphics::Primitives::Light> light = std::move(std::make_shared<Graphics::Primitives::Light>(obj));
		renderer->SetMesh(Singleton<ResourceManager>::Instance().GetResource<Model>("Content/Meshes/sphere_20_averaged.obj"));
		renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
		light->SetPosition(x.pos);
		light->mData.mDirection = x.dir;
		light->mData.mAmbient = glm::vec3(x.amb, x.amb, x.amb);
		light->mData.mDiffuse = x.col;
		light->SetSpecular(x.col);
		light->SetAttenuation(x.att);
		light->mData.mInner = x.inner;
		light->mData.mOutter = x.outer;
		light->mData.mFallOff = x.falloff;

		//If the light is a point light
		if (x.type == "POINT") light->mData.mType = Graphics::Primitives::Light::LightType::Point;
		
		//If the light is a directional light
		else if (x.type == "DIR")
			light->mData.mType = Graphics::Primitives::Light::LightType::Directional;
			
		//else, it's a spot light
		else light->mData.mType = Graphics::Primitives::Light::LightType::Spot;

		obj->components.emplace_back(std::move(renderer));
		obj->components.emplace_back(std::move(light));
		
		//Switch by the light index (to switch behaviors)
		switch (i % 4) {
		case 0:
			obj->components.emplace_back(std::move((std::make_shared<Behaviors::AnimationComponent>(obj))));
			break;

		case 1:
			obj->components.emplace_back(std::move((std::make_shared<Behaviors::AnimationComponent2>(obj))));
			break;

		case 2:
			obj->components.emplace_back(std::move((std::make_shared<Behaviors::AnimationComponent3>(obj))));
			break;

		default:
			obj->components.emplace_back(std::move((std::make_shared<Behaviors::AnimationComponent4>(obj))));
			break;
		}

		i++;
		upload(obj);
		mObjects.emplace_back(std::move(obj));
	});
}

// ------------------------------------------------------------------------
/*! Tick
*
*   Updates all the objects and it's components
*/ // ---------------------------------------------------------------------
void Scene::Tick() {
	std::for_each(std::execution::par, mObjects.begin(), mObjects.end(), [](const std::shared_ptr<Object>& x) {
		std::for_each(std::execution::par, x->components.begin(), x->components.end(), [](const std::shared_ptr<Component>& y) {
			y->Update();
			});
		});
}
