//
// Created by Thomas Buffard on 8/18/20.
//

#ifndef _CHUNK_MANAGER_H_
#define _CHUNK_MANAGER_H_

#include "chunk.h"

class ChunkManager
{
 public:
	ChunkManager();

	void Update(float dt, glm::vec3 cameraPosition, glm::vec3 cameraView);

	void RenderChunks();
 private:
	static const int NUM_CHUNKS = 64;
	static const int ASYNC_NUM_CHUNKS_PER_FRAME = 8;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;

	std::vector<Chunk*> m_vpChunkList;
	std::vector<Chunk*> m_vpChunkLoadList;
	std::vector<Chunk*> m_vpChunkRenderList;
	std::vector<Chunk*> m_vpChunkUnloadList;
	std::vector<Chunk*> m_vpChunkVisibilityList;
	std::vector<Chunk*> m_vpChunkSetupList;

	void UpdateChunkList();
	void UpdateRenderList();
};

#endif //_CHUNK_MANAGER_H_
