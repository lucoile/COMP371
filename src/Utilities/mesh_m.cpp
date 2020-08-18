//
// Created by Thomas Buffard on 8/18/20.
//

#include "mesh_m.h"

Mesh_M::Mesh_M(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
	setupMesh();
}

void Mesh_M::Draw(Shader &shader, GLenum type)
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, VAO, sizeof(vertices));
	glBindVertexArray(0);
}

void Mesh_M::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Color));

	glBindVertexArray(0);
}