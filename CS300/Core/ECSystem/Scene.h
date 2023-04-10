//
//	Scene.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/03/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include <memory>
#include <algorithm>
#include <execution>
#include "CS300Parser.h"
#include "Renderables.h"

namespace Core {
	class Renderable;
}

class Scene {
#pragma region //Functions
public:
	void CreateScene(const std::string_view& file);

	template<typename PIPE>
	void UploadObjectsToPipeline(PIPE& pipe);
	void Tick();
#pragma endregion	

#pragma region //Variables
private:
	static CS300Parser mParser;
	std::vector<std::shared_ptr<Object>> mObjects;
#pragma endregion
};

// ------------------------------------------------------------------------
/*! Get Resource
*
*   Gets a resource given the name
*/ // ---------------------------------------------------------------------
template<typename PIPE>
void Scene::UploadObjectsToPipeline(PIPE& pipe) {
	using namespace Core;

	std::for_each(std::execution::unseq, mObjects.begin(), mObjects.end(), [&pipe](const std::shared_ptr<Object>& obj) {
		std::for_each(std::execution::unseq, obj->components.begin(), obj->components.end(), [&pipe](const std::shared_ptr<Component>& comp) {
			std::shared_ptr<Renderable> renderable = std::dynamic_pointer_cast<Renderable>(comp);
			
			//If the object is a renderable
			if (renderable) pipe.AddRenderable(std::dynamic_pointer_cast<Renderable>(comp));
			});
		});
}

#endif