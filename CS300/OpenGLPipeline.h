#ifndef _OPEN_GL_PIPELINE__H_
#define _OPEN_GL_PIPELINE__H_

#include "Core/Pipeline.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include "Graphics/Primitives/ShaderProgram.h"

class OpenGLPipeline : public Core::Pipeline {
public:
	void Init() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void Shutdown() override;
	virtual void SetDimensions(const glm::lowp_u16vec2& dim) override;
	void AddRenderable(std::weak_ptr<Core::Renderable> renderer);
	void UploadLightDataToGPU(const AssetReference<Graphics::ShaderProgram>& shader);

private:
	std::vector<std::weak_ptr<Core::Renderable>> renderables;
	glm::mat4x4 mProjectionMatrix;
};

#endif