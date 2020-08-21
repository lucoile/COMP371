//
// Created by Thomas Buffard on 8/21/20.
//

#include "player.h"

Player::Player(ChunkManager* ChunkManager, Terrain* Terrain)
{
	this->m_ChunkManager = ChunkManager;
	this->m_Terrain = Terrain;

	Position = glm::vec3(0.0f, (round(m_Terrain->heightMap[(ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE]) + 2.0f) * Voxel::VOXEL_RENDER_SIZE, 0.0f);
	Rotation = glm::vec3(0.0f);
	CreateMesh();
}

void Player::Render(Shader& shader)
{
//	std::cout << position.x << " " << position.y << " " << position.z << "\n";
//	shader.setMat4("world", glm::translate(glm::mat4(1.0f),
//		glm::vec3(-(ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE,
//			0.0f, -(ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE)));

	shader.setMat4("world", glm::mat4(1.0f));

	glm::mat4 model = glm::translate(glm::mat4(1.0f), Position) *
					  glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f, 0.5f));
	shader.setMat4("model", model);

//	std::cout << Position.x << " " << Position.y << " " << Position.z << "\n";


	pMesh->Draw();
}

void Player::CreateMesh()
{
	float voxelAdjust = Voxel::VOXEL_RENDER_SIZE / 2;

	int x = 0;
	int y = 0;
	int z = 0;

	glm::vec3 p0(x + voxelAdjust, y + voxelAdjust, z + voxelAdjust);
	glm::vec3 p1(x - voxelAdjust, y + voxelAdjust, z + voxelAdjust);
	glm::vec3 p2(x - voxelAdjust, y - voxelAdjust, z + voxelAdjust);
	glm::vec3 p3(x + voxelAdjust, y - voxelAdjust, z + voxelAdjust);
	glm::vec3 p4(x + voxelAdjust, y - voxelAdjust, z - voxelAdjust);
	glm::vec3 p5(x + voxelAdjust, y + voxelAdjust, z - voxelAdjust);
	glm::vec3 p6(x - voxelAdjust, y + voxelAdjust, z - voxelAdjust);
	glm::vec3 p7(x - voxelAdjust, y - voxelAdjust, z - voxelAdjust);

	// set voxel color
	glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);

	// Front
	glm::vec3 n1 = glm::vec3(0.0f, 0.0f, 1.0f);

	pVertices.push_back({ p0, n1, color});	// triangle 1
	pVertices.push_back({ p1, n1, color});
	pVertices.push_back({ p2, n1, color});
	pVertices.push_back({ p2, n1, color});	// triangle 2
	pVertices.push_back({ p3, n1, color});
	pVertices.push_back({ p0, n1, color});


	// Back
	n1 = glm::vec3(0.0f, 0.0f, -1.0f);

	pVertices.push_back({ p4, n1, color});	// triangle 1
	pVertices.push_back({ p7, n1, color});
	pVertices.push_back({ p6, n1, color});
	pVertices.push_back({ p6, n1, color});	// triangle 2
	pVertices.push_back({ p5, n1, color});
	pVertices.push_back({ p4, n1, color});

	// Right
	n1 = glm::vec3(1.0f, 0.0f, 0.0f);

	pVertices.push_back({ p0, n1, color});	// triangle 1
	pVertices.push_back({ p3, n1, color});
	pVertices.push_back({ p4, n1, color});
	pVertices.push_back({ p4, n1, color});	// triangle 2
	pVertices.push_back({ p5, n1, color});
	pVertices.push_back({ p0, n1, color});

	// left
	n1 = glm::vec3(-1.0f, 0.0f, 0.0f);

	pVertices.push_back({ p1, n1, color});	// triangle 1
	pVertices.push_back({ p6, n1, color});
	pVertices.push_back({ p7, n1, color});
	pVertices.push_back({ p7, n1, color});	// triangle 2
	pVertices.push_back({ p2, n1, color});
	pVertices.push_back({ p1, n1, color});

	// Bottom
	n1 = glm::vec3(0.0f, -1.0f, 0.0f);

	pVertices.push_back({ p7, n1, color});	// triangle 1
	pVertices.push_back({ p4, n1, color});
	pVertices.push_back({ p3, n1, color});
	pVertices.push_back({ p3, n1, color});	// triangle 2
	pVertices.push_back({ p2, n1, color});
	pVertices.push_back({ p7, n1, color});

	// Top
	n1 = glm::vec3(0.0f, 1.0f, 0.0f);

	pVertices.push_back({ p0, n1, color});	// triangle 1
	pVertices.push_back({ p5, n1, color});
	pVertices.push_back({ p6, n1, color});
	pVertices.push_back({ p6, n1, color});	// triangle 2
	pVertices.push_back({ p1, n1, color});
	pVertices.push_back({ p0, n1, color});

	pMesh = new Mesh_M(pVertices);
}
void Player::process_keyboard_input(Movement_Direction direction, float deltaTime)
{
	if (direction == P_FORWARD) {
//		Position += Front * velocity;

		glm::vec3 newPosition = Position + (glm::vec3(Voxel::VOXEL_RENDER_SIZE, 0.0f, 0.0f) * deltaTime);
		int xVoxel = int((newPosition.x * 10.0) + ((ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE));
		int xVoxelPos = int((Position.x * 10.0) + ((ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE));
		int yVoxelPos = int((Position.y * 10.0) + ((ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE));
		int zVoxel = int((newPosition.z * 10.0) + ((ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE));
		int zVoxelPos = int((Position.z * 10.0) + ((ChunkManager::NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE));
//		std::cout << xVoxel << " " << yVoxelPos << " " << zVoxel << "\n";

		if(m_ChunkManager->IsActive(xVoxel, yVoxelPos, zVoxel) == true)
			Position = newPosition;
	}
}

