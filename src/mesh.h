//
// Created by Thomas Buffard on 7/10/20.
//

#ifndef COMP371_MESH_H
#define COMP371_MESH_H

#include <vector>
#include "shader_m.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 scaling;
    glm::mat4 defaultTranslation;
    glm::mat4 defaultRotation;
    glm::mat4 defaultScaling;

    Mesh();

    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling);

    void draw(Shader &shader, GLenum type);

    void translate(float x, float y, float z);

    void rotate(float angle, glm::vec3 vector);

    void scale(float x, float y, float z);

    void reset();

    void deleteBuffers();

private:
    unsigned int VBO, VAO, EBO;

    void setupMesh();
};


#endif //COMP371_MESH_H
