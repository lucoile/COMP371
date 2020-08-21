//
// Created by Thomas Buffard on 8/5/20.
//

#include "terrain.h"
#include <cmath>

Terrain::Terrain(unsigned int size, int octaveCount, float frequency, float persistence,
                 int renderSize) {
    this->size = size;
    this->octaveCount = octaveCount;
    this->frequency = frequency;
    this->persistence = persistence;
    this->renderSize = renderSize;

    heightMap = new float[size * size];
    vegetationMap = new int[size * size];

    genHeightMap();
}

void Terrain::genHeightMap() {
	// libnoise Perlin height map generator
	// Set noise parameters
	PerlinGen.SetOctaveCount(octaveCount);
	PerlinGen.SetFrequency(frequency);
	PerlinGen.SetPersistence(persistence);

	// Generate first terrain height map using Perlin noise
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinGen);
	heightMapBuilder.SetDestNoiseMap(heightMap1);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();

	PerlinGen.SetOctaveCount(octaveCount);
	PerlinGen.SetFrequency(frequency / 2.0f);
	PerlinGen.SetPersistence(persistence * 1.5f);

	// Generate second terrain height map using Perlin noise
	heightMapBuilder.SetSourceModule(PerlinGen);
	heightMapBuilder.SetDestNoiseMap(heightMap2);
	heightMapBuilder.SetDestSize(size, size);
	heightMapBuilder.SetBounds(0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			heightMap[i * size + j] = (float) (((heightMap1.GetValue(i, j) / 2.0) + 0.5) *
				(((heightMap2.GetValue(i, j) / 2.0) + 0.5) * 2.0f));

            vegetationMap[i * size + j] = (float) ((heightMap1.GetValue(i, j) / 2.0) + 0.5);
		}
	}

	heightMap1.ReclaimMem();
	heightMap2.ReclaimMem();
}