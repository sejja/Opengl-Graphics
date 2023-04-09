#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	~Camera();
	
	void Update();
	glm::mat4 GetViewMatrix();
	
public:
	glm::vec3 mPosition, targetPos;
};