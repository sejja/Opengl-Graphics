﻿//
//  SDL_Window.cpp
//  OpenGL Graphics
//
//  Created by Diego Revilla on 11/03/23
//  Copyright � 2023. All rights reserved.
//

#include <glew.h>
#include "Core/Window/SDLWindow.h"

// ------------------------------------------------------------------------
/*! Constructor
*
*   Initializes the Destructor (What a thing)
*/ //----------------------------------------------------------------------
SDLWindow::SDLWindow() noexcept :
    mWindow(nullptr, SDL_DestroyWindow), mDimensions({0, 0}) {
}

// ------------------------------------------------------------------------
/*! Destructor
*
*   Destroys the Window, and the context too
*/ //----------------------------------------------------------------------
SDLWindow::~SDLWindow() {
    SDL_GL_DeleteContext(mContext);
    SDL_Quit();
}

// ------------------------------------------------------------------------
/*! Create
*
*   Creates the window buffer on SDL. as well as a OpenGL Context
*/ //----------------------------------------------------------------------
void SDLWindow::Create() {
    //If there is an error creating the 
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        throw SDLWindowException(SDL_GetError());
   
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    mWindow.reset(SDL_CreateWindow("OpenGL Graphics",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        mDimensions.x, mDimensions.y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

	//If we got no pointer to the window, throw an exception
    if(!mWindow)
		throw SDLWindowException("Window could not be created");
    
    mContext = SDL_GL_CreateContext(mWindow.get());

    SDL_GL_MakeCurrent(mWindow.get(), mContext);
    SDL_GL_SetSwapInterval(1);

	//If there was any error, throw an exception
    if (glewInit() != GLEW_OK) 
        throw SDLWindowException("Failed to initialize OpenGL loader!");
}

// ------------------------------------------------------------------------
/*! Present
*
*   Presents the Window, and retrieves any event needded
*/ //----------------------------------------------------------------------
bool SDLWindow::Present() {
    SDL_Event event;

    SDL_GL_SwapWindow(mWindow.get());

    return !(SDL_PollEvent(&event) && event.type == SDL_QUIT);
}

// ------------------------------------------------------------------------
/*! Set Dimension
*
*   Set Dimensions of the Window
*/ //----------------------------------------------------------------------
void SDLWindow::SetDimensions(const glm::lowp_u16vec2& dim) {
    mDimensions = dim;
    SDL_SetWindowSize(mWindow.get(), dim.x, dim.y);
}