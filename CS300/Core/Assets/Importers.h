//
//	Importers.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _IMPORTERS__H_
#define _IMPORTERS__H_

#include "../ResourceManager.h"

namespace Assets {
	class ModelImporter : public IResourceImporter {
	public:
		DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
	};

	class TextureImporter : public IResourceImporter {
	public:
		DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
	};

	class ShaderProgramImporter : public IResourceImporter {
	public:
		DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
	};

	class ShaderImporter : public IResourceImporter {
	public:
		DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
	};
}

#endif