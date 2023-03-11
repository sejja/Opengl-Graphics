//
//  SDL_Window.cpp
//  OpenGL Graphics
//
//  Created by Diego Revilla on 11/03/23
//  Copyright � 2023. All rights reserved.
//

#include "SDLWindow.h"
#include <iostream>
#include <glew.h>

SDLWindow::SDLWindow() :
    mWindow(nullptr, SDL_DestroyWindow) {
}

void SDLWindow::Create() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        throw std::runtime_error(SDL_GetError());

    constexpr const char* const glsl_version = "#version 460";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    mWindow.reset(SDL_CreateWindow("OpenGL Graphics",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        960, 540,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));
    mContext = SDL_GL_CreateContext(mWindow.get());

    SDL_GL_MakeCurrent(mWindow.get(), mContext);
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) throw std::runtime_error("Failed to initialize OpenGL loader!");
}

bool SDLWindow::Present() {
    SDL_Event event;

    SDL_GL_SwapWindow(mWindow.get());
    return !(SDL_PollEvent(&event) && event.type == SDL_QUIT);
}

void SDLWindow::Destroy() {
    SDL_GL_DeleteContext(mContext);
    SDL_Quit();
}

void SDLWindow::SetDimensions(const glm::lowp_u16vec2& dim) {
    mDimensions = dim;
    SDL_SetWindowSize(mWindow.get(), dim.x, dim.y);
}