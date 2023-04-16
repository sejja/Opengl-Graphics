//
//	main.cpp
//	CS300
//
//	Created by Diego Revilla on 10/05/22
//	Copyright © 2022. All Rights reserved
//

#include "Core/Application.h"
#include "Core/Window/SDLWindow.h"
#include "Graphics/OpenGLPipeline.h"
#include "Core/ECSystem/Scene.h"

// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {
    Singleton<ResourceManager>::Instance().Initialize();

    struct MyOpenGLApp : public
       GraphicApplication<SDLWindow, Core::Graphics::OpenGLPipeline> {
       Scene mScene;
    } app;
    
    app.SetDimensions({ 1072, 780 });
	app.mScene.CreateScene("Content/Maps/Scene.level");
	app.mScene.UploadObjectsToPipeline(app.GetPipelineRef());
    app.SetTickFunction([&app]() {
        Singleton<Engine::InputManager>::Instance().ProcessInput();
		app.mScene.Tick();
    });
    
    app.Run();
    Singleton<ResourceManager>::Instance().ShutDown();

    return 0;
}