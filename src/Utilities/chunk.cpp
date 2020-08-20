//
// Created by Thomas Buffard on 8/17/20.
//

#include "chunk.h"

Chunk::Chunk(int x, int y, int z)
{
	chunkPos = glm::vec3(x, y, z);

	// Create the voxels
	for(int i = 0; i < CHUNK_SIZE; i++)
	{
		for(int j = 0; j < CHUNK_SIZE; j++)
		{
			for(int k = 0; k < CHUNK_SIZE; k++)
			{
				Voxel voxel;
				m_pVoxels.push_back(voxel);
			}
		}
	}

	CreateCube();
}

Chunk::~Chunk()
{
	//TODO: Delete the voxels
//	for (int i = 0; i < CHUNK_SIZE; ++i)
//	{
//		for (int j = 0; j < CHUNK_SIZE; ++j)
//		{
//			delete [] m_pVoxels[i][j];
//		}
//
//		delete [] m_pVoxels[i];
//	}
//	delete [] m_pVoxels;
}

void Chunk::CreateMesh()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].IsActive() == false)
				{
					// Don't create triangle data for inactive blocks
					continue;
				}

//				CreateCube();
			}
		}
	}

//	chunkMesh = new Mesh_M(chunkVertices);
}

void Chunk::CreateCube()
{
	float vertex_data[] = {
		// Cube
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};


	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// Bind VAO
	glBindVertexArray(VAO);
	// Bind VBO and copy vertices into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Chunk::Render(Shader& shader)
{
	// TODO: set model transformation using chunk position
	float xOffset = chunkPos.x * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;
	float yOffset = chunkPos.y * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;
	float zOffset = chunkPos.z * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].IsActive() == false)
				{
					// Don't create triangle data for inactive blocks
					//	std::cout << x << " " << y << " " << z << "\n";
					glm::vec3 position(
						(x * Voxel::VOXEL_RENDER_SIZE) + xOffset,
						(y * Voxel::VOXEL_RENDER_SIZE) + yOffset,
						(z * Voxel::VOXEL_RENDER_SIZE) + zOffset);
					std::cout << position.x << " " << position.y << " " << position.z << "\n";
					glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
					shader.setMat4("model", model);
					glBindVertexArray(VBO);
					glDrawArrays(GL_TRIANGLES, 0, 36);
					continue;
				}
			}
		}
	}
}

void Chunk::Update()
{
	chunkVertices.clear();

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].IsActive() == false)
				{
					// Don't create triangle data for inactive blocks
					continue;
				}

				CreateCube();
			}
		}
	}

//	chunkMesh->Update(chunkVertices);
}

bool Chunk::IsLoaded()
{
	return loaded;
}

void Chunk::Load()
{
	loaded = true;
}

void Chunk::Setup_Landscape(Terrain terrain)
{
	// TODO: use chunk position to get height
	float* heightMap = terrain.heightMap;

	int xOffset = chunkPos.x;
	int yOffset = chunkPos.y;
	int zOffset = chunkPos.z;

	for(int x = 0; x < CHUNK_SIZE; x++)
	{
		for(int z = 0; z < CHUNK_SIZE; z++)
		{
			// Use the height map texture to get the height value of x, z
			float height = heightMap[(x + xOffset) * 1000 + (z + zOffset)];

			for (int y = 0; y < height; y++)
			{
				m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].SetActive(true);
//				m_pVoxels[x][y][z].SetBlockType(BlockType_Grass);
			}

			for (int y = height + 1; y < CHUNK_SIZE; y++)
			{
				m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].SetActive(false);
//				m_pVoxels[x][y][z].SetBlockType(BlockType_Grass);
			}
		}
	}
}
