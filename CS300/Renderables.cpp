#include "Graphics/Primitives/Renderables.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tinyobjloader.h>
#include <glew.h>

namespace Core {
	void ModelRenderer<GraphicsAPIS::OpenGL>::Render() {
		mModel->Get()->set_uniforms(*mShaderProgram->Get());
		glBindVertexArray(mModel->Get()->GetVao());
		glDrawArrays(GL_TRIANGLES, 0, mModel->Get()->GetVertexCount());
	}
}