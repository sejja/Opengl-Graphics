//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright © 2023. All Rights reserved
//

#include "Importers.h"
#include "../Renderables.h"
#include "../Graphics/Primitives/Model.h"
#include "Allocator.h"
#include "PageAllocator.h"

namespace Assets {
	
}
// ------------------------------------------------------------------------
/*! Allocate
*
*   Allocates sizeof(TYPE) * size bytes
*/ //---------------------------------------------------------------------
std::shared_ptr<IResource> ModelImporter::ImportFromFile(const std::string_view& filename) const {
	PageAllocator<TResource<Model>> resalloc;
	PageAllocator<Model> modealloc;

	std::shared_ptr<TResource<Model>> const rawResource(resalloc.New(), [](TResource<Model>* p) {
		PageAllocator<TResource<Model>> resalloc_;
		p->Get()->Clear();
		resalloc_.deallocate(p);
	});
	rawResource->rawData.reset(modealloc.New());
	rawResource->rawData->LoadFromFile(filename.data());

	return rawResource;
}
