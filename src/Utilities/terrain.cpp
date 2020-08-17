//
// Created by Thomas Buffard on 8/5/20.
//

#include "terrain.h"
#include <cmath>

Terrain::Terrain(unsigned int size, Model cube, Model sphere, int octaveCount, float frequency, float persistence,
                 int renderSize) {
    this->size = size;
    this->cube = cube;
    this->sphere = sphere;
    this->octaveCount = octaveCount;
    this->frequency = frequency;
    this->persistence = persistence;
    this->renderSize = renderSize;

    createR1Model();
    createH6Model();
    createN5Model();
    create08Model();
    createK5Model();
    createTreeModel();

    genHeightMap();
}

void Terrain::Render(Shader &shader, glm::mat4 world, glm::vec2 worldPos) {
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
                               (float) (z - (renderSize / 2.0) - startZ) * ULEN);

            // Scale each voxel to the height in height map rounded to the nearest unit
            glm::mat4 model = world *
                              glm::scale(id, glm::vec3(1.0f, 1.0 + round((10.0 * height)), 1.0f)) *
                              glm::translate(id, voxelPos);

            // Render cube with position and height
            shader.setMat4("model", model);
            cube.Draw(shader, GL_TRIANGLES);

            RenderVegetationAndModels(shader, world, startX, startZ, x, z, height);
        }
    }
}

void Terrain::RenderVegetationAndModels(Shader &shader, const glm::mat4 &world, int startX, int startZ, int x, int z,
                                        float height) {
    module::Perlin perlinModule;

    // Responsible for the generation of a model
    double val = perlinModule.GetValue(x, height, z);

    // Responsible for the generation of which model
    double val2 = perlinModule.GetValue(z, height ,x);

    modelCount = 5;

    if(val2 > 0 && val2 <= 0.5){
        // R1
        modelCount = 0;
    }
    if(val2 > 0.5 && val2 <= 0.10){
        // H6
        modelCount = 1;
    }
    if(val2 > 0.10 && val2 <= 0.15){
        // N5
        modelCount = 2;
    }
    if(val2 > 0.15 && val2 <= 0.20){
        // 08
        modelCount = 3;
    }
    if(val2 > 0.20 && val2 <= 0.25){
        // K5
        modelCount = 4;
    }
    if(val2 > 0.25){
        // Tree
        modelCount = 5;
    }

    bool generateModel = val > 0.999 && height > 0;
    if (generateModel) {
        if (modelCount >= 0 && modelCount < 5) {
            RenderModel(shader, world, startX, startZ, x, z, height);
        }
        if (modelCount == 5) {
            RenderTree(shader, world, startX, startZ, x, z, height);
        }
    }
}

void Terrain::RenderModel(Shader &shader, const glm::mat4 &world, int startX, int startZ, int x, int z, float height) {
    glm::vec3 voxelPos2((float) (x - (renderSize / 2.0) - startX) * ULEN,
            height,
                        (float) (z - (renderSize / 2.0) - startZ) * ULEN);

    // Draw the Letter
    for (unsigned int i = 0; i < models[modelCount].letterTrans.size(); i++) {
        glm::mat4 model =
                world * glm::translate(id, voxelPos2) * models[modelCount].letterTranslation * models[modelCount].rotation *
                models[modelCount].scale * models[modelCount].letterAdjust * models[modelCount].letterRotation * models[modelCount].letterTrans[i];
        shader.setMat4("model", model);

        cube.Draw(shader, GL_TRIANGLES);
    }

    // Draw The Number
    for (unsigned int i = 0; i < models[modelCount].numTrans.size(); i++) {
        glm::mat4 model =
                world * glm::translate(id, voxelPos2) * models[modelCount].numberTranslation * models[modelCount].rotation *
                models[modelCount].scale * models[modelCount].numAdjust * models[modelCount].numberRotation * models[modelCount].numTrans[i];
        shader.setMat4("model", model);

        cube.Draw(shader, GL_TRIANGLES);
    }

    // Draw the Sphere
    glm::mat4 model =
            world * glm::translate(id, voxelPos2) * glm::translate(id, glm::vec3(0, 5.0 * ULEN, 0)) *
            models[modelCount].rotation * models[modelCount].scale * glm::scale(id, glm::vec3(3.0f, 3.0f, 3.0f));
    shader.setMat4("model", model);

    sphere.Draw(shader, GL_TRIANGLES);
}

void Terrain::RenderTree(Shader &shader, const glm::mat4 &world, int startX, int startZ, int x, int z, float height) {
    glm::vec3 voxelPos2((float) (x - (renderSize / 2.0) - startX) * ULEN,
                        height,
                        (float) (z - (renderSize / 2.0) - startZ) * ULEN);

    // Draw the Trunk
    for (unsigned int i = 0; i < models[modelCount].letterTrans.size(); i++) {
        glm::mat4 model =
                world * glm::translate(id, voxelPos2) * models[modelCount].letterTranslation *
                models[modelCount].rotation *
                models[modelCount].scale * models[modelCount].letterAdjust * models[modelCount].letterRotation *
                models[modelCount].letterTrans[i];
        shader.setMat4("model", model);

        cube.Draw(shader, GL_TRIANGLES);
    }

    // Draw the Leaves
    for (unsigned int i = 0; i < models[modelCount].numTrans.size(); i++) {
        glm::mat4 model =
                world * glm::translate(id, voxelPos2) * models[modelCount].numberTranslation * models[modelCount].rotation *
                models[modelCount].scale * models[modelCount].numAdjust * models[modelCount].numberRotation * models[modelCount].numTrans[i];
        shader.setMat4("model", model);

        cube.Draw(shader, GL_TRIANGLES);
    }
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
    heightMapBuilder.SetDestNoiseMap(heightMap);
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
}

void Terrain::createTreeModel(){
    models[5].letterTrans.push_back(
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[5].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0, 5.0f * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(5.0f, 2.0f, 5.0f))
    );
    models[5].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0, 7.0f * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(3.0f, 1.0f, 3.0f))
    );

    models[5].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0, 8.0f * ULEN, 0.0f))
    );

    models[5].scale = id;
    models[5].translation = id;
    models[5].rotation = id;
    models[5].letterTranslation = id;
    models[5].numberTranslation = id;
    models[5].rotationAngle = 0.0f;
    models[5].sphereScale = sphereScale;
    models[5].sphereTranslation = sphereTranslation;
    models[5].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[5].animationTimeValue = 0;
    models[5].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Terrain::createK5Model() {
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    glm::mat4 shearKTop = {
            1.0f, 0.0f, 0.0f, 0.0f,
            2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 3.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearKTop
    );
    glm::mat4 shearKBottom = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f)) * shearKBottom
    );

    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5 * ULEN, 4.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            id
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[4].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[4].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[4].scale = id;
    models[4].translation = glm::translate(id, glm::vec3(20 * ULEN, 0.0f, -20 * ULEN));
    models[4].rotation = id;
    models[4].rotationAngle = 0.0f;
    models[4].letterTranslation = id;
    models[4].numberTranslation = id;
    models[4].sphereScale = sphereScale;
    models[4].sphereTranslation = sphereTranslation;
    models[4].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[4].animationTimeValue = 0;
    models[4].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Terrain::create08Model() {
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].letterTrans.push_back(
            id
    );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );

    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(-1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            id
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );

    models[3].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[3].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[3].scale = id;
    models[3].translation = glm::translate(id, glm::vec3(-20 * ULEN, 0.0f, -20 * ULEN));
    models[3].rotation = id;
    models[3].letterTranslation = id;
    models[3].numberTranslation = id;
    models[3].rotationAngle = 0.0f;
    models[3].sphereScale = sphereScale;
    models[3].sphereTranslation = sphereTranslation;
    models[3].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[3].animationTimeValue = 0;
    models[3].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Terrain::createN5Model() {
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    glm::mat4 shearN = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -3.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)) * shearN
    );

    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5 * ULEN, 4.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            id
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[2].letterAdjust = glm::translate(id, glm::vec3(-2.5 * ULEN, 0.0f, 0.0f));
    models[2].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[2].scale = id;
    models[2].translation = glm::translate(id, glm::vec3(-20 * ULEN, 0.0f, 20 * ULEN));
    models[2].rotation = id;
    models[2].letterTranslation = id;
    models[2].numberTranslation = id;
    models[2].rotationAngle = 0.0f;
    models[2].sphereScale = sphereScale;
    models[2].sphereTranslation = sphereTranslation;
    models[2].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[2].animationTimeValue = 0;
    models[2].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Terrain::createH6Model() {
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2 * ULEN, 0.0f))
    );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );

    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            id
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[1].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[1].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[1].scale = id;
    models[1].translation = glm::translate(id, glm::vec3(20 * ULEN, 0.0f, 20 * ULEN));
    models[1].rotation = id;
    models[1].letterTranslation = id;
    models[1].numberTranslation = id;
    models[1].rotationAngle = 0.0f;
    models[1].sphereScale = sphereScale;
    models[1].sphereTranslation = sphereTranslation;
    models[1].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[1].animationTimeValue = 0;
    models[1].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Terrain::createR1Model() {
    models[0].letterTrans.push_back(
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 4.0f * ULEN, 0.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 2.0 * ULEN, 0.0f))
    );
    glm::mat4 shearR = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -1.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearR
    );

    models[0].numTrans.push_back(glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)));

    models[0].letterAdjust = glm::translate(id, glm::vec3(-3 * ULEN, 0.0f, 0.0f));
    models[0].numAdjust = glm::translate(id, glm::vec3(ULEN, 0.0f, 0.0f));

    models[0].scale = id;
    models[0].translation = id;
    models[0].rotation = id;
    models[0].letterTranslation = id;
    models[0].numberTranslation = id;
    models[0].rotationAngle = 0.0f;
    models[0].sphereScale = sphereScale;
    models[0].sphereTranslation = sphereTranslation;
    models[0].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[0].animationTimeValue = 0;
    models[0].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}







