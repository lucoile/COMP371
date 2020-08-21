//
// Created by Thomas Buffard on 8/21/20.
//

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "voxel.h"
#include "glm/gtc/matrix_transform.hpp"
#include "chunk_manager.h"

enum Movement_Direction
{
	P_FORWARD,
	P_BACKWARD,
	P_LEFT,
	P_RIGHT
};

class Player
{
 public:
	glm::vec3 Position;
	glm::vec3 Rotation;

	ChunkManager* m_ChunkManager;
	Terrain* m_Terrain;

	Player(ChunkManager* ChunkManager, Terrain* Terrain);

	void Render(Shader& shader);

	void process_keyboard_input(Movement_Direction direction, float deltaTime);

 private:
	unsigned int VBO, VAO;
	std::vector<Vertex> pVertices;

	Mesh_M* pMesh;

	void CreateMesh();
};

#endif //_PLAYER_H_
