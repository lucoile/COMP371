//
// Created by Thomas Buffard on 8/5/20.
//

#ifndef COMP371_TERRAIN_H
#define COMP371_TERRAIN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#include "shader_m.h"
#include "model.h"

#include "../../lib/libnoise/include/noise/noise.h"
#include "../../lib/libnoise/include/noise/noiseutils.h"


class Terrain
{
public:
	// Terrain height map
	noise::utils::NoiseMap heightMap;
	// Terrain size
	unsigned int size;
	unsigned int renderSize = 100;

	// Terrain class constructor
	Terrain(unsigned int size, Model cube, int octaveCount = 6, float frequency = 1.0, float persistence = 0.5, int renderSize = 100);

	// Render terrain
	void Render(Shader &shader, glm::mat4 world, glm::vec2 worldPos);

private:
	// Terrain unit size
	float ULEN = 0.1f;
	// Identity matrix
	glm::mat4 id = glm::mat4(1.0f);

	// Perlin noise generation module from libnoise
	noise::module::Perlin PerlinGen;

	// Perlin noise parameters
	int octaveCount;
	float frequency;
	float persistence;

	// Cube model used for voxels
	Model cube = Model("../../../res/models/cube/cube.obj");

	// Height map generation function
	void genHeightMap();
};


#endif //COMP371_TERRAIN_H
