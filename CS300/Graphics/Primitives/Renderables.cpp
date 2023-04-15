//
//	Renderables.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#include "Graphics/Primitives/Renderables.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>
#include <glew.h>

namespace Core {
	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders this  Model
	*/ //----------------------------------------------------------------------
	void ModelRenderer<GraphicsAPIS::OpenGL>::Render() {
		Model* const model = mModel->Get();
	
		model->set_uniforms(*mShaderProgram->Get());
		glBindVertexArray(model->GetVao());
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model->GetVertexCount()));
	}
}