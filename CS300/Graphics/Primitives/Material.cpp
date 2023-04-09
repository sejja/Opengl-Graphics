#include "Material.h"
#include <cassert>
#include <exception>
#include <list>

namespace Graphics {
    void Material::SetAmbient(const glm::vec4& ambient) {
        mAmbient = ambient;
        std::list<int> listi;
        listi.size();
    }

    void Material::SetDiffuse(const glm::vec4& diffuse) {
        mDiffuse = diffuse;
    }

    void Material::SetSpecular(const glm::vec4& specular) {
        mSpecular = specular;
        
    }

    void Material::SetEmissive(const glm::vec4& emissive) {
        mEmissive = emissive;
    }

	void Material::SetShininess(const float n) {
        if (n < 0.f || n > 1.f)
            throw std::exception("n is not in the range of 0 to 1");
            
		mShininess = n;
	}

    void Material::UploadToGPU() const {
    }
}