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

	CreateMesh();
}

Chunk::~Chunk()
{
	chunkVertices.clear();
	m_pVoxels.clear();
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

				float voxelAdjust = Voxel::VOXEL_RENDER_SIZE * Voxel::VOXEL_RENDER_SIZE;
                CreateCube(x * voxelAdjust, y * voxelAdjust, z * voxelAdjust, m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].getType());
			}
		}
	}

	chunkMesh = new Mesh_M(chunkVertices);
}

void Chunk::CreateCube(float x, float y, float z, VoxelType type)
{
//	std::cout << x << " " << y << " " << z << "\n";

	float voxelAdjust = Voxel::VOXEL_RENDER_SIZE * Voxel::VOXEL_RENDER_SIZE / 2;

	glm::vec3 p0(x + voxelAdjust, y + voxelAdjust, z + voxelAdjust);
	glm::vec3 p1(x - voxelAdjust, y + voxelAdjust, z + voxelAdjust);
	glm::vec3 p2(x - voxelAdjust, y - voxelAdjust, z + voxelAdjust);
	glm::vec3 p3(x + voxelAdjust, y - voxelAdjust, z + voxelAdjust);
	glm::vec3 p4(x + voxelAdjust, y - voxelAdjust, z - voxelAdjust);
	glm::vec3 p5(x + voxelAdjust, y + voxelAdjust, z - voxelAdjust);
	glm::vec3 p6(x - voxelAdjust, y + voxelAdjust, z - voxelAdjust);
	glm::vec3 p7(x - voxelAdjust, y - voxelAdjust, z - voxelAdjust);

	// set voxel color
    glm::vec4 color(1.0f,1.0f,1.0f, 1.0f);

    if(type == VoxelType_Wood)
    {
        color = glm::vec4(0.50f, 0.30f, 0.25f, 1.0f);
    }
    if(type == VoxelType_Leaves)
    {
        color = glm::vec4( 0.5f, 1.0f, 0.0f, 1.0f);
    }

	// Front
	glm::vec3 n1 = glm::vec3(0.0f, 0.0f, 1.0f);

	chunkVertices.push_back({p0, n1, color});	// triangle 1
	chunkVertices.push_back({p1, n1, color});
	chunkVertices.push_back({p2, n1, color});
	chunkVertices.push_back({p2, n1, color});	// triangle 2
	chunkVertices.push_back({p3, n1, color});
	chunkVertices.push_back({p0, n1, color});


	// Back
	n1 = glm::vec3(0.0f, 0.0f, -1.0f);

	chunkVertices.push_back({p4, n1, color});	// triangle 1
	chunkVertices.push_back({p7, n1, color});
	chunkVertices.push_back({p6, n1, color});
	chunkVertices.push_back({p6, n1, color});	// triangle 2
	chunkVertices.push_back({p5, n1, color});
	chunkVertices.push_back({p4, n1, color});

	// Right
	n1 = glm::vec3(1.0f, 0.0f, 0.0f);

	chunkVertices.push_back({p0, n1, color});	// triangle 1
	chunkVertices.push_back({p3, n1, color});
	chunkVertices.push_back({p4, n1, color});
	chunkVertices.push_back({p4, n1, color});	// triangle 2
	chunkVertices.push_back({p5, n1, color});
	chunkVertices.push_back({p0, n1, color});

	// left
	n1 = glm::vec3(-1.0f, 0.0f, 0.0f);

	chunkVertices.push_back({p1, n1, color});	// triangle 1
	chunkVertices.push_back({p6, n1, color});
	chunkVertices.push_back({p7, n1, color});
	chunkVertices.push_back({p7, n1, color});	// triangle 2
	chunkVertices.push_back({p2, n1, color});
	chunkVertices.push_back({p1, n1, color});

    // Bottom
    n1 = glm::vec3(0.0f, -1.0f, 0.0f);

    chunkVertices.push_back({p7, n1, color});	// triangle 1
    chunkVertices.push_back({p4, n1, color});
    chunkVertices.push_back({p3, n1, color});
    chunkVertices.push_back({p3, n1, color});	// triangle 2
    chunkVertices.push_back({p2, n1, color});
    chunkVertices.push_back({p7, n1, color});

	// Top
	n1 = glm::vec3(0.0f, 1.0f, 0.0f);

    if(type == VoxelType_Grass){
        color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
    }

	chunkVertices.push_back({p0, n1, color});	// triangle 1
	chunkVertices.push_back({p5, n1, color});
	chunkVertices.push_back({p6, n1, color});
	chunkVertices.push_back({p6, n1, color});	// triangle 2
	chunkVertices.push_back({p1, n1, color});
	chunkVertices.push_back({p0, n1, color});
}

void Chunk::Render(Shader& shader)
{
	// TODO: set model transformation using chunk position
	float xOffset = chunkPos.x * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;
	float yOffset = chunkPos.y * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;
	float zOffset = chunkPos.z * CHUNK_SIZE * Voxel::VOXEL_RENDER_SIZE;
//	std::cout << xOffset << " " << yOffset << " " << zOffset << "\n";

	glm::vec3 position( xOffset, yOffset, zOffset);
//	std::cout << position.x << " " << position.y << " " << position.z << "\n";
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position) *
		(glm::scale(glm::mat4(1.0f), glm::vec3(1 / Voxel::VOXEL_RENDER_SIZE)));
	shader.setMat4("model", model);
	chunkMesh->Draw();
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

				float voxelAdjust = Voxel::VOXEL_RENDER_SIZE * Voxel::VOXEL_RENDER_SIZE;
                CreateCube(x * voxelAdjust, y * voxelAdjust, z * voxelAdjust, m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].getType());
			}
		}
	}

	chunkMesh = new Mesh_M(chunkVertices);
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
    module::Perlin perlinModule;
	float* heightMap = terrain.heightMap;
	float* vegetationMap = terrain.vegetationMap;

	int xOffset = chunkPos.x * CHUNK_SIZE;
	int yOffset = chunkPos.y * CHUNK_SIZE;
	int zOffset = chunkPos.z * CHUNK_SIZE;

	for(int x = 0; x < CHUNK_SIZE; x++)
	{
		for(int z = 0; z < CHUNK_SIZE; z++)
		{
			// Use the height map texture to get the height value of x, z
			int height = round(heightMap[(x + xOffset) * 1000 + (z + zOffset)] * 10.0);
            float vegetationHeight = round(vegetationMap[(x + xOffset) * 1000 + (z + zOffset)] * 10.0);
//			std::cout << yOffset << "\n";

			for (int y = (yOffset); y < height; y++)
			{
			    double val = perlinModule.GetValue(x, vegetationHeight, z);
                m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].SetActive(true);
			    if((y - height) == -1)
			    {// set top layer to grass
                    m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].SetType(VoxelType_Grass);

					// random num to determine whether or not trees should be generated
//					int rdm = rand() % 100; // random number between 0 and 99
                    std::cout << "value: "<< val << " height: " << vegetationHeight << " x: " << x << " y: " << y << " z: " << z << "\n";
					if(val > 0.999 && (x < 15) && (y < 12) && (z < 15)) {
						// Trunk
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 1) * CHUNK_SIZE) + z].SetActive(true);
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 1) * CHUNK_SIZE) + z].SetType(VoxelType_Wood);
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 2) * CHUNK_SIZE) + z].SetActive(true);
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 2) * CHUNK_SIZE) + z].SetType(VoxelType_Wood);
						// Leaves
						for(int i = 3; i < 6; i++)
						{
							for(int j = -1; j <= 1; j++)
							{
								if(j == 0)
								{
									m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + z].SetActive(true);
									m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + z].SetType(VoxelType_Wood);
								}
								int rdm = rand() % 100;
								if(rdm < 80)
								{
									m_pVoxels[((x + j) * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + (z)].SetActive(true);
									m_pVoxels[((x + j) * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + (z)].SetType(VoxelType_Leaves);

									m_pVoxels[((x) * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + (z + j)].SetActive(true);
									m_pVoxels[((x) * CHUNK_SIZE * CHUNK_SIZE) + ((y + i) * CHUNK_SIZE) + (z + j)].SetType(VoxelType_Leaves);
								}
							}

						}
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 6) * CHUNK_SIZE) + z].SetActive(true);
						m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + ((y + 6) * CHUNK_SIZE) + z].SetType(VoxelType_Leaves);
					}
			    }
			    else
			    {
			        m_pVoxels[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z].SetType(VoxelType_Dirt);
			    }
			}
		}
	}

	Update();
}
