#pragma once

#include "ResourceManager.h"

class ModelImporter : public IResourceImporter {
public:
	DONTDISCARD IResource* ImportFromFile(const char* filename) const override;
};