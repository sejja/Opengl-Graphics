#pragma once
#include "CS300Parser.h"
#include "Renderables.h"

struct Object {	
	glm::vec3 position, scale, rotation;
	
	std::shared_ptr<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>> modelrend
		= std::make_shared<Core::ModelRenderer<Core::GraphicsAPIS::OpenGL>>();
};

class Scene {
public:
	void CreateScene(const char*);

	template<typename PIPE>
	void UploadObjectsToPipeline(PIPE& pipe) {
		for (auto& obj : objects) {
			pipe.AddRenderable(obj.modelrend);
		}
	}

private:
	CS300Parser parser;
	std::vector<Object> objects;
};