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
    struct MyOpenGLApp : public
       Core::GraphicApplication<SDLWindow, Core::Graphics::OpenGLPipeline> {
       Scene mScene;
    } app;
    
	app.mScene.CreateScene("Content/Maps/Scene.level", [&app](const std::shared_ptr<Object>& obj) {
        std::for_each(std::execution::unseq, obj->components.begin(), obj->components.end(), [&app](const std::shared_ptr<Component>& comp) {
            std::shared_ptr<Core::Renderable> renderable = std::dynamic_pointer_cast<Core::Renderable>(comp);

            //If the object is a renderable
            if (renderable) app.GetPipelineRef().AddRenderable(renderable);
            });
        });

    app.SetTickFunction([&app]() {
		app.mScene.Tick();
    });
    
    app.Run();

    return 0;
}