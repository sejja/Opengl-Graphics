//
//	Camera.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

#include <glm/glm.hpp>

class Camera {
public:
	Camera();	
	void Update();
	glm::mat4 GetViewMatrix();
	
public:
	glm::vec3 mPosition, mTargetPosition;
};

#endif