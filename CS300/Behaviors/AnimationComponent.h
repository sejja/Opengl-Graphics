#pragma once
#include "Scene.h"
#include "Animation.h"
#include "CS300Parser.h"

namespace Behaviors {
	class AnimationComponent : public Component {
	public:
		AnimationComponent(std::weak_ptr<Object> parent) : 
		Component(parent), mElapsed(0), mOriginal(parent.lock()->transform.mPostion) {}
		~AnimationComponent() = default;
		void Update() override;
	
	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent2 : public Component {
	public:
		AnimationComponent2(std::weak_ptr<Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->transform.mPostion) {}
		~AnimationComponent2() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent3 : public Component {
	public:
		AnimationComponent3(std::weak_ptr<Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->transform.mPostion) {}
		~AnimationComponent3() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};

	class AnimationComponent4 : public Component {
	public:
		AnimationComponent4(std::weak_ptr<Object> parent) :
			Component(parent), mElapsed(0), mOriginal(parent.lock()->transform.mPostion) {}
		~AnimationComponent4() = default;
		void Update() override;

	private:
		float mElapsed;
		glm::vec3 mOriginal;

	};
} // namespace Behaviors