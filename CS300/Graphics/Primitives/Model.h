#pragma once
#include <string>
#include <glew.h>
#include <vector>
#include <tinyobjloader.h>

class Model {

public:
	~Model();
	void Clear();
	void LoadFromFile(const std::string& inputfile);
	static Model CreatePlane();
	static Model CreateCube();
	static Model CreateCone(unsigned slices);
	static Model CreateCylinder();
	static Model CreateSphere();
	GLuint GetVao() { return mVAO; }
	size_t GetVertexCount() { return mCount; }

private:
	void UploadToGPU(std::vector<float>& vertices, std::vector<int>& indexes);
	std::size_t mCount;
	GLuint mVAO, mVBO, mIBO;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	tinyobj::attrib_t attrib;
};

