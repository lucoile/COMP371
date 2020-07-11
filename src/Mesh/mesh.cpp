//
// Created by Thomas Buffard on 7/10/20.
//

#include "mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling) {
    this->vertices = vertices;
    this->indices = indices;
    this->translation = translation;
    this->rotation = rotation;
    this->scaling = scaling;
    this->defaultTranslation = translation;
    this->defaultRotation = rotation;
    this->defaultScaling = scaling;

    setupMesh();
}

void Mesh::scale(float x, float y, float z) {
    scaling = glm::scale(scaling, glm::vec3(1.0f + x, 1.0f + y, 1.0f + z));
}

void Mesh::translate(float x, float y, float z) {
    translation = glm::translate(translation, glm::vec3(x, y, z));
}

void Mesh::rotate(float angle, glm::vec3 vector) {
    rotation = glm::rotate(rotation, glm::radians(angle), vector);
}

void Mesh::draw(Shader &shader, GLenum type) {
    shader.setMat4("translation", translation);
    shader.setMat4("rotation", rotation);
    shader.setMat4("scaling", scaling);

    glBindVertexArray(VAO);
    glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::reset() {
    rotation = defaultRotation;
    translation = defaultTranslation;
    scaling = defaultScaling;
}

void Mesh::deleteBuffers() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float ), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glBindVertexArray(0);
}






