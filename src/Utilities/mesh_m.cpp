//
// Created by Thomas Buffard on 8/18/20.
//

#include "mesh_m.h"

Mesh_M::Mesh_M(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
//	std::cout << vertices[0].Position.x;
	setupMesh();
}

void Mesh_M::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
	glBindVertexArray(0);
}

void Mesh_M::setupMesh()
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

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

void Mesh_M::Update(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	setupMesh();
}

