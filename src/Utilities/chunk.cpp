//
// Created by Thomas Buffard on 8/17/20.
//

#include "chunk.h"

Chunk::Chunk()
{
	// Create the voxels
	m_pVoxels = new Voxel**[CHUNK_SIZE];
	for(int i = 0; i < CHUNK_SIZE; i++)
	{
		m_pVoxels[i] = new Voxel*[CHUNK_SIZE];

		for(int j = 0; j < CHUNK_SIZE; j++)
		{
			m_pVoxels[i][j] = new Voxel[CHUNK_SIZE];
		}
	}

	chunkMesh = CreateMesh();
}

Chunk::~Chunk()
{
	// Delete the blocks
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			delete [] m_pVoxels[i][j];
		}

		delete [] m_pVoxels[i];
	}
	delete [] m_pVoxels;
}

Mesh_M* Chunk::CreateMesh()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(m_pVoxels[x][y][z].IsActive() == false)
				{
					// Don't create triangle data for inactive blocks
					continue;
				}

				CreateCube(x, y, z);
			}
		}
	}

	Mesh_M m_pMesh(chunkVertices);
	return &m_pMesh;
}

void Chunk::CreateCube(int x, int y, int z)
{
	glm::vec3 p1(x - Voxel::VOXEL_RENDER_SIZE, y - Voxel::VOXEL_RENDER_SIZE, z + Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p2(x + Voxel::VOXEL_RENDER_SIZE, y - Voxel::VOXEL_RENDER_SIZE, z + Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p3(x + Voxel::VOXEL_RENDER_SIZE, y + Voxel::VOXEL_RENDER_SIZE, z + Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p4(x - Voxel::VOXEL_RENDER_SIZE, y + Voxel::VOXEL_RENDER_SIZE, z + Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p5(x + Voxel::VOXEL_RENDER_SIZE, y - Voxel::VOXEL_RENDER_SIZE, z - Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p6(x - Voxel::VOXEL_RENDER_SIZE, y - Voxel::VOXEL_RENDER_SIZE, z - Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p7(x - Voxel::VOXEL_RENDER_SIZE, y + Voxel::VOXEL_RENDER_SIZE, z - Voxel::VOXEL_RENDER_SIZE);
	glm::vec3 p8(x + Voxel::VOXEL_RENDER_SIZE, y + Voxel::VOXEL_RENDER_SIZE, z - Voxel::VOXEL_RENDER_SIZE);

	glm::vec3 n1(0.0f);

	Vertex v1, v2, v3, v4, v5, v6, v7, v8;

	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	glm::vec4 color(1.0f);

	// Front
	n1 = glm::vec3(0.0f, 0.0f, 1.0f);

	v1 = {p1, n1, color};
	v2 = {p2, n1, color};
	v3 = {p3, n1, color};
	v4 = {p4, n1, color};

	chunkVertices.push_back(v1);
	chunkVertices.push_back(v2);
	chunkVertices.push_back(v3);
	chunkVertices.push_back(v1);
	chunkVertices.push_back(v3);
	chunkVertices.push_back(v4);


	// Back
	n1 = glm::vec3(0.0f, 0.0f, -1.0f);

	v5 = {p5, n1, color};
	v6 = {p6, n1, color};
	v7 = {p7, n1, color};
	v8 = {p8, n1, color};

	chunkVertices.push_back(v5);
	chunkVertices.push_back(v6);
	chunkVertices.push_back(v7);
	chunkVertices.push_back(v5);
	chunkVertices.push_back(v7);
	chunkVertices.push_back(v8);

	// Right
	n1 = glm::vec3(1.0f, 0.0f, 0.0f);

	v2 = {p5, n1, color};
	v5 = {p6, n1, color};
	v8 = {p7, n1, color};
	v3 = {p8, n1, color};

	chunkVertices.push_back(v2);
	chunkVertices.push_back(v5);
	chunkVertices.push_back(v8);
	chunkVertices.push_back(v2);
	chunkVertices.push_back(v8);
	chunkVertices.push_back(v3);

	// left
	n1 = glm::vec3(-1.0f, 0.0f, 0.0f);

	v6 = {p5, n1, color};
	v1 = {p6, n1, color};
	v4 = {p7, n1, color};
	v7 = {p8, n1, color};

	chunkVertices.push_back(v6);
	chunkVertices.push_back(v1);
	chunkVertices.push_back(v4);
	chunkVertices.push_back(v6);
	chunkVertices.push_back(v4);
	chunkVertices.push_back(v7);

	// Top
	n1 = glm::vec3(0.0f, 1.0f, 0.0f);

	v4 = {p5, n1, color};
	v3 = {p6, n1, color};
	v8 = {p7, n1, color};
	v7 = {p8, n1, color};

	chunkVertices.push_back(v4);
	chunkVertices.push_back(v3);
	chunkVertices.push_back(v8);
	chunkVertices.push_back(v4);
	chunkVertices.push_back(v8);
	chunkVertices.push_back(v7);

	// Bottom
	n1 = glm::vec3(0.0f, -1.0f, 0.0f);

	v6 = {p5, n1, color};
	v5 = {p6, n1, color};
	v2 = {p7, n1, color};
	v1 = {p8, n1, color};

	chunkVertices.push_back(v6);
	chunkVertices.push_back(v5);
	chunkVertices.push_back(v2);
	chunkVertices.push_back(v6);
	chunkVertices.push_back(v2);
	chunkVertices.push_back(v1);
}

void Chunk::Render(Shader &shader)
{
	chunkMesh->Draw(shader, GL_TRIANGLES);
}