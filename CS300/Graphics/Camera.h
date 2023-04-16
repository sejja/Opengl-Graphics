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
#include <glm/gtc/matrix_transform.hpp>
#include "CommonDefines.h"
#include "Core/Events/EventDispatcher.h"

namespace Core {
	namespace Primitives {
		class Camera : Events::Listener {
		public:
			Camera();
			~Camera();
			DONTDISCARD inline glm::mat4&& GetViewMatrix() const;
			void HandleEvent(const Events::Event& event) override;
			
		public:
			glm::vec3 mPosition, mTargetPosition;
		};

		glm::mat4&& Camera::GetViewMatrix() const {
			return std::move(glm::lookAt(mPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
		}
	}
}

#endif