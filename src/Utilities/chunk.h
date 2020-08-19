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
	Chunk();
	~Chunk();

	void Update();

	void Setup_Landscape(Terrain terrain);

	void Render();

	static const int CHUNK_SIZE = 16;

	bool IsLoaded();
	void Load();
 private:
	// The blocks data
	Voxel*** m_pVoxels;
	std::vector<Vertex> chunkVertices;
	Mesh_M* chunkMesh;

	void CreateMesh();
	void CreateCube(int x, int y, int z);

	bool loaded = false;
};

#endif //_CHUNK_H_
