#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SDL.h"
#include "../Core/InputManager.h"

void Camera::Update() {
	if (GInput->IsKeyDown('A')) {
		mPosition.x += 1;
	}

	if (GInput->IsKeyDown('D')) {
		mPosition.x -= 1;
	}

	if (GInput->IsKeyDown('W')) {
		mPosition.z += 1;
	}

	if (GInput->IsKeyDown('S')) {
		mPosition.z -= 1;
	}
}

glm::mat4 Camera::GetViewMatrix() {

	return glm::lookAt(mPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

Camera::Camera() :
	mPosition(0, 0, 100) {
}

Camera::~Camera()
{
}
