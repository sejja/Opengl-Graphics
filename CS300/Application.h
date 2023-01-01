#ifndef _APPLICATION__H_
#define _APPLICATION__H_

#include <functional>
#include <type_traits>
#include "Core/Window.h"
#include "Core/Pipeline.h"

class Application {
public:
	virtual ~Application();
	virtual void Run() = 0;
};

template<class WINDOW, class PIPELINE>
class GraphicApplication : protected Application {
public:
	GraphicApplication();
	~GraphicApplication();
	void Run() override;
	void SetTickFunction(const std::function<void()>& const tick);
	void SetDimensions(const glm::lowp_u16vec2& dim);
private:
	WINDOW mWindow;
	PIPELINE mPipe;
	std::function<void()> mTick;
};

template<class WINDOW, class PIPELINE>
GraphicApplication<WINDOW, PIPELINE>::GraphicApplication() {
	static_assert(std::is_base_of<Core::Window, WINDOW>::value);
	static_assert(std::is_base_of<Core::Pipeline, PIPELINE>::value);
	mWindow.Create();
	mPipe.Init();
}

template<class WINDOW, class PIPELINE>
GraphicApplication<WINDOW, PIPELINE>::~GraphicApplication() {
	mPipe.Shutdown();
	mWindow.Destroy();
}

template<class WINDOW, class PIPELINE>
void GraphicApplication<WINDOW, PIPELINE>::Run() {
	while(mWindow.Present()) {
		if(mTick) mTick();
		mPipe.PreRender();
		mPipe.Render();
		mPipe.PostRender();
	}
}

template<class WINDOW, class PIPELINE>
inline void GraphicApplication<WINDOW, PIPELINE>::SetTickFunction(const std::function<void()>& const tick) {
	mTick = tick;
}

template<class WINDOW, class PIPELINE>
inline void GraphicApplication<WINDOW, PIPELINE>::SetDimensions(const glm::lowp_u16vec2& dim) {
	mWindow.SetDimensions(dim);
	mPipe.SetDimensions(dim);
}

#endif