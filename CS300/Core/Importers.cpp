#include "Importers.h"
#include "../Renderables.h"
#include "../Graphics/Primitives/Model.h"
#include "Allocator.h"

IResource* ModelImporter::ImportFromFile(const char* filename) const {
	TResource<Model>* rawResource = Allocator<TResource<Model>>::New();
	Model* _tex = Allocator<Model>::New();
	_tex->LoadFromFile(filename);
	rawResource->rawData = _tex;

	return rawResource;
}
