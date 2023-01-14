#pragma once

#include "ResourceManager.h"

class ModelImporter : public IResourceImporter {
public:
	DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
};