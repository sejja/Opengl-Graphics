//
//	Scene.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include <memory>
#include "CS300Parser.h"
#include "Renderables.h"

namespace Core {
	class Renderable;
}

class Scene {
public:
	void CreateScene(const char*);

	template<typename PIPE>
	void UploadObjectsToPipeline(PIPE& pipe) {
		using namespace Core;

		for (auto& x : objects) {
			for (auto& comp : x->components) {
				if (std::dynamic_pointer_cast<Renderable>(comp).get() != nullptr) {
					pipe.AddRenderable(std::dynamic_pointer_cast<Renderable>(comp));
				}
			}
		}
	}

public:
	static CS300Parser parser;
	std::vector<std::shared_ptr<Object>> objects;
};

#endif