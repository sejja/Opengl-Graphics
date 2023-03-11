#include "Scene.h"

void Scene::CreateScene(const char* file) {
	parser.LoadDataFromFile(file);
	
	for (auto& x : parser.objects) {
		Object obj;
		obj.modelrend.get()->SetMesh(GContent->GetResource<Model>(x.mesh.c_str()));
		obj.modelrend.get()->pos = x.pos;
		obj.modelrend.get()->scale = x.sca;
		obj.modelrend.get()->rot = glm::vec3(glm::radians(x.rot.x), glm::radians(x.rot.y), glm::radians(x.rot.z));
		
		objects.push_back(obj);
		
	}
}
