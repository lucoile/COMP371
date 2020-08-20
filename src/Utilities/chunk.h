//
// Created by Thomas Buffard on 8/17/20.
//

#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "shader_m.h"
#include "voxel.h"
#include "mesh_m.h"
#include "terrain.h"

#include <stdlib.h>

class Chunk
{
 public:
	Chunk(int x, int y, int z);
	~Chunk();

	void Update();

	void Setup_Landscape(Terrain terrain);

	void Render(Shader& shader);

	static const int CHUNK_SIZE = 16;

	glm::vec3 chunkPos;

	bool IsLoaded();
	void Load();
 private:
	unsigned int VBO, VAO;

	// The blocks data
	std::vector<Voxel> m_pVoxels;
	std::vector<Vertex> chunkVertices;
	Mesh_M* voxelMesh;
	Mesh_M* chunkMesh;

	void CreateMesh();
	void CreateCube(float x, float y, float z, VoxelType type);

	bool loaded = false;
};

#endif //_CHUNK_H_
