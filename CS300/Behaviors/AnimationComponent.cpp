#include "AnimationComponent.h"
#include "CS300Parser.h"
#include <SDL.h>

namespace Behaviors {
	void AnimationComponent::Update() {
		if (!mParent.expired()) {
			auto parent = mParent.lock();
		
			parent->transform.mPostion.x = cos(mElapsed +  PI / 2) * 50 + mOriginal.x;
			parent->transform.mPostion.y = sin(mElapsed / 2 + PI / 2) * 10 + mOriginal.y;
			parent->transform.mPostion.z = sin(mElapsed + PI / 2) * 50 + mOriginal.z;

			mElapsed += 0.016;
		}
	}

	void AnimationComponent2::Update() {
		if (!mParent.expired()) {
			auto parent = mParent.lock();

			parent->transform.mPostion.x = cos(mElapsed) * 50 + mOriginal.x;
			parent->transform.mPostion.y = sin(mElapsed / 2) * 10 + mOriginal.y;
			parent->transform.mPostion.z = sin(mElapsed) * 50 + mOriginal.z;

			mElapsed += 0.016;
		}
	}

	void AnimationComponent3::Update() {
		if (!mParent.expired()) {
			auto parent = mParent.lock();
			parent->transform.mPostion.y = sin(mElapsed) * 50 + mOriginal.y;

			mElapsed += 0.016;
		}
	}

	void AnimationComponent4::Update() {
		if (!mParent.expired()) {
			auto parent = mParent.lock();

			parent->transform.mPostion.y = cos(mElapsed) * 50 + mOriginal.y;
			parent->transform.mPostion.x = sin(mElapsed / 2) * 10 + mOriginal.x;
			parent->transform.mPostion.z = sin(mElapsed) * 50 + mOriginal.z;

			mElapsed += 0.016;
		}
	}
}