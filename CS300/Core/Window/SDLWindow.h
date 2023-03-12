//
//  SDL_Window.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 11/03/23
//  Copyright � 2023. All rights reserved.
//

#ifndef _SDL_WINDOW__H_
#define _SDL_WINDOW__H_

#include "Core/Window.h"
#include <SDL.h>
#include "CommonDefines.h"

#undef main

class SDLWindow : public Core::Window {
public:
	SDLWindow();
	void Create() override;
	bool Present() override;
	void Destroy() override;
	void SetDimensions(const glm::lowp_u16vec2& dim) override;

private:
	SDL_GLContext mContext;
	std::deleted_pointer<SDL_Window> mWindow;
	glm::lowp_u16vec2 mDimensions;
};

#endif