//
//  SharedResources.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#include <stack>
#include <algorithm>
#include "ResourceManager.h"
#include "Importers.h"

// ------------------------------------------------------------------------
/*! Initialize
*
*  Initializes the Resource Manager
*/ // --------------------------------------------------------------------
void ResourceManager::Initialize() {
	importers.insert({ "obj", new ModelImporter });
}

// ------------------------------------------------------------------------
/*! Get Resource
*
*   Gets a resource given the name
*/ // --------------------------------------------------------------------
std::shared_ptr<IResource> ResourceManager::GetResource(raw_text name) {
	auto it = resources.find(name);

	//If we do indeed have the resource
	if (it != resources.end()) {
		return it->second;
	}

	return AddResource(name);
}

// ------------------------------------------------------------------------
/*! Add Importer
*
*   Adds an Importer to the Resource Manager
*/ // --------------------------------------------------------------------
void ResourceManager::AddImporter(IResourceImporter* res, raw_text ext) noexcept {
	importers.insert(std::make_pair(ext, res));
}

// ------------------------------------------------------------------------
/*! Add Resource
*
*   Adds a Resource given the resource, the path and the name
*/ // --------------------------------------------------------------------
std::shared_ptr<IResource> ResourceManager::AddResource(raw_text mPath) {
	std::string_view temp_(mPath);

	//assertThis(!temp_.empty(), "Maybe you should think about giving an actual asset path, you fucking clown");
	if (temp_.empty()) return nullptr;

	//Try importing from file
	try {
		IResourceImporter* ireimp_ =
			GetImporterByExtension(temp_.substr(temp_.find_last_of(".") + 1).data());
		std::shared_ptr<IResource> res_(ireimp_->ImportFromFile(mPath));

		
		if (res_)
			resources.insert(std::make_pair(mPath, res_));

		return res_;

		//If we could not find an importer
	}
	catch (...) {
		return nullptr;
	}
}

// ------------------------------------------------------------------------
/*! Get Importer By Extension
*
*   Gets an importer by it's extension
*/ // --------------------------------------------------------------------
IResourceImporter* ResourceManager::GetImporterByExtension(raw_text ext) const noexcept {
	return importers.at(ext);
}

// ------------------------------------------------------------------------
/*! Remove Resource
*
*   Removes a resource
*/ // --------------------------------------------------------------------
void ResourceManager::RemoveResource(raw_text name) noexcept {
	resources.erase(name);
}

void ResourceManager::ShutDown() {
	std::for_each(importers.begin(), importers.end(), [](const std::pair<std::string, IResourceImporter*>& x)
		{
			Allocator<IResourceImporter>::deallocate(x.second);
		});

	resources.clear();
}

// ------------------------------------------------------------------------
/*! Destructor
*
*   EMPTY FUNCTION
*/ // --------------------------------------------------------------------
IResource::~IResource() {}