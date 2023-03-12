//
//	main.cpp
//	CS300
//
//	Created by Diego Revilla on 10/05/22
//	Copyright © 2022 Digipen. All Rights reserved
//

#include "Application.h"
#include "Core/Window/SDLWindow.h"
#include "OpenGLPipeline.h"
#include "Scene.h"

// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {
    struct MyOpenGLApp : public
       GraphicApplication<SDLWindow, OpenGLPipeline> {
       Scene mScene;
    } app;
    
    app.SetDimensions({ 1072, 780 });
	app.mScene.CreateScene("Content/Maps/Scene.level");
    app.mScene.UploadObjectsToPipeline(app.mPipe);
    app.Run();

    return 0;
}