#pragma once

#include <glm/glm.hpp>

namespace Mathematics {
	struct Transformation {
		glm::vec3 mPostion;
		glm::vec3 mScale;
		glm::vec3 mRotation;
	};
}