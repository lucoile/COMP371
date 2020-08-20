//
// Created by Thomas Buffard on 8/18/20.
//

#ifndef _MESH_M_H_
#define _MESH_M_H_

#include <glm/glm.hpp>

#include "shader_m.h"

#include <string>
#include <vector>


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Color;
};

class Mesh_M {
 public:
	// mesh data
	std::vector<Vertex> vertices;

	// Mesh constructor
	Mesh_M(std::vector<Vertex> vertices);

	// Render the mesh
	void Draw(Shader &shader, GLenum type);

	void Update(std::vector<Vertex> vertices);
 private:
	// render data
	unsigned int VAO, VBO;

	// Initializes the buffers and arrays for rendering
	void setupMesh();
};

#endif //_MESH_M_H_
