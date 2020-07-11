//
// Created by Thomas Buffard on 7/10/20.
//

#ifndef COMP371_MESH_H
#define COMP371_MESH_H

#include <vector>
#include "shader_m.h"

class Mesh {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);

    void draw(Shader &shader);

    void deleteBuffers();

private:
    unsigned int VBO, VAO, EBO;

    void setupMesh();
};


#endif //COMP371_MESH_H
