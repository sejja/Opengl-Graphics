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

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*  Initializes the Resource Manager
	*/ // --------------------------------------------------------------------
	void ResourceManager::Initialize() {
	}

	// ------------------------------------------------------------------------
	/*! Get Resource
	*
	*   Gets a resource given the name
	*/ // --------------------------------------------------------------------
	std::shared_ptr<Assets::IResource> ResourceManager::GetResource(raw_text name) {
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
	void ResourceManager::AddImporter(Assets::IResourceImporter* res, raw_text ext) noexcept {
		importers.insert(std::make_pair(ext, res));
	}

	// ------------------------------------------------------------------------
	/*! Add Resource
	*
	*   Adds a Resource given the resource, the path and the name
	*/ // --------------------------------------------------------------------
	std::shared_ptr<Assets::IResource> ResourceManager::AddResource(raw_text mPath) {
		std::string_view temp_(mPath);

		//assertThis(!temp_.empty(), "Maybe you should think about giving an actual asset path, you fucking clown");
		if (temp_.empty()) return nullptr;

		//Try importing from file
		try {
			Assets::IResourceImporter* ireimp_ =
				GetImporterByExtension(temp_.substr(temp_.find_last_of(".") + 1).data());
			std::shared_ptr<Assets::IResource> res_(ireimp_->ImportFromFile(mPath));

			
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
	Assets::IResourceImporter* ResourceManager::GetImporterByExtension(raw_text ext) const noexcept {
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
		std::for_each(importers.begin(), importers.end(), [](const std::pair<std::string, Assets::IResourceImporter*>& x)
			{
				Allocator<Assets::IResourceImporter>::deallocate(x.second);
			});

		resources.clear();
	}


	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // --------------------------------------------------------------------
		IResource::~IResource() {}
	}
}