#include "Model.h"
#include <tinyobjloader.h>
#include <glew.h>
#include <iostream>

void Model::LoadFromFile(const std::string& inputfile) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<tinyobj::real_t> vertices;
	std::vector<tinyobj::real_t> normals;
	std::vector<int> indexes;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
				vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
				vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

				//tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				//tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				//tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
					vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
					vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);

					//tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					//tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					//tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
					vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);

					//tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					//tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}
				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tinyobj::real_t) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	mCount = vertices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(tinyobj::index_t), indexes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(GLfloat), (void*)3);
	// UVs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(GLfloat), (void*)6);

	glBindVertexArray(0);
}