//
//	Importers.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright © 2023. All Rights reserved
//

#include "Importers.h"
#include "../../Renderables.h"
#include "../../Graphics/Primitives/Model.h"
#include "../Allocator.h"
#include "../PageAllocator.h"
#include "Importers.h"
#include "../../Graphics/Primitives/Texture.h"

namespace Assets {
	// ------------------------------------------------------------------------
	/*! Import From File
	*
	*   Imports Models from File
	*/ //----------------------------------------------------------------------
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

	// ------------------------------------------------------------------------
	/*! Import From File
	*
	*   Imports Textures from File
	*/ //----------------------------------------------------------------------
	std::shared_ptr<IResource> Assets::TextureImporter::ImportFromFile(const std::string_view& filename) const {
		using Graphics::Texture;
		PageAllocator<TResource<Texture>> resalloc;

		std::shared_ptr<TResource<Texture>> const rawResource(resalloc.New(), [](TResource<Texture>* p) {
			PageAllocator<TResource<Texture>> resalloc_;
			resalloc_.destroy(p);
			resalloc_.deallocate(p);
			});


		Graphics::Texture* _tex = Allocator<Texture>::New();
		_tex->LoadFromFile(filename.data());
		rawResource->rawData.reset(_tex);
		auto toup = reinterpret_cast<Texture*>(rawResource->rawData.get());
		toup->UploadToGPU();

		return rawResource;
	}
}