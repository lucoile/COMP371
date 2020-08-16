//
// Created by Thomas Buffard on 8/5/20.
//

#include "terrain.h"


Terrain::Terrain(unsigned int size, Model cube, int octaveCount, float frequency, float persistence, int renderSize)
{
	this->size = size;
	this->cube = cube;
	this->octaveCount = octaveCount;
	this->frequency = frequency;
	this->persistence = persistence;
	this->renderSize = renderSize;

	genHeightMap();
}

void Terrain::Render(Shader& shader, glm::mat4 world, glm::vec2 worldPos)
{
	int startX = worldPos.x + round(size / 2);
	int endX = renderSize + worldPos.x + round(size / 2);
	int startZ = worldPos.y + round(size / 2);
	int endZ = renderSize + worldPos.y + round(size / 2);

  for (int x = startX; x < endX; x++) {
	  for (int z = startZ; z < endZ; z++) {
          // Get height value by combining the two height maps
          // using a scale factor of 2.0f for the second
		  float height = ((heightMap.GetValue(x, z) / 2.0) + 0.5) *
		  				(((heightMap2.GetValue(x, z) / 2.0) + 0.5) * 2.0f);

		  // Calculate position of voxel on x-z plane
		  glm::vec3 voxelPos((float) (x - (renderSize / 2.0) - startX) * ULEN,
							 0.0f,
							 (float) (z - (renderSize  / 2.0) - startZ) * ULEN);

		  // Scale each voxel to the height in height map rounded to the nearest unit
		  glm::mat4 model = world *
							glm::scale(id, glm::vec3(1.0f, 1.0 + round((10.0 * height)), 1.0f)) *
							glm::translate(id, voxelPos);

		  // Render cube with position and height
		  shader.setMat4("model", model);
		  cube.Draw(shader, GL_TRIANGLES);
		}
	}
}

void Terrain::genHeightMap()
{
	// libnoise Perlin height map generator
	// Set noise parameters
	PerlinGen.SetOctaveCount(octaveCount);
	PerlinGen.SetFrequency(frequency);
	PerlinGen.SetPersistence(persistence);

	// Generate first terrain height map using Perlin noise
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinGen);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();

	PerlinGen.SetOctaveCount(octaveCount);
	PerlinGen.SetFrequency(frequency/2.0f);
	PerlinGen.SetPersistence(persistence * 1.5f);

  	// Generate second terrain height map using Perlin noise
  	heightMapBuilder.SetSourceModule(PerlinGen);
	heightMapBuilder.SetDestNoiseMap(heightMap2);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();
}








