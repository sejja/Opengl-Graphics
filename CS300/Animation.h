//
//	Animation.h
//	CS300
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _ANIMATION__H_
#define _ANIMATION__H_

#include <glm/glm.hpp>
#include <functional>
#include <map>
#include <string>

namespace Animations
{
    using AnimUpdater = std::function<glm::vec3(const glm::vec3&, const glm::vec3&, float)>;
    class Anim
    {
    public:
        Anim(const AnimUpdater& u, const glm::vec3& p) :
            updater(u),
            param(p) {}

        glm::vec3 Update(const glm::vec3& pos, float time) const;

    private:
        AnimUpdater updater;
        glm::vec3   param;
    };

    glm::vec3 Sinusoidal(const glm::vec3& pos, const glm::vec3& offset, float time);
    glm::vec3 Orbit(const glm::vec3& pos, const glm::vec3& center, float time);

    const std::map<std::string, AnimUpdater> NameToUpdater = {
        { "sinusoidal", Sinusoidal },
        { "orbit", Orbit }
    };

}

#endif