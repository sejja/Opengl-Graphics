#pragma once

#include "Core/Window.h"
#include <SDL.h>
#include <glm/glm.hpp>

#undef main

class SDLWindow : public Core::Window {
public:
	void Create() override;
	bool Present() override;
	void Destroy() override;
	void SetDimensions(const glm::lowp_u16vec2& dim) override;

private:
	SDL_GLContext mContext;
	SDL_Window* mWindow;
	glm::lowp_u16vec2 mDimensions;
};