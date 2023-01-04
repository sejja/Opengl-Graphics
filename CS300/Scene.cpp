#include "Scene.h"

void Scene::CreateScene(const char* file) {
	parser.LoadDataFromFile(file);
	
	for (auto& x : parser.objects) {
		Object obj;

		obj.modelrend.get()->SetMesh(GContent->GetResource<Model>(x.mesh.c_str()));
		obj.modelrend.get()->pos = x.pos;
		obj.modelrend.get()->scale = x.sca;
		obj.modelrend.get()->rot = x.rot;
		
		objects.push_back(obj);
		
	}
}
