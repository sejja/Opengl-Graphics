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
       Core::GraphicApplication<SDLWindow, Core::Graphics::OpenGLPipeline> {
       Scene mScene;
    } app;
    
    app.SetDimensions({ 1072, 780 });
	app.mScene.CreateScene("Content/Maps/Scene.level", [&app](const std::shared_ptr<Object>& obj) {
        std::for_each(std::execution::unseq, obj->components.begin(), obj->components.end(), [&app](const std::shared_ptr<Component>& comp) {
            std::shared_ptr<Core::Renderable> renderable = std::dynamic_pointer_cast<Core::Renderable>(comp);

            //If the object is a renderable
            if (renderable) app.GetPipelineRef().AddRenderable(std::dynamic_pointer_cast<Core::Renderable>(comp));
            });
        });
	//app.mScene.UploadObjectsToPipeline(app.GetPipelineRef());
    app.SetTickFunction([&app]() {
        Singleton<Core::InputManager>::Instance().ProcessInput();
		app.mScene.Tick();
    });
    
    app.Run();
    Singleton<ResourceManager>::Instance().ShutDown();

    return 0;
}