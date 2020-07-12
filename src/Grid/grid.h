//
// Created by Daniel Batista-Ferreira on 7/11/20.
//

#ifndef COMP371_GRID_H
#define COMP371_GRID_H

#include <vector>
#include "../Utilities/shader_m.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Grid {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 gridPositions[100][100];

    Grid(std::vector<float> vertices, std::vector<unsigned int> indices);

    void draw(Shader &shader);

    void deleteBuffers();

private:
    unsigned int VBO, VAO, EBO;

    void setupGrid();
};


#endif //COMP371_GRID_H
