#include "Scene.h"

void Scene::CreateScene(const char* file) {
	parser.LoadDataFromFile(file);
	
	for (auto& x : parser.objects) {
		Object obj;

		if (x.mesh == "PLANE" || x.mesh == "CUBE" || x.mesh == "CONE"
			|| x.mesh == "CYLINDER" || x.mesh == "SPHERE")
			continue;

		obj.modelrend.get()->SetMesh(GContent->GetResource<Model>(x.mesh.c_str()));
		
		objects.push_back(obj);
	}
}
