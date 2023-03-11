#ifndef _MATERIAL__H_
#define _MATERIAL__H_

#include "CommonDefines.h"
#include "glm/vec4.hpp"
#include <exception>

namespace Graphics {
	class Material {
	public:
		struct MaterialException : public std::exception {
			MaterialException(const char* message) : std::exception(message) {}
		};
	
		Material() = default;
		Material(const Material&) = default;
		Material(Material&&) = default;
		Material& operator=(const Material&) = default;
		Material& operator=(Material&&) = default;
		~Material() = default;
		void SetAmbient(const glm::vec4& ambient);
		void SetDiffuse(const glm::vec4& diffuse);
		void SetSpecular(const glm::vec4& specular);
		void SetEmissive(const glm::vec4& emissive);
		void SetShininess(const float n);
		DONTDISCARD constexpr inline glm::vec4 GetAmbient() const;
		DONTDISCARD constexpr inline glm::vec4 GetDiffuse() const;
		DONTDISCARD constexpr inline glm::vec4 GetSpecular() const;
		DONTDISCARD constexpr inline glm::vec4 GetEmissive() const;
		DONTDISCARD constexpr inline float GetShininess() const;
		void UploadToGPU() const;
		
	private:
	#pragma region // Variables
		glm::vec4 mAmbient;
		glm::vec4 mEmissive;
		glm::vec4 mSpecular;
		glm::vec4 mDiffuse;
		float mShininess;
	#pragma endregion
	};

	constexpr inline glm::vec4 Material::GetAmbient() const {
		return mAmbient;
	}

	constexpr inline glm::vec4 Material::GetDiffuse() const {
		return mDiffuse;
	}

	constexpr inline glm::vec4 Material::GetSpecular() const {
		return mSpecular;
	}

	constexpr inline glm::vec4 Material::GetEmissive() const {
		return mEmissive;
	}

	constexpr  inline float Material::GetShininess() const {
		return mShininess;
	}
}

#endif