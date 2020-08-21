//
// Created by Thomas Buffard on 8/18/20.
//

#include "chunk_manager.h"

ChunkManager::ChunkManager()
{
	Terrain terrain;
	for(int x = 0; x < NUM_CHUNKS; x++)
	{
		for(int y = 0; y < NUM_CHUNKS; y++)
		{
			for(int z = 0; z < NUM_CHUNKS; z++)
			{
				// TODO: add terrain map to chunk constructor
				Chunk* pChunk = new Chunk(x, y, z);
				pChunk->Setup_Landscape(terrain);
				m_vpChunkList.push_back(pChunk);
			}
		}
	}
}

void ChunkManager::Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView)
{
	UpdateChunkList();

	m_cameraPosition = cameraPosition;
	m_cameraView = cameraView;
}

void ChunkManager::UpdateChunkList()
{
	std::vector<Chunk*>::iterator iterator;

	for(iterator = m_vpChunkLoadList.begin(); iterator != m_vpChunkLoadList.end(); ++iterator)
	{
		Chunk* pChunk = (*iterator);

		if(pChunk->IsLoaded() == false)
		{
			pChunk->Load();
		}
	}

	// Clear the load list (every frame)
	m_vpChunkLoadList.clear();
}

void ChunkManager::RenderChunks(Shader& shader)
{
	std::vector<Chunk*>::iterator iterator;

	shader.setMat4("world", glm::translate(glm::mat4(1.0f),
		glm::vec3(-(NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE,
			0.0f, -(NUM_CHUNKS / 2) * Chunk::CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE)));

	for (iterator = m_vpChunkList.begin(); iterator != m_vpChunkList.end(); ++iterator)
	{
		Chunk* pChunk = (*iterator);

		pChunk->Render(shader);
	}
}

bool ChunkManager::IsActive(int x, int y, int z)
{
	// Get the x, y, z coordinates of the chunk the voxel is located in
	int xChunk = int(x / Chunk::CHUNK_SIZE);
	int yChunk = int(y / Chunk::CHUNK_SIZE);
	int zChunk = int(z / Chunk::CHUNK_SIZE);


	// Get the x, y, z coordinates of the voxel relative to the chunk
	int xVoxel = x - (Chunk::CHUNK_SIZE * xChunk);
	int yVoxel = y - (Chunk::CHUNK_SIZE * yChunk);
	int zVoxel = z - (Chunk::CHUNK_SIZE * zChunk);

//	std::cout << xChunk << " " << yChunk << " " << zChunk << "\t";
//	std::cout << xVoxel << " " << yVoxel << " " << zVoxel << "\n";

	return m_vpChunkList[(xChunk * NUM_CHUNKS * NUM_CHUNKS) + (yChunk * NUM_CHUNKS) + zChunk]->IsActive(xVoxel, yVoxel, zVoxel);
}