// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "texture.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <vector>
#include <stdexcept> // std::runtime_error

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void process_input(GLFWwindow *window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::vector<float> read_csv(const std::string &filename);

// Settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
const float ULEN = 0.1f; // Unit Length

// Camera
Camera camera(glm::vec3(0.0f, 0.1f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Render Type
GLenum type = GL_TRIANGLES;
unsigned int selectedModel = 1;

// Translation matrices for models
glm::mat4 defaultTranslations[] = {
        glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, 40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, 40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, -40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, -40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, ULEN, 0.0f))
};

glm::mat4 translations[] = {
        glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, 40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, 40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, -40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, -40 * ULEN)),
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, ULEN, 0.0f))
};

glm::mat4 defaultRotations[] = {
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0))
};

glm::mat4 rotations[] = {
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)),
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0))
};

glm::mat4 defaultScalings[] = {
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0))
};

glm::mat4 scalings[] = {
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)),
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0))
};

glm::mat4 defaultView;

glm::mat4 worldOrientation = glm::mat4(1.0f);


int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW Window Creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COMP371 Project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure Global Opengl State
    glEnable(GL_DEPTH_TEST);

    // Build and Compile our Shader Program
    Shader gridShader("../res/shaders/grid.vert", "../res/shaders/grid.frag");
    Shader lineShader("../res/shaders/line.vert", "../res/shaders/line.frag");
    Shader modelShader("../res/shaders/model.vert", "../res/shaders/model.frag");

    float squareVertices[] = {
//          Vertices            Colors
            0.0f, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f,  // Point 0
            ULEN, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f,  // Point 1
            0.0f, 0.0f, ULEN, 0.0f, 0.407f, 0.478f,  // Point 2
            ULEN, 0.0f, ULEN, 0.0f, 0.407f, 0.478f   // Point 3
    };

    unsigned int squareIndices[] = {
            0, 1,
            0, 2,
            2, 3,
            1, 3,
    };

    // Generate Grid Positions from Center
    glm::vec3 gridPositions[100][100];

    for (int i = -50; i < 50; i++) {
        for (int j = -50; j < 50; j++) {
            gridPositions[i + 50][j + 50] = glm::vec3((float) i / (1 / ULEN), 0.0f, (float) j / (1 / ULEN));
        }
    }

    // Square
    unsigned int squareEBO, squareVAO, squareVBO;
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);
    glGenBuffers(1, &squareEBO);

    glBindVertexArray(squareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float linesVertices[] = {
//          Vertices                  Colors
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,       // red x-axis line
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,       // green y-axis line
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,       // blue z-axis line
            ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f
    };
    unsigned int linesIndices[] = {
            0, 3,   // red x-axis line
            1, 4,   // green y-axis line
            2, 5    // blue z-axis line
    };

    // Lines
    unsigned int lineEBO, lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(linesVertices), linesVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(linesIndices), linesIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//    std::vector<float> modelVAO1 = read_csv("../res/models/alphanumeric_vertices.csv");
//    std::vector<float> modelEBO1 = read_csv("../res/models/alphanumeric_elements.csv");

    // Alphanumberic model data
    float vertH6[] = {0.074075, -0.050000, 0.050000, 0.174075, -0.050000, 0.050000, 0.074075, 0.050000, 0.050000,
                      0.174075, 0.050000, 0.050000, 0.074075, 0.050000, -0.050000, 0.174075, 0.050000, -0.050000,
                      0.074075, -0.050000, -0.050000, 0.174075, -0.050000, -0.050000, 0.274075, -0.050000, -0.050000,
                      0.274075, -0.050000, 0.050000, 0.274075, 0.050000, -0.050000, 0.274075, 0.050000, 0.050000,
                      0.374075, -0.050000, -0.050000, 0.374075, -0.050000, 0.050000, 0.374075, 0.050000, -0.050000,
                      0.374075, 0.050000, 0.050000, 0.074075, 0.150000, 0.050000, 0.174075, 0.150000, 0.050000,
                      0.174075, 0.150000, -0.050000, 0.074075, 0.150000, -0.050000, 0.074075, 0.250000, 0.050000,
                      0.174075, 0.250000, 0.050000, 0.174075, 0.250000, -0.050000, 0.074075, 0.250000, -0.050000,
                      0.074075, 0.350000, 0.050000, 0.174075, 0.350000, 0.050000, 0.174075, 0.350000, -0.050000,
                      0.074075, 0.350000, -0.050000, 0.074075, 0.450000, 0.050000, 0.174075, 0.450000, 0.050000,
                      0.174075, 0.450000, -0.050000, 0.074075, 0.450000, -0.050000, 0.274075, 0.350000, 0.050000,
                      0.274075, 0.350000, -0.050000, 0.274075, 0.450000, -0.050000, 0.274075, 0.450000, 0.050000,
                      0.374075, 0.350000, 0.050000, 0.374075, 0.350000, -0.050000, 0.374075, 0.450000, -0.050000,
                      0.374075, 0.450000, 0.050000, 0.274075, 0.350000, 0.050000, 0.274075, 0.350000, -0.050000,
                      0.374075, 0.350000, -0.050000, 0.374075, 0.350000, 0.050000, 0.274075, 0.150000, 0.050000,
                      0.274075, 0.150000, -0.050000, 0.274075, 0.250000, -0.050000, 0.274075, 0.250000, 0.050000,
                      0.374075, 0.150000, 0.050000, 0.374075, 0.150000, -0.050000, 0.374075, 0.250000, -0.050000,
                      0.374075, 0.250000, 0.050000, -0.495108, -0.050000, 0.050000, -0.395108, -0.050000, 0.050000,
                      -0.495108, 0.250000, 0.050000, -0.395108, 0.150000, 0.050000, -0.495108, 0.250000, -0.050000,
                      -0.395108, 0.150000, -0.050000, -0.495108, -0.050000, -0.050000, -0.395108, -0.050000, -0.050000,
                      -0.395108, 0.250000, 0.050000, -0.395108, 0.250000, -0.050000, -0.495108, 0.450000, 0.050000,
                      -0.395108, 0.450000, 0.050000, -0.395108, 0.450000, -0.050000, -0.495108, 0.450000, -0.050000,
                      -0.095108, 0.150000, 0.050000, -0.095108, 0.150000, -0.050000, -0.195108, 0.250000, 0.050000,
                      -0.195108, 0.250000, -0.050000, -0.195108, 0.150000, -0.050000, -0.195108, 0.150000, 0.050000,
                      -0.195108, 0.450000, 0.050000, -0.195108, 0.450000, -0.050000, -0.095108, 0.450000, 0.050000,
                      -0.095108, 0.450000, -0.050000, -0.195108, -0.050000, -0.050000, -0.195108, -0.050000, 0.050000,
                      -0.095108, -0.050000, -0.050000, -0.095108, -0.050000, 0.050000};
    unsigned int indexH6[] = {0, 1, 2, 2, 1, 3, 28, 29, 31, 31, 29, 30, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 13, 12, 15,
                              15, 12, 14, 6, 0, 4, 4, 0, 2, 1, 7, 9, 9, 7, 8, 7, 5, 8, 8, 5, 10, 5, 3, 10, 10, 3, 11, 3,
                              1, 11, 11, 1, 9, 9, 8, 13, 13, 8, 12, 8, 10, 12, 12, 10, 14, 11, 9, 15, 15, 9, 13, 2, 3,
                              16, 16, 3, 17, 3, 5, 17, 17, 5, 18, 5, 4, 18, 18, 4, 19, 4, 2, 19, 19, 2, 16, 16, 17, 20,
                              20, 17, 21, 48, 49, 51, 51, 49, 50, 18, 19, 22, 22, 19, 23, 19, 16, 23, 23, 16, 20, 20,
                              21, 24, 24, 21, 25, 21, 22, 25, 25, 22, 26, 22, 23, 26, 26, 23, 27, 23, 20, 27, 27, 20,
                              24, 24, 25, 28, 28, 25, 29, 36, 37, 39, 39, 37, 38, 26, 27, 30, 30, 27, 31, 27, 24, 31,
                              31, 24, 28, 25, 26, 32, 32, 26, 33, 26, 30, 33, 33, 30, 34, 30, 29, 34, 34, 29, 35, 29,
                              25, 35, 35, 25, 32, 40, 41, 43, 43, 41, 42, 33, 34, 37, 37, 34, 38, 34, 35, 38, 38, 35,
                              39, 35, 32, 39, 39, 32, 36, 40, 32, 41, 32, 33, 41, 41, 33, 42, 33, 37, 42, 42, 37, 43,
                              37, 36, 43, 43, 36, 40, 36, 32, 40, 17, 18, 44, 44, 18, 45, 18, 22, 45, 45, 22, 46, 22,
                              21, 46, 46, 21, 47, 21, 17, 47, 47, 17, 44, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47,
                              51, 47, 44, 51, 51, 44, 48, 10, 11, 45, 45, 11, 44, 11, 15, 44, 44, 15, 48, 15, 14, 48,
                              48, 14, 49, 14, 10, 49, 49, 10, 45, 53, 55, 52, 52, 55, 54, 62, 63, 65, 65, 63, 64, 56,
                              57, 58, 58, 57, 59, 58, 59, 52, 52, 59, 53, 53, 59, 55, 55, 59, 57, 58, 52, 56, 56, 52,
                              54, 54, 55, 60, 57, 56, 61, 54, 60, 62, 62, 60, 63, 60, 61, 63, 63, 61, 64, 61, 56, 64,
                              64, 56, 65, 56, 54, 65, 65, 54, 62, 73, 72, 75, 75, 72, 74, 70, 69, 67, 77, 76, 79, 79,
                              76, 78, 68, 71, 66, 61, 60, 69, 69, 60, 68, 57, 61, 70, 70, 61, 69, 55, 57, 71, 71, 57,
                              70, 60, 55, 68, 68, 55, 71, 69, 68, 73, 73, 68, 72, 66, 74, 68, 68, 74, 72, 66, 67, 74,
                              74, 67, 75, 67, 69, 75, 75, 69, 73, 71, 70, 77, 77, 70, 76, 70, 67, 76, 76, 67, 78, 67,
                              66, 78, 78, 66, 79, 66, 71, 79, 79, 71, 77};
    float vertJ5[] = {-0.325565, -0.050000, 0.050000, -0.236164, -0.050000, 0.050000, -0.325565, 0.450000, 0.050000, -0.236164, 0.450000, 0.050000, -0.325565, 0.450000, -0.050000, -0.236164, 0.450000, -0.050000, -0.325565, -0.050000, -0.050000, -0.236164, -0.050000, -0.050000, -0.236164, 0.357072, 0.050000, -0.325565, 0.350000, 0.050000, -0.325565, 0.350000, -0.050000, -0.236164, 0.357072, -0.050000, -0.212963, 0.259301, 0.050000, -0.325565, 0.250000, 0.050000, -0.325565, 0.250000, -0.050000, -0.212963, 0.259301, -0.050000, -0.236164, 0.139011, 0.050000, -0.325565, 0.150000, 0.050000, -0.325565, 0.150000, -0.050000, -0.236164, 0.139011, -0.050000, -0.236164, 0.050000, 0.050000, -0.325565, 0.050000, 0.050000, -0.325565, 0.050000, -0.050000, -0.236164, 0.050000, -0.050000, -0.132116, -0.048058, -0.050000, -0.132116, -0.048058, 0.050000, -0.032142, -0.045795, -0.050000, -0.032142, -0.045795, 0.050000, -0.236164, 0.050000, -0.050000, -0.236164, 0.050000, 0.050000, -0.236164, 0.139011, -0.050000, -0.236164, 0.139011, 0.050000, -0.051039, 0.449769, -0.050000, -0.051039, 0.449769, 0.050000, -0.151039, 0.449923, -0.050000, -0.151039, 0.449923, 0.050000, 0.363836, -0.050000, 0.050000, 0.363836, 0.050000, 0.050000, 0.363836, 0.050000, -0.050000, 0.363836, -0.050000, -0.050000, 0.263836, 0.050000, 0.050000, 0.263836, 0.050000, -0.050000, 0.263836, -0.050000, -0.050000, 0.263836, -0.050000, 0.050000, 0.263836, 0.150000, 0.050000, 0.363836, 0.150000, 0.050000, 0.363836, 0.150000, -0.050000, 0.263836, 0.150000, -0.050000, 0.263836, 0.250000, 0.050000, 0.363836, 0.250000, 0.050000, 0.363836, 0.250000, -0.050000, 0.263836, 0.250000, -0.050000, 0.163836, 0.150000, 0.050000, 0.163836, 0.150000, -0.050000, 0.163836, 0.250000, 0.050000, 0.163836, 0.250000, -0.050000, 0.063836, 0.150000, 0.050000, 0.063836, 0.150000, -0.050000, 0.063836, 0.250000, 0.050000, 0.063836, 0.250000, -0.050000, 0.163836, 0.350000, 0.050000, 0.163836, 0.350000, -0.050000, 0.063836, 0.350000, -0.050000, 0.063836, 0.350000, 0.050000, 0.163836, 0.450000, 0.050000, 0.163836, 0.450000, -0.050000, 0.063836, 0.450000, -0.050000, 0.063836, 0.450000, 0.050000, 0.363836, 0.350000, 0.050000, 0.363836, 0.350000, -0.050000, 0.363836, 0.450000, -0.050000, 0.363836, 0.450000, 0.050000, 0.063836, 0.050000, -0.050000, 0.063836, -0.050000, -0.050000, 0.063836, -0.050000, 0.050000, 0.063836, 0.050000, 0.050000};
    unsigned int indexJ5[] = {0, 1, 21, 21, 1, 20, 2, 3, 4, 4, 3, 5, 22, 23, 6, 6, 23, 7, 6, 7, 0, 0, 7, 1, 1, 7, 20, 20, 7, 23, 6, 0, 22, 22, 0, 21, 9, 8, 2, 2, 8, 3, 10, 9, 4, 4, 9, 2, 5, 11, 4, 4, 11, 10, 8, 11, 3, 3, 11, 5, 12, 8, 13, 13, 8, 9, 14, 13, 10, 10, 13, 9, 10, 11, 14, 14, 11, 15, 33, 32, 35, 35, 32, 34, 17, 16, 13, 13, 16, 12, 18, 17, 14, 14, 17, 13, 15, 19, 14, 14, 19, 18, 25, 24, 27, 27, 24, 26, 20, 16, 21, 21, 16, 17, 22, 21, 18, 18, 21, 17, 18, 19, 22, 22, 19, 23, 29, 28, 31, 31, 28, 30, 16, 19, 25, 25, 19, 24, 15, 26, 19, 19, 26, 24, 15, 12, 26, 26, 12, 27, 12, 16, 27, 27, 16, 25, 29, 20, 28, 20, 23, 28, 28, 23, 30, 23, 19, 30, 30, 19, 31, 19, 16, 31, 31, 16, 29, 16, 20, 29, 12, 15, 33, 33, 15, 32, 11, 34, 15, 15, 34, 32, 11, 8, 34, 34, 8, 35, 8, 12, 35, 35, 12, 33, 36, 39, 37, 37, 39, 38, 48, 49, 51, 51, 49, 50, 42, 41, 39, 39, 41, 38, 43, 42, 36, 36, 42, 39, 40, 43, 37, 37, 43, 36, 40, 37, 44, 44, 37, 45, 37, 38, 45, 45, 38, 46, 38, 41, 46, 46, 41, 47, 41, 40, 47, 47, 40, 44, 44, 45, 48, 48, 45, 49, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 57, 56, 59, 59, 56, 58, 47, 44, 53, 53, 44, 52, 44, 48, 52, 52, 48, 54, 48, 51, 54, 54, 51, 55, 51, 47, 55, 55, 47, 53, 53, 52, 57, 57, 52, 56, 52, 54, 56, 56, 54, 58, 64, 65, 67, 67, 65, 66, 55, 53, 59, 59, 53, 57, 54, 55, 60, 60, 55, 61, 55, 59, 61, 61, 59, 62, 59, 58, 62, 62, 58, 63, 58, 54, 63, 63, 54, 60, 68, 69, 71, 71, 69, 70, 61, 62, 65, 65, 62, 66, 62, 63, 66, 66, 63, 67, 63, 60, 67, 67, 60, 64, 60, 61, 68, 68, 61, 69, 61, 65, 69, 69, 65, 70, 65, 64, 70, 70, 64, 71, 64, 60, 71, 71, 60, 68, 72, 73, 75, 75, 73, 74, 41, 42, 72, 72, 42, 73, 42, 43, 73, 73, 43, 74, 43, 40, 74, 74, 40, 75, 40, 41, 75, 75, 41, 72};
    float vertN5[] = {0.100000, -0.050000, 0.050000, 0.200000, -0.050000, 0.050000, 0.100000, 0.090032, 0.050000,
                      0.100000, 0.090032, -0.050000, 0.200000, -0.050000, -0.050000, 0.100000, -0.050000, -0.050000,
                      0.100000, 0.450000, 0.050000, 0.200000, 0.450000, 0.050000, 0.200000, 0.450000, -0.050000,
                      0.100000, 0.450000, -0.050000, -0.277414, -0.050000, 0.050000, -0.177414, -0.050000, 0.050000,
                      -0.277414, 0.450000, 0.050000, -0.177414, 0.301243, 0.050000, -0.277414, 0.450000, -0.050000,
                      -0.177414, 0.301243, -0.050000, -0.277414, -0.050000, -0.050000, -0.177414, -0.050000, -0.050000,
                      -0.177414, 0.450000, 0.050000, -0.177414, 0.450000, -0.050000, 0.650000, -0.050000, 0.050000,
                      0.650000, 0.050000, 0.050000, 0.650000, 0.050000, -0.050000, 0.650000, -0.050000, -0.050000,
                      0.550000, 0.050000, 0.050000, 0.550000, 0.050000, -0.050000, 0.550000, -0.050000, -0.050000,
                      0.550000, -0.050000, 0.050000, 0.550000, 0.150000, 0.050000, 0.650000, 0.150000, 0.050000,
                      0.650000, 0.150000, -0.050000, 0.550000, 0.150000, -0.050000, 0.550000, 0.250000, 0.050000,
                      0.650000, 0.250000, 0.050000, 0.650000, 0.250000, -0.050000, 0.550000, 0.250000, -0.050000,
                      0.450000, 0.150000, 0.050000, 0.450000, 0.150000, -0.050000, 0.450000, 0.250000, 0.050000,
                      0.450000, 0.250000, -0.050000, 0.350000, 0.150000, 0.050000, 0.350000, 0.150000, -0.050000,
                      0.350000, 0.250000, 0.050000, 0.350000, 0.250000, -0.050000, 0.450000, 0.350000, 0.050000,
                      0.450000, 0.350000, -0.050000, 0.350000, 0.350000, -0.050000, 0.350000, 0.350000, 0.050000,
                      0.450000, 0.450000, 0.050000, 0.450000, 0.450000, -0.050000, 0.350000, 0.450000, -0.050000,
                      0.350000, 0.450000, 0.050000, 0.650000, 0.350000, 0.050000, 0.650000, 0.350000, -0.050000,
                      0.650000, 0.450000, -0.050000, 0.650000, 0.450000, 0.050000, 0.350000, 0.050000, -0.050000,
                      0.350000, -0.050000, -0.050000, 0.350000, -0.050000, 0.050000, 0.350000, 0.050000, 0.050000};
    unsigned int indexN5[] = {0, 1, 2, 6, 7, 9, 9, 7, 8, 3, 4, 5, 5, 4, 0, 0, 4, 1, 1, 7, 2, 2, 7, 6, 1, 4, 7, 7, 4, 8,
                              4, 3, 8, 8, 3, 9, 3, 2, 9, 9, 2, 6, 11, 13, 10, 10, 13, 12, 12, 18, 14, 14, 18, 19, 14,
                              15, 16, 16, 15, 17, 16, 17, 10, 10, 17, 11, 11, 17, 13, 13, 17, 15, 16, 10, 14, 14, 10,
                              12, 12, 13, 18, 15, 14, 19, 2, 3, 18, 18, 3, 19, 5, 15, 3, 3, 15, 19, 5, 0, 15, 15, 0, 13,
                              0, 2, 13, 13, 2, 18, 20, 23, 21, 21, 23, 22, 32, 33, 35, 35, 33, 34, 26, 25, 23, 23, 25,
                              22, 27, 26, 20, 20, 26, 23, 24, 27, 21, 21, 27, 20, 24, 21, 28, 28, 21, 29, 21, 22, 29,
                              29, 22, 30, 22, 25, 30, 30, 25, 31, 25, 24, 31, 31, 24, 28, 28, 29, 32, 32, 29, 33, 29,
                              30, 33, 33, 30, 34, 30, 31, 34, 34, 31, 35, 41, 40, 43, 43, 40, 42, 31, 28, 37, 37, 28,
                              36, 28, 32, 36, 36, 32, 38, 32, 35, 38, 38, 35, 39, 35, 31, 39, 39, 31, 37, 37, 36, 41,
                              41, 36, 40, 36, 38, 40, 40, 38, 42, 48, 49, 51, 51, 49, 50, 39, 37, 43, 43, 37, 41, 38,
                              39, 44, 44, 39, 45, 39, 43, 45, 45, 43, 46, 43, 42, 46, 46, 42, 47, 42, 38, 47, 47, 38,
                              44, 52, 53, 55, 55, 53, 54, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 47, 44, 51,
                              51, 44, 48, 44, 45, 52, 52, 45, 53, 45, 49, 53, 53, 49, 54, 49, 48, 54, 54, 48, 55, 48,
                              44, 55, 55, 44, 52, 56, 57, 59, 59, 57, 58, 25, 26, 56, 56, 26, 57, 26, 27, 57, 57, 27,
                              58, 27, 24, 58, 58, 24, 59, 24, 25, 59, 59, 25, 56};
    float vertO8[] = {0.050000, -0.050000, 0.050000, 0.350000, -0.050000, 0.050000, 0.050000, 0.050000, 0.050000,
                      0.350000, 0.050000, 0.050000, 0.050000, 0.050000, -0.050000, 0.350000, 0.050000, -0.050000,
                      0.050000, -0.050000, -0.050000, 0.350000, -0.050000, -0.050000, 0.250000, 0.050000, 0.050000,
                      0.250000, 0.050000, -0.050000, 0.250000, -0.050000, -0.050000, 0.250000, -0.050000, 0.050000,
                      0.150000, 0.050000, 0.050000, 0.150000, 0.050000, -0.050000, 0.150000, -0.050000, -0.050000,
                      0.150000, -0.050000, 0.050000, 0.050000, 0.150000, 0.050000, 0.150000, 0.150000, 0.050000,
                      0.150000, 0.150000, -0.050000, 0.050000, 0.150000, -0.050000, 0.250000, 0.150000, 0.050000,
                      0.350000, 0.150000, 0.050000, 0.350000, 0.150000, -0.050000, 0.250000, 0.150000, -0.050000,
                      0.050000, 0.250000, 0.050000, 0.150000, 0.250000, 0.050000, 0.150000, 0.250000, -0.050000,
                      0.050000, 0.250000, -0.050000, 0.250000, 0.250000, 0.050000, 0.350000, 0.250000, 0.050000,
                      0.350000, 0.250000, -0.050000, 0.250000, 0.250000, -0.050000, 0.050000, 0.350000, 0.050000,
                      0.150000, 0.350000, 0.050000, 0.150000, 0.350000, -0.050000, 0.050000, 0.350000, -0.050000,
                      0.250000, 0.350000, 0.050000, 0.350000, 0.350000, 0.050000, 0.350000, 0.350000, -0.050000,
                      0.250000, 0.350000, -0.050000, 0.050000, 0.450000, 0.050000, 0.150000, 0.450000, 0.050000,
                      0.150000, 0.450000, -0.050000, 0.050000, 0.450000, -0.050000, 0.250000, 0.450000, 0.050000,
                      0.350000, 0.450000, 0.050000, 0.350000, 0.450000, -0.050000, 0.250000, 0.450000, -0.050000,
                      -0.050000, -0.050000, 0.050000, -0.050000, 0.050000, 0.050000, -0.050000, 0.050000, -0.050000,
                      -0.050000, -0.050000, -0.050000, -0.150000, 0.050000, 0.050000, -0.150000, 0.050000, -0.050000,
                      -0.150000, -0.050000, -0.050000, -0.150000, -0.050000, 0.050000, -0.150000, 0.350000, 0.050000,
                      -0.050000, 0.350000, 0.050000, -0.050000, 0.350000, -0.050000, -0.150000, 0.350000, -0.050000,
                      -0.150000, 0.450000, 0.050000, -0.050000, 0.450000, 0.050000, -0.050000, 0.450000, -0.050000,
                      -0.150000, 0.450000, -0.050000, -0.350000, 0.350000, 0.050000, -0.350000, 0.350000, -0.050000,
                      -0.350000, 0.450000, 0.050000, -0.350000, 0.450000, -0.050000, -0.350000, 0.050000, -0.050000,
                      -0.350000, -0.050000, -0.050000, -0.350000, -0.050000, 0.050000, -0.350000, 0.050000, 0.050000,
                      -0.450000, 0.350000, 0.050000, -0.450000, 0.350000, -0.050000, -0.450000, 0.450000, 0.050000,
                      -0.450000, 0.450000, -0.050000, -0.450000, 0.050000, -0.050000, -0.450000, -0.050000, -0.050000,
                      -0.450000, -0.050000, 0.050000, -0.450000, 0.050000, 0.050000};
    unsigned int indexO8[] = {0, 15, 2, 2, 15, 12, 40, 41, 43, 43, 41, 42, 4, 13, 6, 6, 13, 14, 6, 14, 0, 0, 14, 15, 1,
                              7, 3, 3, 7, 5, 6, 0, 4, 4, 0, 2, 44, 45, 47, 47, 45, 46, 10, 9, 7, 7, 9, 5, 11, 10, 1, 1,
                              10, 7, 8, 11, 3, 3, 11, 1, 12, 8, 13, 13, 8, 9, 14, 13, 10, 10, 13, 9, 15, 14, 11, 11, 14,
                              10, 12, 15, 8, 8, 15, 11, 2, 12, 16, 16, 12, 17, 12, 13, 17, 17, 13, 18, 13, 4, 18, 18, 4,
                              19, 4, 2, 19, 19, 2, 16, 8, 3, 20, 20, 3, 21, 3, 5, 21, 21, 5, 22, 5, 9, 22, 22, 9, 23, 9,
                              8, 23, 23, 8, 20, 16, 17, 24, 24, 17, 25, 18, 19, 26, 26, 19, 27, 19, 16, 27, 27, 16, 24,
                              20, 21, 28, 28, 21, 29, 21, 22, 29, 29, 22, 30, 22, 23, 30, 30, 23, 31, 24, 25, 32, 32,
                              25, 33, 25, 26, 33, 33, 26, 34, 26, 27, 34, 34, 27, 35, 27, 24, 35, 35, 24, 32, 28, 29,
                              36, 36, 29, 37, 29, 30, 37, 37, 30, 38, 30, 31, 38, 38, 31, 39, 31, 28, 39, 39, 28, 36,
                              18, 26, 23, 23, 26, 31, 26, 25, 31, 31, 25, 28, 25, 17, 28, 28, 17, 20, 17, 18, 20, 20,
                              18, 23, 32, 33, 40, 40, 33, 41, 34, 35, 42, 42, 35, 43, 35, 32, 43, 43, 32, 40, 36, 37,
                              44, 44, 37, 45, 37, 38, 45, 45, 38, 46, 38, 39, 46, 46, 39, 47, 34, 42, 39, 39, 42, 47,
                              42, 41, 47, 47, 41, 44, 41, 33, 44, 44, 33, 36, 33, 34, 36, 36, 34, 39, 48, 51, 49, 49,
                              51, 50, 60, 61, 63, 63, 61, 62, 54, 53, 51, 51, 53, 50, 55, 54, 48, 48, 54, 51, 52, 55,
                              49, 49, 55, 48, 56, 57, 60, 60, 57, 61, 57, 58, 61, 61, 58, 62, 58, 59, 62, 62, 59, 63,
                              50, 53, 58, 58, 53, 59, 53, 52, 59, 59, 52, 56, 52, 49, 56, 56, 49, 57, 49, 50, 57, 57,
                              50, 58, 73, 72, 75, 75, 72, 74, 76, 77, 79, 79, 77, 78, 59, 56, 65, 65, 56, 64, 56, 60,
                              64, 64, 60, 66, 60, 63, 66, 66, 63, 67, 63, 59, 67, 67, 59, 65, 53, 54, 68, 68, 54, 69,
                              54, 55, 69, 69, 55, 70, 55, 52, 70, 70, 52, 71, 52, 53, 71, 71, 53, 68, 64, 66, 72, 72,
                              66, 74, 66, 67, 74, 74, 67, 75, 67, 65, 75, 75, 65, 73, 68, 69, 76, 76, 69, 77, 69, 70,
                              77, 77, 70, 78, 70, 71, 78, 78, 71, 79, 65, 64, 68, 68, 64, 71, 64, 72, 71, 71, 72, 79,
                              72, 73, 79, 79, 73, 76, 73, 65, 76, 76, 65, 68};
    float vertR1[] = {0.051719, -0.050000, 0.050000, 0.151719, -0.050000, 0.050000, 0.051719, 0.450000, 0.050000,
                      0.151719, 0.450000, 0.050000, 0.051719, 0.450000, -0.050000, 0.151719, 0.450000, -0.050000,
                      0.051719, -0.050000, -0.050000, 0.151719, -0.050000, -0.050000, -0.348031, -0.050000, 0.050000,
                      -0.248031, -0.050000, 0.050000, -0.348031, 0.450000, 0.050000, -0.248031, 0.450000, 0.050000,
                      -0.348031, 0.450000, -0.050000, -0.248031, 0.450000, -0.050000, -0.348031, -0.050000, -0.050000,
                      -0.248031, -0.050000, -0.050000, -0.248031, 0.350000, 0.050000, -0.348031, 0.350000, 0.050000,
                      -0.348031, 0.350000, -0.050000, -0.248031, 0.350000, -0.050000, -0.248031, 0.250000, 0.050000,
                      -0.348031, 0.250000, 0.050000, -0.348031, 0.250000, -0.050000, -0.248031, 0.250000, -0.050000,
                      -0.248031, 0.108034, 0.050000, -0.348031, 0.150000, 0.050000, -0.348031, 0.150000, -0.050000,
                      -0.248031, 0.108034, -0.050000, -0.248031, 0.050000, 0.050000, -0.348031, 0.050000, 0.050000,
                      -0.348031, 0.050000, -0.050000, -0.248031, 0.050000, -0.050000, -0.148031, 0.350000, -0.050000,
                      -0.148031, 0.350000, 0.050000, -0.148031, 0.450000, -0.050000, -0.148031, 0.450000, 0.050000,
                      -0.048031, 0.350000, -0.050000, -0.048031, 0.350000, 0.050000, -0.048031, 0.450000, -0.050000,
                      -0.048031, 0.450000, 0.050000, -0.148031, 0.250000, -0.050000, -0.148031, 0.250000, 0.050000,
                      -0.048031, 0.250000, -0.050000, -0.048031, 0.250000, 0.050000, -0.156174, 0.150000, -0.050000,
                      -0.156174, 0.150000, 0.050000, -0.048031, 0.150000, -0.050000, -0.048031, 0.150000, 0.050000,
                      -0.162415, -0.050000, -0.050000, -0.162415, -0.050000, 0.050000, -0.043567, -0.050000, -0.050000,
                      -0.043567, -0.050000, 0.050000};
    unsigned int indexR1[] = {0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 1, 7, 3, 3, 7, 5,
                              6, 0, 4, 4, 0, 2, 8, 9, 29, 29, 9, 28, 10, 11, 12, 12, 11, 13, 30, 31, 14, 14, 31, 15, 14,
                              15, 8, 8, 15, 9, 9, 15, 28, 28, 15, 31, 14, 8, 30, 30, 8, 29, 17, 16, 10, 10, 16, 11, 18,
                              17, 12, 12, 17, 10, 12, 13, 18, 18, 13, 19, 37, 36, 39, 39, 36, 38, 21, 20, 17, 17, 20,
                              16, 22, 21, 18, 18, 21, 17, 18, 19, 22, 22, 19, 23, 20, 23, 16, 16, 23, 19, 24, 20, 25,
                              25, 20, 21, 26, 25, 22, 22, 25, 21, 22, 23, 26, 26, 23, 27, 28, 24, 29, 29, 24, 25, 30,
                              29, 26, 26, 29, 25, 26, 27, 30, 30, 27, 31, 28, 31, 24, 24, 31, 27, 16, 19, 33, 33, 19,
                              32, 19, 13, 32, 32, 13, 34, 13, 11, 34, 34, 11, 35, 11, 16, 35, 35, 16, 33, 45, 44, 47,
                              47, 44, 46, 32, 34, 36, 36, 34, 38, 34, 35, 38, 38, 35, 39, 35, 33, 39, 39, 33, 37, 33,
                              32, 41, 41, 32, 40, 32, 36, 40, 40, 36, 42, 36, 37, 42, 42, 37, 43, 37, 33, 43, 43, 33,
                              41, 42, 46, 40, 40, 46, 44, 42, 43, 46, 46, 43, 47, 43, 41, 47, 47, 41, 45, 27, 23, 44,
                              44, 23, 40, 23, 20, 40, 40, 20, 41, 24, 45, 20, 20, 45, 41, 49, 48, 51, 51, 48, 50, 24,
                              27, 49, 49, 27, 48, 27, 44, 48, 48, 44, 50, 44, 45, 50, 50, 45, 51, 24, 49, 45, 45, 49,
                              51};

    float *modelVertices[5] = {
            vertH6, vertJ5, vertN5, vertO8, vertR1
    };
    unsigned int *modelIndices[5] = {
            indexH6, indexJ5, indexN5, indexO8, indexR1
    };

    // Models
//    unsigned int modelEBO[5], modelVAO[5], modelVBO[5];
//    glGenVertexArrays(5, modelVAO);
//    glGenBuffers(5, modelVBO);
//    glGenBuffers(5, modelEBO);

    unsigned int modelH6EBO, modelH6VAO, modelH6VBO;
    unsigned int modelJ5EBO, modelJ5VAO, modelJ5VBO;
    unsigned int modelN5EBO, modelN5VAO, modelN5VBO;
    unsigned int modelO8EBO, modelO8VAO, modelO8VBO;
    unsigned int modelR1EBO, modelR1VAO, modelR1VBO;

    glGenVertexArrays(1, &modelH6VAO);
    glGenBuffers(1, &modelH6VBO);
    glGenBuffers(1, &modelH6EBO);

    glGenVertexArrays(1, &modelJ5VAO);
    glGenBuffers(1, &modelJ5VBO);
    glGenBuffers(1, &modelJ5EBO);

    glGenVertexArrays(1, &modelN5VAO);
    glGenBuffers(1, &modelN5VBO);
    glGenBuffers(1, &modelN5EBO);

    glGenVertexArrays(1, &modelO8VAO);
    glGenBuffers(1, &modelO8VBO);
    glGenBuffers(1, &modelO8EBO);

    glGenVertexArrays(1, &modelR1VAO);
    glGenBuffers(1, &modelR1VBO);
    glGenBuffers(1, &modelR1EBO);


//    for (int i = 0; i < 5; i++) {
//        glBindVertexArray(modelVAO[i]);
//        glBindBuffer(GL_ARRAY_BUFFER, modelVBO[i]);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertices[i]), modelVertices[i], GL_STATIC_DRAW);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEBO[i]);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndices[i]), modelIndices[i], GL_STATIC_DRAW);
//
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
//        glEnableVertexAttribArray(0);
//    }

    glBindVertexArray(modelH6VAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelH6VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertH6), vertH6, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelH6EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexH6), indexH6, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(modelJ5VAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelJ5VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertJ5), vertJ5, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelJ5EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexJ5), indexJ5, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(modelN5VAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelN5VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertN5), vertN5, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelN5EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexN5), indexN5, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(modelO8VAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelO8VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertO8), vertO8, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelO8EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexO8), indexO8, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(modelR1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelR1VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertR1), vertR1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelR1EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexR1), indexR1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Load and Create a texture
    Texture texture1("res/textures/dirt.jpg");

    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    gridShader.use();
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Per Frame Time Logic
        auto currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        process_input(window);

        // Render
        glClearColor(0.0f, 0.0784f, 0.1607f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(45.0f, (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);

        // Camera/view transformation
        glm::mat4 view = camera.get_view_matrix();

        // Render lines
        // Activate line shader
        lineShader.use();
        lineShader.setMat4("projection", projection);
        lineShader.setMat4("view", view);

        // Draw lines
        glBindVertexArray(lineVAO);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render grid
        // Activate shader
        gridShader.use();
        gridShader.setMat4("projection", projection);
        gridShader.setMat4("view", view);
        gridShader.setMat4("world", worldOrientation);

        glBindVertexArray(squareVAO);
        for (auto &gridPosition : gridPositions) {
            for (auto &j : gridPosition) {
                glm::mat4 model = glm::mat4(1.0f); //Use Identity Matrix to bring back to original
                model = glm::translate(model, j);
                gridShader.setMat4("model", model);

                glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
            }
        }
        glBindVertexArray(0);

        // Render models
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("world", worldOrientation);

        modelShader.setMat4("translation", translations[0]);
        modelShader.setMat4("rotation", rotations[0]);
        modelShader.setMat4("scaling", scalings[0]);
//        modelVertices = translationMatrix * modelVertices;

        glBindVertexArray(modelH6VAO);
        glDrawElements(type, sizeof(vertH6), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        modelShader.setMat4("translation", translations[1]);
        modelShader.setMat4("rotation", rotations[1]);
        modelShader.setMat4("scaling", scalings[1]);

        glBindVertexArray(modelJ5VAO);
        glDrawElements(type, sizeof(vertJ5), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        modelShader.setMat4("translation", translations[2]);
        modelShader.setMat4("rotation", rotations[2]);
        modelShader.setMat4("scaling", scalings[2]);

        glBindVertexArray(modelN5VAO);
        glDrawElements(type, sizeof(vertN5), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        modelShader.setMat4("translation", translations[3]);
        modelShader.setMat4("rotation", rotations[3]);
        modelShader.setMat4("scaling", scalings[3]);

        glBindVertexArray(modelO8VAO);
        glDrawElements(type, sizeof(vertO8), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        modelShader.setMat4("translation", translations[4]);
        modelShader.setMat4("rotation", rotations[4]);
        modelShader.setMat4("scaling", scalings[4]);

        glBindVertexArray(modelR1VAO);
        glDrawElements(type, sizeof(vertR1), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteBuffers(1, &lineEBO);
    glDeleteBuffers(1, &squareEBO);
    glDeleteBuffers(1, &squareVAO);

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

std::vector<float> read_csv(const std::string &filename) {
    using namespace std;
    vector<string> models;
    string model;
    vector<float> result;
    string line, element, temp;

    // input filestream
    ifstream myFile(filename);

    // open file
    if (!myFile.is_open()) throw runtime_error("Error: File Was Not Opened");

    while (getline(myFile, model, ',')) {
        if (model == "\n") {
            cout << "new line";
        }
        cout << model;
        result.push_back(std::stof(model));
    }

    myFile.close();
    return result;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void process_input(GLFWwindow *window) {

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    auto xoffset = float(xpos - lastX);
    auto yoffset = float(lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = float(xpos);
    lastY = float(ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        camera.process_mouse_movement(xoffset, 0, PAN);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.process_mouse_movement(0, yoffset, ZOOM);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        camera.process_mouse_movement(0, yoffset, TILT);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        type = GL_POINTS;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        type = GL_LINES;
    } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        type = GL_TRIANGLES;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        selectedModel = 0;
        std::cout << "Model 0 Selected" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        selectedModel = 1;
        std::cout << "Model 1 Selected" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        selectedModel = 2;
        std::cout << "Model 2 Selected" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        selectedModel = 3;
        std::cout << "Model 3 Selected" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        selectedModel = 4;
        std::cout << "Model 4 Selected" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        //TODO: Rotate around x axis positively
        worldOrientation = glm::rotate(worldOrientation, glm::radians(1.0f), glm::vec3(ULEN, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        //TODO: Rotate around x axis negatively
        worldOrientation = glm::rotate(worldOrientation, glm::radians(-1.0f), glm::vec3(ULEN, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        //TODO: Rotate around y axis positively
        worldOrientation = glm::rotate(worldOrientation, glm::radians(1.0f), glm::vec3(0.0f, ULEN, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        //TODO: Rotate around y axis negatively
        worldOrientation = glm::rotate(worldOrientation, glm::radians(-1.0f), glm::vec3(0.0f, ULEN, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        for(int i = 0; i < 5; i++){
            translations[i] = defaultTranslations[i];
            rotations[i] = defaultRotations[i];
            scalings[i] = defaultScalings[i];
            worldOrientation = glm::mat4(1.0f);
        }
        camera = Camera(glm::vec3(0.0f, 0.1f, 2.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        scalings[selectedModel] = glm::scale(scalings[selectedModel], glm::vec3(1.0f + ULEN, 1.0f + ULEN, 1.0f + ULEN));
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        scalings[selectedModel] = glm::scale(scalings[selectedModel], glm::vec3(1.0f - ULEN, 1.0f - ULEN, 1.0f - ULEN));
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        translations[selectedModel] =  glm::translate(translations[selectedModel], glm::vec3(0, 0 ,-ULEN));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        translations[selectedModel] =  glm::translate(translations[selectedModel], glm::vec3(-ULEN, 0 ,0));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        translations[selectedModel] =  glm::translate(translations[selectedModel], glm::vec3(0, 0 ,ULEN));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        translations[selectedModel] =  glm::translate(translations[selectedModel], glm::vec3(ULEN, 0 ,0));
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        rotations[selectedModel] = glm::rotate(rotations[selectedModel], glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        rotations[selectedModel] = glm::rotate(rotations[selectedModel], glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
    }
}