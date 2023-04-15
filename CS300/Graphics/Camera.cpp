#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SDL.h"
#include "../Core/InputManager.h"
#include "Global.h"

void Camera::Update() {
	if (Singleton<Engine::InputManager>::Instance().IsKeyDown('A')) {
		mPosition.x += 1;
	}

	if (Singleton<Engine::InputManager>::Instance().IsKeyDown('D')) {
		mPosition.x -= 1;
	}

	if (Singleton<Engine::InputManager>::Instance().IsKeyDown('W')) {
		mPosition.z += 1;
	}

	if (Singleton<Engine::InputManager>::Instance().IsKeyDown('S')) {
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
