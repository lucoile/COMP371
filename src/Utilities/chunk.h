//
// Created by Thomas Buffard on 8/17/20.
//

#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "shader_m.h"
#include "voxel.h"
#include "mesh_m.h"

class Chunk
{
 public:
	Chunk();
	~Chunk();

	void Update();

	void Render(Shader &shader);

	static const int CHUNK_SIZE = 16;

 private:
	// The blocks data
	Voxel*** m_pVoxels;
	std::vector<Vertex> chunkVertices;
	Mesh_M* chunkMesh;

	Mesh_M* CreateMesh();
	void CreateCube(int x, int y, int z);
};

#endif //_CHUNK_H_
