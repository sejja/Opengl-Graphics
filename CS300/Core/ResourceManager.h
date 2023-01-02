﻿//
//  SharedResources.h
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#ifndef _RESOURCE_MANAGER__H_
#define _RESOURCE_MANAGER__H_

#include <string>
#include <memory>
#include <map>
#include "Allocator.h"
#include "Singleton.h"

namespace Engine {
	namespace Assets {
		struct IResource {
			virtual ~IResource();
		};

		class IResourceImporter {
		public:
			virtual IResource* ImportFromFile(const char* filename) const = 0;
		};

		template<typename Ty_>
		class TResource : public IResource {
		public:
			~TResource() {
				Allocator<Ty_> alloc;
				alloc.destroy(rawData);
				alloc.deallocate(rawData);
			}
			// ------------------------------------------------------------------------
			/*! Get
			*
			*   Gets the Resource as something
			*/ // --------------------------------------------------------------------
			DONTDISCARD Ty_ inline* Get() {
				return reinterpret_cast<Ty_*>(rawData);
			}

		private:
			Ty_* rawData = nullptr;
		};
	}

	class ResourceManager : public Singleton<ResourceManager> {
	public:
		using raw_text = const char*;
		// ------------------------------------------------------------------------
		/*! Get Resource
		*
		*   Gets a Loaded resource, if not loaded load it
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD std::shared_ptr<Assets::TResource<Ty_>> GetResource(raw_text name) noexcept {
			return std::reinterpret_pointer_cast<Assets::TResource<Ty_>>(GetResource(name));
		}

		DONTDISCARD std::shared_ptr<Assets::IResource> GetResource(raw_text name);
		void RemoveResource(raw_text name) noexcept;
		void Initialize();
		DONTDISCARD Assets::IResourceImporter* GetImporterByExtension(raw_text ext) const noexcept;
		void AddImporter(Assets::IResourceImporter* res, raw_text ext) noexcept;
		void ShutDown();

	protected:
		std::shared_ptr<Assets::IResource> AddResource(raw_text mPath);

		std::map<std::string, std::shared_ptr<Assets::IResource>> resources;
		std::map<std::string, Assets::IResourceImporter*> importers;
	};
}

#define GContent (&Engine::ResourceManager::Instance())

template<typename T>
using Asset = std::shared_ptr<Engine::Assets::TResource<T>>;

template<typename T>
using AssetReference = std::weak_ptr<Engine::Assets::TResource<T>>;

#endif