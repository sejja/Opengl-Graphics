#include "Pipeline.h"

void Core::Pipeline::AddRenderable(std::weak_ptr<Renderable> x) {
	mRenderables.emplace(x.lock());
}
