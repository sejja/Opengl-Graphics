#pragma once
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
	CS300Parser parser;
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<Object> lights;
};