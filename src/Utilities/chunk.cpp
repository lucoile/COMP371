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

void Chunk::CreateMesh()
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

	unsigned int v1, v2, v3, v4, v5, v6, v7, v8;

	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	// Front
	n1 = Vector3d(0.0f, 0.0f, 1.0f);

	v1 = m_pRenderer->AddVertexToMesh(m_meshID, p1, n1, r, g, b, a);
	v2 = m_pRenderer->AddVertexToMesh(m_meshID, p2, n1, r, g, b, a);
	v3 = m_pRenderer->AddVertexToMesh(m_meshID, p3, n1, r, g, b, a);
	v4 = m_pRenderer->AddVertexToMesh(m_meshID, p4, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v1, v2, v3);
	m_pRenderer->AddTriangleToMesh(m_meshID, v1, v3, v4);


	// Back
	n1 = Vector3d(0.0f, 0.0f, -1.0f);

	v5 = m_pRenderer->AddVertexToMesh(m_meshID, p5, n1, r, g, b, a);
	v6 = m_pRenderer->AddVertexToMesh(m_meshID, p6, n1, r, g, b, a);
	v7 = m_pRenderer->AddVertexToMesh(m_meshID, p7, n1, r, g, b, a);
	v8 = m_pRenderer->AddVertexToMesh(m_meshID, p8, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v5, v6, v7);
	m_pRenderer->AddTriangleToMesh(m_meshID, v5, v7, v8);

	// Right
	n1 = Vector3d(1.0f, 0.0f, 0.0f);

	v2 = m_pRenderer->AddVertexToMesh(m_meshID, p2, n1, r, g, b, a);
	v5 = m_pRenderer->AddVertexToMesh(m_meshID, p5, n1, r, g, b, a);
	v8 = m_pRenderer->AddVertexToMesh(m_meshID, p8, n1, r, g, b, a);
	v3 = m_pRenderer->AddVertexToMesh(m_meshID, p3, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v2, v5, v8);
	m_pRenderer->AddTriangleToMesh(m_meshID, v2, v8, v3);

	// left
	n1 = Vector3d(-1.0f, 0.0f, 0.0f);

	v6 = m_pRenderer->AddVertexToMesh(m_meshID, p6, n1, r, g, b, a);
	v1 = m_pRenderer->AddVertexToMesh(m_meshID, p1, n1, r, g, b, a);
	v4 = m_pRenderer->AddVertexToMesh(m_meshID, p4, n1, r, g, b, a);
	v7 = m_pRenderer->AddVertexToMesh(m_meshID, p7, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v6, v1, v4);
	m_pRenderer->AddTriangleToMesh(m_meshID, v6, v4, v7);

	// Top
	n1 = Vector3d(0.0f, 1.0f, 0.0f);

	v4 = m_pRenderer->AddVertexToMesh(m_meshID, p4, n1, r, g, b, a);
	v3 = m_pRenderer->AddVertexToMesh(m_meshID, p3, n1, r, g, b, a);
	v8 = m_pRenderer->AddVertexToMesh(m_meshID, p8, n1, r, g, b, a);
	v7 = m_pRenderer->AddVertexToMesh(m_meshID, p7, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v4, v3, v8);
	m_pRenderer->AddTriangleToMesh(m_meshID, v4, v8, v7);

	// Bottom
	n1 = Vector3d(0.0f, -1.0f, 0.0f);

	v6 = m_pRenderer->AddVertexToMesh(m_meshID, p6, n1, r, g, b, a);
	v5 = m_pRenderer->AddVertexToMesh(m_meshID, p5, n1, r, g, b, a);
	v2 = m_pRenderer->AddVertexToMesh(m_meshID, p2, n1, r, g, b, a);
	v1 = m_pRenderer->AddVertexToMesh(m_meshID, p1, n1, r, g, b, a);

	m_pRenderer->AddTriangleToMesh(m_meshID, v6, v5, v2);
	m_pRenderer->AddTriangleToMesh(m_meshID, v6, v2, v1);
}