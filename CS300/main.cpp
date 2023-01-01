//
//	main.cpp
//	CS300
//
//	Created by Diego Revilla on 10/05/22
//	Copyright © 2022 Digipen. All Rights reserved
//

#include "Application.h"
#include "SDLWindow.h"
#include "OpenGLPipeline.h"

// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {
    GraphicApplication<SDLWindow, OpenGLPipeline> app;
    app.SetDimensions({ 1072, 780 });
    app.Run();

    return 0;
}