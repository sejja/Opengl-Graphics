#pragma once
#include <string>
#include <glew.h>
#include <vector>
#include <tinyobjloader.h>

class Model {

public:
	void LoadFromFile(const std::string& inputfile);

	std::size_t mCount;
	GLuint mVAO, mVBO, mIBO;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	tinyobj::attrib_t attrib;
};

