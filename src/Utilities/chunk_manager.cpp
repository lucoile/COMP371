//
// Created by Thomas Buffard on 8/18/20.
//

#include "chunk_manager.h"

ChunkManager::ChunkManager()
{
	Terrain terrain;
	for(int i = 0; i < NUM_CHUNKS; i++)
	{
		Chunk* pChunk = new Chunk();
		pChunk->Setup_Landscape(terrain);
		m_vpChunkList.push_back(pChunk);
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

void ChunkManager::RenderChunks()
{
	std::vector<Chunk*>::iterator iterator;

	for (iterator = m_vpChunkList.begin(); iterator != m_vpChunkList.end(); ++iterator)
	{
		Chunk* pChunk = (*iterator);

		pChunk->Render();
	}
}
