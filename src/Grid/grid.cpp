//
// Created by Daniel Batista-Ferreira on 7/11/20.
//

#include "grid.h"

const float ULEN = 0.1f; // Unit Length

Grid::Grid(std::vector<float> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;

    for (int i = -50; i < 50; i++) {
        for (int j = -50; j < 50; j++) {
            gridPositions[i + 50][j + 50] = glm::vec3((float) i / (1 / ULEN), 0.0f, (float) j / (1 / ULEN));
        }
    }

    setupGrid();
}

void Grid::draw(Shader &shader) {
    glBindVertexArray(VAO);
    for (auto &gridPosition : gridPositions) {
        for (auto &j : gridPosition) {
            glm::mat4 model = glm::mat4(1.0f); //Use Identity Matrix to bring back to original
            model = glm::translate(model, j);
            shader.setMat4("model", model);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
        }
    }
    glBindVertexArray(0);
}

void Grid::deleteBuffers() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void Grid::setupGrid() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

}
