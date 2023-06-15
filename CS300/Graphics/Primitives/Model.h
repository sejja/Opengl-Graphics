#pragma once
#include <string>
#include <glew.h>
#include <vector>
#include <tinyobjloader.h>
#include "Material.h"
#include "ShaderProgram.h"

class Model {
public:
	struct ModelVertex {
		tinyobj::real_t pos[3];
		tinyobj::real_t normal[3];
		tinyobj::real_t uv[2];
		tinyobj::real_t tangent[3];
		tinyobj::real_t bitangent[3];
	};

	~Model();
	void Clear();
	void LoadFromFile(const std::string& inputfile);
	GLuint GetVao() { return mVAO; }
	size_t GetVertexCount() { return mCount; }
	void set_uniforms(Core::Graphics::ShaderProgram& s) const;

private:
	void UploadToGPU(std::vector<float>& vertices, std::vector<int>& indexes);
	std::size_t mCount;
	GLuint mVAO, mVBO, mIBO;
	std::vector<tinyobj::shape_t> shapes;
	Graphics::Material mMaterial;
	tinyobj::attrib_t attrib;
};

