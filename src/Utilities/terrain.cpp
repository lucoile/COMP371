//
// Created by Thomas Buffard on 8/5/20.
//

#include "terrain.h"

Terrain::Terrain(unsigned int size, Model cube, int octaveCount, float frequency, float persistence)
{
	this->size = size;
	this->cube = cube;
	this->octaveCount = octaveCount;
	this->frequency = frequency;
	this->persistence = persistence;

	genHeightMap();
}

void Terrain::Render(Shader& shader, glm::mat4 world)
{
	// Loop through height map and render each voxel
	for (unsigned int x = 0; x < size; x++) {
		for (unsigned int z = 0; z < size; z++) {
			// Get height at x, z position from height map
			float height = (heightMap.GetValue(x, z) / 2.0) + 0.5;

			// Calculate position of voxel on x-z plane
			glm::vec3 pos((float) (x - (size / 2.0)) * ULEN, 0.0f, (float) (z - (size / 2.0)) * ULEN);

			// Scale each voxel to the height in height map rounded to the nearest unit
			glm::mat4 model = world *
							  glm::scale(id, glm::vec3(1.0f, 1.0 + round((10.0 * height)), 1.0f)) *
							  glm::translate(id, pos);

			// Render cube with position and height
			shader.setMat4("model", model);
			cube.Draw(shader, GL_TRIANGLES);
		}
	}
}

void Terrain::genHeightMap()
{
	// Set noise parameters
	PerlinGen.SetOctaveCount(octaveCount);
	PerlinGen.SetFrequency(frequency);
	PerlinGen.SetPersistence(persistence);

	// Generate terrain height map using Perlin noise
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinGen);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(-0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();
}
