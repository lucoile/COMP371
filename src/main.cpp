// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utilities/shader_m.h"
#include "Utilities/camera.h"
#include "Utilities/texture.h"
#include "Mesh/mesh.h"
#include "Grid/grid.h"
#include "Line/line.h"

#include <iostream>
#include <istream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void process_input(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

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

// Selected Model
unsigned int selectedModel = 0;

// Models
Mesh modelMeshes[5];

// World Orientation
glm::mat4 worldOrientation = glm::mat4(1.0f);

int main() {
    // GLFW: Initialize and configure
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

    std::vector<float> vertGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f,
                                  ULEN, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f,
                                  0.0f, 0.0f, ULEN, 0.0f, 0.407f, 0.478f,
                                  ULEN, 0.0f, ULEN, 0.0f, 0.407f, 0.478f};
    std::vector<unsigned int> indexGrid = { 0, 1,
                                          0, 2,
                                          2, 3,
                                          1, 3};
    Grid grid(vertGrid, indexGrid);

    std::vector<float> vertLines = {
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f
    };
    std::vector<unsigned int> indexLines = {
            0, 3,   // red x-axis line
            1, 4,   // green y-axis line
            2, 5    // blue z-axis line
    };

    Line line(vertLines, indexLines);

    // Alphanumberic model data
    std::vector<float> vertH6 = {0.074075, -0.050000, 0.050000, 0.174075, -0.050000, 0.050000, 0.074075, 0.050000, 0.050000, 0.174075, 0.050000, 0.050000, 0.074075, 0.050000, -0.050000, 0.174075, 0.050000, -0.050000, 0.074075, -0.050000, -0.050000, 0.174075, -0.050000, -0.050000, 0.274075, -0.050000, -0.050000, 0.274075, -0.050000, 0.050000, 0.274075, 0.050000, -0.050000, 0.274075, 0.050000, 0.050000, 0.374075, -0.050000, -0.050000, 0.374075, -0.050000, 0.050000, 0.374075, 0.050000, -0.050000, 0.374075, 0.050000, 0.050000, 0.074075, 0.150000, 0.050000, 0.174075, 0.150000, 0.050000, 0.174075, 0.150000, -0.050000, 0.074075, 0.150000, -0.050000, 0.074075, 0.250000, 0.050000, 0.174075, 0.250000, 0.050000, 0.174075, 0.250000, -0.050000, 0.074075, 0.250000, -0.050000, 0.074075, 0.350000, 0.050000, 0.174075, 0.350000, 0.050000, 0.174075, 0.350000, -0.050000, 0.074075, 0.350000, -0.050000, 0.074075, 0.450000, 0.050000, 0.174075, 0.450000, 0.050000, 0.174075, 0.450000, -0.050000, 0.074075, 0.450000, -0.050000, 0.274075, 0.350000, 0.050000, 0.274075, 0.350000, -0.050000, 0.274075, 0.450000, -0.050000, 0.274075, 0.450000, 0.050000, 0.374075, 0.350000, 0.050000, 0.374075, 0.350000, -0.050000, 0.374075, 0.450000, -0.050000, 0.374075, 0.450000, 0.050000, 0.274075, 0.350000, 0.050000, 0.274075, 0.350000, -0.050000, 0.374075, 0.350000, -0.050000, 0.374075, 0.350000, 0.050000, 0.274075, 0.150000, 0.050000, 0.274075, 0.150000, -0.050000, 0.274075, 0.250000, -0.050000, 0.274075, 0.250000, 0.050000, 0.374075, 0.150000, 0.050000, 0.374075, 0.150000, -0.050000, 0.374075, 0.250000, -0.050000, 0.374075, 0.250000, 0.050000, -0.495108, -0.050000, 0.050000, -0.395108, -0.050000, 0.050000, -0.495108, 0.250000, 0.050000, -0.395108, 0.150000, 0.050000, -0.495108, 0.250000, -0.050000, -0.395108, 0.150000, -0.050000, -0.495108, -0.050000, -0.050000, -0.395108, -0.050000, -0.050000, -0.395108, 0.250000, 0.050000, -0.395108, 0.250000, -0.050000, -0.495108, 0.450000, 0.050000, -0.395108, 0.450000, 0.050000, -0.395108, 0.450000, -0.050000, -0.495108, 0.450000, -0.050000, -0.095108, 0.150000, 0.050000, -0.095108, 0.150000, -0.050000, -0.195108, 0.250000, 0.050000, -0.195108, 0.250000, -0.050000, -0.195108, 0.150000, -0.050000, -0.195108, 0.150000, 0.050000, -0.195108, 0.450000, 0.050000, -0.195108, 0.450000, -0.050000, -0.095108, 0.450000, 0.050000, -0.095108, 0.450000, -0.050000, -0.195108, -0.050000, -0.050000, -0.195108, -0.050000, 0.050000, -0.095108, -0.050000, -0.050000, -0.095108, -0.050000, 0.050000};
    std::vector<unsigned int> indexH6 = {0, 1, 2, 2, 1, 3, 28, 29, 31, 31, 29, 30, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 13, 12, 15, 15, 12, 14, 6, 0, 4, 4, 0, 2, 1, 7, 9, 9, 7, 8, 7, 5, 8, 8, 5, 10, 5, 3, 10, 10, 3, 11, 3, 1, 11, 11, 1, 9, 9, 8, 13, 13, 8, 12, 8, 10, 12, 12, 10, 14, 11, 9, 15, 15, 9, 13, 2, 3, 16, 16, 3, 17, 3, 5, 17, 17, 5, 18, 5, 4, 18, 18, 4, 19, 4, 2, 19, 19, 2, 16, 16, 17, 20, 20, 17, 21, 48, 49, 51, 51, 49, 50, 18, 19, 22, 22, 19, 23, 19, 16, 23, 23, 16, 20, 20, 21, 24, 24, 21, 25, 21, 22, 25, 25, 22, 26, 22, 23, 26, 26, 23, 27, 23, 20, 27, 27, 20, 24, 24, 25, 28, 28, 25, 29, 36, 37, 39, 39, 37, 38, 26, 27, 30, 30, 27, 31, 27, 24, 31, 31, 24, 28, 25, 26, 32, 32, 26, 33, 26, 30, 33, 33, 30, 34, 30, 29, 34, 34, 29, 35, 29, 25, 35, 35, 25, 32, 40, 41, 43, 43, 41, 42, 33, 34, 37, 37, 34, 38, 34, 35, 38, 38, 35, 39, 35, 32, 39, 39, 32, 36, 40, 32, 41, 32, 33, 41, 41, 33, 42, 33, 37, 42, 42, 37, 43, 37, 36, 43, 43, 36, 40, 36, 32, 40, 17, 18, 44, 44, 18, 45, 18, 22, 45, 45, 22, 46, 22, 21, 46, 46, 21, 47, 21, 17, 47, 47, 17, 44, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 47, 44, 51, 51, 44, 48, 10, 11, 45, 45, 11, 44, 11, 15, 44, 44, 15, 48, 15, 14, 48, 48, 14, 49, 14, 10, 49, 49, 10, 45, 53, 55, 52, 52, 55, 54, 62, 63, 65, 65, 63, 64, 56, 57, 58, 58, 57, 59, 58, 59, 52, 52, 59, 53, 53, 59, 55, 55, 59, 57, 58, 52, 56, 56, 52, 54, 54, 55, 60, 57, 56, 61, 54, 60, 62, 62, 60, 63, 60, 61, 63, 63, 61, 64, 61, 56, 64, 64, 56, 65, 56, 54, 65, 65, 54, 62, 73, 72, 75, 75, 72, 74, 70, 69, 67, 77, 76, 79, 79, 76, 78, 68, 71, 66, 61, 60, 69, 69, 60, 68, 57, 61, 70, 70, 61, 69, 55, 57, 71, 71, 57, 70, 60, 55, 68, 68, 55, 71, 69, 68, 73, 73, 68, 72, 66, 74, 68, 68, 74, 72, 66, 67, 74, 74, 67, 75, 67, 69, 75, 75, 69, 73, 71, 70, 77, 77, 70, 76, 70, 67, 76, 76, 67, 78, 67, 66, 78, 78, 66, 79, 66, 71, 79, 79, 71, 77};
    Mesh meshH6(vertH6, indexH6, glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, 40 * ULEN)), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)) , glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)));

    std::vector<float> vertK5 = {-0.325565, -0.050000, 0.050000, -0.236164, -0.050000, 0.050000, -0.325565, 0.450000, 0.050000, -0.236164, 0.450000, 0.050000, -0.325565, 0.450000, -0.050000, -0.236164, 0.450000, -0.050000, -0.325565, -0.050000, -0.050000, -0.236164, -0.050000, -0.050000, -0.236164, 0.357072, 0.050000, -0.325565, 0.350000, 0.050000, -0.325565, 0.350000, -0.050000, -0.236164, 0.357072, -0.050000, -0.212963, 0.259301, 0.050000, -0.325565, 0.250000, 0.050000, -0.325565, 0.250000, -0.050000, -0.212963, 0.259301, -0.050000, -0.236164, 0.139011, 0.050000, -0.325565, 0.150000, 0.050000, -0.325565, 0.150000, -0.050000, -0.236164, 0.139011, -0.050000, -0.236164, 0.050000, 0.050000, -0.325565, 0.050000, 0.050000, -0.325565, 0.050000, -0.050000, -0.236164, 0.050000, -0.050000, -0.132116, -0.048058, -0.050000, -0.132116, -0.048058, 0.050000, -0.032142, -0.045795, -0.050000, -0.032142, -0.045795, 0.050000, -0.236164, 0.050000, -0.050000, -0.236164, 0.050000, 0.050000, -0.236164, 0.139011, -0.050000, -0.236164, 0.139011, 0.050000, -0.051039, 0.449769, -0.050000, -0.051039, 0.449769, 0.050000, -0.151039, 0.449923, -0.050000, -0.151039, 0.449923, 0.050000, 0.363836, -0.050000, 0.050000, 0.363836, 0.050000, 0.050000, 0.363836, 0.050000, -0.050000, 0.363836, -0.050000, -0.050000, 0.263836, 0.050000, 0.050000, 0.263836, 0.050000, -0.050000, 0.263836, -0.050000, -0.050000, 0.263836, -0.050000, 0.050000, 0.263836, 0.150000, 0.050000, 0.363836, 0.150000, 0.050000, 0.363836, 0.150000, -0.050000, 0.263836, 0.150000, -0.050000, 0.263836, 0.250000, 0.050000, 0.363836, 0.250000, 0.050000, 0.363836, 0.250000, -0.050000, 0.263836, 0.250000, -0.050000, 0.163836, 0.150000, 0.050000, 0.163836, 0.150000, -0.050000, 0.163836, 0.250000, 0.050000, 0.163836, 0.250000, -0.050000, 0.063836, 0.150000, 0.050000, 0.063836, 0.150000, -0.050000, 0.063836, 0.250000, 0.050000, 0.063836, 0.250000, -0.050000, 0.163836, 0.350000, 0.050000, 0.163836, 0.350000, -0.050000, 0.063836, 0.350000, -0.050000, 0.063836, 0.350000, 0.050000, 0.163836, 0.450000, 0.050000, 0.163836, 0.450000, -0.050000, 0.063836, 0.450000, -0.050000, 0.063836, 0.450000, 0.050000, 0.363836, 0.350000, 0.050000, 0.363836, 0.350000, -0.050000, 0.363836, 0.450000, -0.050000, 0.363836, 0.450000, 0.050000, 0.063836, 0.050000, -0.050000, 0.063836, -0.050000, -0.050000, 0.063836, -0.050000, 0.050000, 0.063836, 0.050000, 0.050000};
    std::vector<unsigned int> indexK5 = {0, 1, 21, 21, 1, 20, 2, 3, 4, 4, 3, 5, 22, 23, 6, 6, 23, 7, 6, 7, 0, 0, 7, 1, 1, 7, 20, 20, 7, 23, 6, 0, 22, 22, 0, 21, 9, 8, 2, 2, 8, 3, 10, 9, 4, 4, 9, 2, 5, 11, 4, 4, 11, 10, 8, 11, 3, 3, 11, 5, 12, 8, 13, 13, 8, 9, 14, 13, 10, 10, 13, 9, 10, 11, 14, 14, 11, 15, 33, 32, 35, 35, 32, 34, 17, 16, 13, 13, 16, 12, 18, 17, 14, 14, 17, 13, 15, 19, 14, 14, 19, 18, 25, 24, 27, 27, 24, 26, 20, 16, 21, 21, 16, 17, 22, 21, 18, 18, 21, 17, 18, 19, 22, 22, 19, 23, 29, 28, 31, 31, 28, 30, 16, 19, 25, 25, 19, 24, 15, 26, 19, 19, 26, 24, 15, 12, 26, 26, 12, 27, 12, 16, 27, 27, 16, 25, 29, 20, 28, 20, 23, 28, 28, 23, 30, 23, 19, 30, 30, 19, 31, 19, 16, 31, 31, 16, 29, 16, 20, 29, 12, 15, 33, 33, 15, 32, 11, 34, 15, 15, 34, 32, 11, 8, 34, 34, 8, 35, 8, 12, 35, 35, 12, 33, 36, 39, 37, 37, 39, 38, 48, 49, 51, 51, 49, 50, 42, 41, 39, 39, 41, 38, 43, 42, 36, 36, 42, 39, 40, 43, 37, 37, 43, 36, 40, 37, 44, 44, 37, 45, 37, 38, 45, 45, 38, 46, 38, 41, 46, 46, 41, 47, 41, 40, 47, 47, 40, 44, 44, 45, 48, 48, 45, 49, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 57, 56, 59, 59, 56, 58, 47, 44, 53, 53, 44, 52, 44, 48, 52, 52, 48, 54, 48, 51, 54, 54, 51, 55, 51, 47, 55, 55, 47, 53, 53, 52, 57, 57, 52, 56, 52, 54, 56, 56, 54, 58, 64, 65, 67, 67, 65, 66, 55, 53, 59, 59, 53, 57, 54, 55, 60, 60, 55, 61, 55, 59, 61, 61, 59, 62, 59, 58, 62, 62, 58, 63, 58, 54, 63, 63, 54, 60, 68, 69, 71, 71, 69, 70, 61, 62, 65, 65, 62, 66, 62, 63, 66, 66, 63, 67, 63, 60, 67, 67, 60, 64, 60, 61, 68, 68, 61, 69, 61, 65, 69, 69, 65, 70, 65, 64, 70, 70, 64, 71, 64, 60, 71, 71, 60, 68, 72, 73, 75, 75, 73, 74, 41, 42, 72, 72, 42, 73, 42, 43, 73, 73, 43, 74, 43, 40, 74, 74, 40, 75, 40, 41, 75, 75, 41, 72};
    Mesh meshK5(vertK5, indexK5, glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, 40 * ULEN)), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)) , glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)));

    std::vector<float> vertN5 = {0.100000, -0.050000, 0.050000, 0.200000, -0.050000, 0.050000, 0.100000, 0.090032, 0.050000, 0.100000, 0.090032, -0.050000, 0.200000, -0.050000, -0.050000, 0.100000, -0.050000, -0.050000, 0.100000, 0.450000, 0.050000, 0.200000, 0.450000, 0.050000, 0.200000, 0.450000, -0.050000, 0.100000, 0.450000, -0.050000, -0.277414, -0.050000, 0.050000, -0.177414, -0.050000, 0.050000, -0.277414, 0.450000, 0.050000, -0.177414, 0.301243, 0.050000, -0.277414, 0.450000, -0.050000, -0.177414, 0.301243, -0.050000, -0.277414, -0.050000, -0.050000, -0.177414, -0.050000, -0.050000, -0.177414, 0.450000, 0.050000, -0.177414, 0.450000, -0.050000, 0.650000, -0.050000, 0.050000, 0.650000, 0.050000, 0.050000, 0.650000, 0.050000, -0.050000, 0.650000, -0.050000, -0.050000, 0.550000, 0.050000, 0.050000, 0.550000, 0.050000, -0.050000, 0.550000, -0.050000, -0.050000, 0.550000, -0.050000, 0.050000, 0.550000, 0.150000, 0.050000, 0.650000, 0.150000, 0.050000, 0.650000, 0.150000, -0.050000, 0.550000, 0.150000, -0.050000, 0.550000, 0.250000, 0.050000, 0.650000, 0.250000, 0.050000, 0.650000, 0.250000, -0.050000, 0.550000, 0.250000, -0.050000, 0.450000, 0.150000, 0.050000, 0.450000, 0.150000, -0.050000, 0.450000, 0.250000, 0.050000, 0.450000, 0.250000, -0.050000, 0.350000, 0.150000, 0.050000, 0.350000, 0.150000, -0.050000, 0.350000, 0.250000, 0.050000, 0.350000, 0.250000, -0.050000, 0.450000, 0.350000, 0.050000, 0.450000, 0.350000, -0.050000, 0.350000, 0.350000, -0.050000, 0.350000, 0.350000, 0.050000, 0.450000, 0.450000, 0.050000, 0.450000, 0.450000, -0.050000, 0.350000, 0.450000, -0.050000, 0.350000, 0.450000, 0.050000, 0.650000, 0.350000, 0.050000, 0.650000, 0.350000, -0.050000, 0.650000, 0.450000, -0.050000, 0.650000, 0.450000, 0.050000, 0.350000, 0.050000, -0.050000, 0.350000, -0.050000, -0.050000, 0.350000, -0.050000, 0.050000, 0.350000, 0.050000, 0.050000};
    std::vector<unsigned int> indexN5 = {0, 1, 2, 6, 7, 9, 9, 7, 8, 3, 4, 5, 5, 4, 0, 0, 4, 1, 1, 7, 2, 2, 7, 6, 1, 4, 7, 7, 4, 8, 4, 3, 8, 8, 3, 9, 3, 2, 9, 9, 2, 6, 11, 13, 10, 10, 13, 12, 12, 18, 14, 14, 18, 19, 14, 15, 16, 16, 15, 17, 16, 17, 10, 10, 17, 11, 11, 17, 13, 13, 17, 15, 16, 10, 14, 14, 10, 12, 12, 13, 18, 15, 14, 19, 2, 3, 18, 18, 3, 19, 5, 15, 3, 3, 15, 19, 5, 0, 15, 15, 0, 13, 0, 2, 13, 13, 2, 18, 20, 23, 21, 21, 23, 22, 32, 33, 35, 35, 33, 34, 26, 25, 23, 23, 25, 22, 27, 26, 20, 20, 26, 23, 24, 27, 21, 21, 27, 20, 24, 21, 28, 28, 21, 29, 21, 22, 29, 29, 22, 30, 22, 25, 30, 30, 25, 31, 25, 24, 31, 31, 24, 28, 28, 29, 32, 32, 29, 33, 29, 30, 33, 33, 30, 34, 30, 31, 34, 34, 31, 35, 41, 40, 43, 43, 40, 42, 31, 28, 37, 37, 28, 36, 28, 32, 36, 36, 32, 38, 32, 35, 38, 38, 35, 39, 35, 31, 39, 39, 31, 37, 37, 36, 41, 41, 36, 40, 36, 38, 40, 40, 38, 42, 48, 49, 51, 51, 49, 50, 39, 37, 43, 43, 37, 41, 38, 39, 44, 44, 39, 45, 39, 43, 45, 45, 43, 46, 43, 42, 46, 46, 42, 47, 42, 38, 47, 47, 38, 44, 52, 53, 55, 55, 53, 54, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 47, 44, 51, 51, 44, 48, 44, 45, 52, 52, 45, 53, 45, 49, 53, 53, 49, 54, 49, 48, 54, 54, 48, 55, 48, 44, 55, 55, 44, 52, 56, 57, 59, 59, 57, 58, 25, 26, 56, 56, 26, 57, 26, 27, 57, 57, 27, 58, 27, 24, 58, 58, 24, 59, 24, 25, 59, 59, 25, 56};
    Mesh meshN5(vertN5, indexN5, glm::translate(glm::mat4(1.0f), glm::vec3(40 * ULEN, ULEN, -40 * ULEN)), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)) , glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)));

    std::vector<float> vertO8 = {0.050000, -0.050000, 0.050000, 0.350000, -0.050000, 0.050000, 0.050000, 0.050000, 0.050000, 0.350000, 0.050000, 0.050000, 0.050000, 0.050000, -0.050000, 0.350000, 0.050000, -0.050000, 0.050000, -0.050000, -0.050000, 0.350000, -0.050000, -0.050000, 0.250000, 0.050000, 0.050000, 0.250000, 0.050000, -0.050000, 0.250000, -0.050000, -0.050000, 0.250000, -0.050000, 0.050000, 0.150000, 0.050000, 0.050000, 0.150000, 0.050000, -0.050000, 0.150000, -0.050000, -0.050000, 0.150000, -0.050000, 0.050000, 0.050000, 0.150000, 0.050000, 0.150000, 0.150000, 0.050000, 0.150000, 0.150000, -0.050000, 0.050000, 0.150000, -0.050000, 0.250000, 0.150000, 0.050000, 0.350000, 0.150000, 0.050000, 0.350000, 0.150000, -0.050000, 0.250000, 0.150000, -0.050000, 0.050000, 0.250000, 0.050000, 0.150000, 0.250000, 0.050000, 0.150000, 0.250000, -0.050000, 0.050000, 0.250000, -0.050000, 0.250000, 0.250000, 0.050000, 0.350000, 0.250000, 0.050000, 0.350000, 0.250000, -0.050000, 0.250000, 0.250000, -0.050000, 0.050000, 0.350000, 0.050000, 0.150000, 0.350000, 0.050000, 0.150000, 0.350000, -0.050000, 0.050000, 0.350000, -0.050000, 0.250000, 0.350000, 0.050000, 0.350000, 0.350000, 0.050000, 0.350000, 0.350000, -0.050000, 0.250000, 0.350000, -0.050000, 0.050000, 0.450000, 0.050000, 0.150000, 0.450000, 0.050000, 0.150000, 0.450000, -0.050000, 0.050000, 0.450000, -0.050000, 0.250000, 0.450000, 0.050000, 0.350000, 0.450000, 0.050000, 0.350000, 0.450000, -0.050000, 0.250000, 0.450000, -0.050000, -0.050000, -0.050000, 0.050000, -0.050000, 0.050000, 0.050000, -0.050000, 0.050000, -0.050000, -0.050000, -0.050000, -0.050000, -0.150000, 0.050000, 0.050000, -0.150000, 0.050000, -0.050000, -0.150000, -0.050000, -0.050000, -0.150000, -0.050000, 0.050000, -0.150000, 0.350000, 0.050000, -0.050000, 0.350000, 0.050000, -0.050000, 0.350000, -0.050000, -0.150000, 0.350000, -0.050000, -0.150000, 0.450000, 0.050000, -0.050000, 0.450000, 0.050000, -0.050000, 0.450000, -0.050000, -0.150000, 0.450000, -0.050000, -0.350000, 0.350000, 0.050000, -0.350000, 0.350000, -0.050000, -0.350000, 0.450000, 0.050000, -0.350000, 0.450000, -0.050000, -0.350000, 0.050000, -0.050000, -0.350000, -0.050000, -0.050000, -0.350000, -0.050000, 0.050000, -0.350000, 0.050000, 0.050000, -0.450000, 0.350000, 0.050000, -0.450000, 0.350000, -0.050000, -0.450000, 0.450000, 0.050000, -0.450000, 0.450000, -0.050000, -0.450000, 0.050000, -0.050000, -0.450000, -0.050000, -0.050000, -0.450000, -0.050000, 0.050000, -0.450000, 0.050000, 0.050000};
    std::vector<unsigned int> indexO8 = {0, 15, 2, 2, 15, 12, 40, 41, 43, 43, 41, 42, 4, 13, 6, 6, 13, 14, 6, 14, 0, 0, 14, 15, 1, 7, 3, 3, 7, 5, 6, 0, 4, 4, 0, 2, 44, 45, 47, 47, 45, 46, 10, 9, 7, 7, 9, 5, 11, 10, 1, 1, 10, 7, 8, 11, 3, 3, 11, 1, 12, 8, 13, 13, 8, 9, 14, 13, 10, 10, 13, 9, 15, 14, 11, 11, 14, 10, 12, 15, 8, 8, 15, 11, 2, 12, 16, 16, 12, 17, 12, 13, 17, 17, 13, 18, 13, 4, 18, 18, 4, 19, 4, 2, 19, 19, 2, 16, 8, 3, 20, 20, 3, 21, 3, 5, 21, 21, 5, 22, 5, 9, 22, 22, 9, 23, 9, 8, 23, 23, 8, 20, 16, 17, 24, 24, 17, 25, 18, 19, 26, 26, 19, 27, 19, 16, 27, 27, 16, 24, 20, 21, 28, 28, 21, 29, 21, 22, 29, 29, 22, 30, 22, 23, 30, 30, 23, 31, 24, 25, 32, 32, 25, 33, 25, 26, 33, 33, 26, 34, 26, 27, 34, 34, 27, 35, 27, 24, 35, 35, 24, 32, 28, 29, 36, 36, 29, 37, 29, 30, 37, 37, 30, 38, 30, 31, 38, 38, 31, 39, 31, 28, 39, 39, 28, 36, 18, 26, 23, 23, 26, 31, 26, 25, 31, 31, 25, 28, 25, 17, 28, 28, 17, 20, 17, 18, 20, 20, 18, 23, 32, 33, 40, 40, 33, 41, 34, 35, 42, 42, 35, 43, 35, 32, 43, 43, 32, 40, 36, 37, 44, 44, 37, 45, 37, 38, 45, 45, 38, 46, 38, 39, 46, 46, 39, 47, 34, 42, 39, 39, 42, 47, 42, 41, 47, 47, 41, 44, 41, 33, 44, 44, 33, 36, 33, 34, 36, 36, 34, 39, 48, 51, 49, 49, 51, 50, 60, 61, 63, 63, 61, 62, 54, 53, 51, 51, 53, 50, 55, 54, 48, 48, 54, 51, 52, 55, 49, 49, 55, 48, 56, 57, 60, 60, 57, 61, 57, 58, 61, 61, 58, 62, 58, 59, 62, 62, 59, 63, 50, 53, 58, 58, 53, 59, 53, 52, 59, 59, 52, 56, 52, 49, 56, 56, 49, 57, 49, 50, 57, 57, 50, 58, 73, 72, 75, 75, 72, 74, 76, 77, 79, 79, 77, 78, 59, 56, 65, 65, 56, 64, 56, 60, 64, 64, 60, 66, 60, 63, 66, 66, 63, 67, 63, 59, 67, 67, 59, 65, 53, 54, 68, 68, 54, 69, 54, 55, 69, 69, 55, 70, 55, 52, 70, 70, 52, 71, 52, 53, 71, 71, 53, 68, 64, 66, 72, 72, 66, 74, 66, 67, 74, 74, 67, 75, 67, 65, 75, 75, 65, 73, 68, 69, 76, 76, 69, 77, 69, 70, 77, 77, 70, 78, 70, 71, 78, 78, 71, 79, 65, 64, 68, 68, 64, 71, 64, 72, 71, 71, 72, 79, 72, 73, 79, 79, 73, 76, 73, 65, 76, 76, 65, 68};
    Mesh meshO8(vertO8, indexO8, glm::translate(glm::mat4(1.0f), glm::vec3(-40 * ULEN, ULEN, -40 * ULEN)), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)) , glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)));

    std::vector<float> vertR1 = {0.051719, -0.050000, 0.050000, 0.151719, -0.050000, 0.050000, 0.051719, 0.450000, 0.050000, 0.151719, 0.450000, 0.050000, 0.051719, 0.450000, -0.050000, 0.151719, 0.450000, -0.050000, 0.051719, -0.050000, -0.050000, 0.151719, -0.050000, -0.050000, -0.348031, -0.050000, 0.050000, -0.248031, -0.050000, 0.050000, -0.348031, 0.450000, 0.050000, -0.248031, 0.450000, 0.050000, -0.348031, 0.450000, -0.050000, -0.248031, 0.450000, -0.050000, -0.348031, -0.050000, -0.050000, -0.248031, -0.050000, -0.050000, -0.248031, 0.350000, 0.050000, -0.348031, 0.350000, 0.050000, -0.348031, 0.350000, -0.050000, -0.248031, 0.350000, -0.050000, -0.248031, 0.250000, 0.050000, -0.348031, 0.250000, 0.050000, -0.348031, 0.250000, -0.050000, -0.248031, 0.250000, -0.050000, -0.248031, 0.108034, 0.050000, -0.348031, 0.150000, 0.050000, -0.348031, 0.150000, -0.050000, -0.248031, 0.108034, -0.050000, -0.248031, 0.050000, 0.050000, -0.348031, 0.050000, 0.050000, -0.348031, 0.050000, -0.050000, -0.248031, 0.050000, -0.050000, -0.148031, 0.350000, -0.050000, -0.148031, 0.350000, 0.050000, -0.148031, 0.450000, -0.050000, -0.148031, 0.450000, 0.050000, -0.048031, 0.350000, -0.050000, -0.048031, 0.350000, 0.050000, -0.048031, 0.450000, -0.050000, -0.048031, 0.450000, 0.050000, -0.148031, 0.250000, -0.050000, -0.148031, 0.250000, 0.050000, -0.048031, 0.250000, -0.050000, -0.048031, 0.250000, 0.050000, -0.156174, 0.150000, -0.050000, -0.156174, 0.150000, 0.050000, -0.048031, 0.150000, -0.050000, -0.048031, 0.150000, 0.050000, -0.162415, -0.050000, -0.050000, -0.162415, -0.050000, 0.050000, -0.043567, -0.050000, -0.050000, -0.043567, -0.050000, 0.050000};
    std::vector<unsigned int> indexR1 = {0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 1, 7, 3, 3, 7, 5, 6, 0, 4, 4, 0, 2, 8, 9, 29, 29, 9, 28, 10, 11, 12, 12, 11, 13, 30, 31, 14, 14, 31, 15, 14, 15, 8, 8, 15, 9, 9, 15, 28, 28, 15, 31, 14, 8, 30, 30, 8, 29, 17, 16, 10, 10, 16, 11, 18, 17, 12, 12, 17, 10, 12, 13, 18, 18, 13, 19, 37, 36, 39, 39, 36, 38, 21, 20, 17, 17, 20, 16, 22, 21, 18, 18, 21, 17, 18, 19, 22, 22, 19, 23, 20, 23, 16, 16, 23, 19, 24, 20, 25, 25, 20, 21, 26, 25, 22, 22, 25, 21, 22, 23, 26, 26, 23, 27, 28, 24, 29, 29, 24, 25, 30, 29, 26, 26, 29, 25, 26, 27, 30, 30, 27, 31, 28, 31, 24, 24, 31, 27, 16, 19, 33, 33, 19, 32, 19, 13, 32, 32, 13, 34, 13, 11, 34, 34, 11, 35, 11, 16, 35, 35, 16, 33, 45, 44, 47, 47, 44, 46, 32, 34, 36, 36, 34, 38, 34, 35, 38, 38, 35, 39, 35, 33, 39, 39, 33, 37, 33, 32, 41, 41, 32, 40, 32, 36, 40, 40, 36, 42, 36, 37, 42, 42, 37, 43, 37, 33, 43, 43, 33, 41, 42, 46, 40, 40, 46, 44, 42, 43, 46, 46, 43, 47, 43, 41, 47, 47, 41, 45, 27, 23, 44, 44, 23, 40, 23, 20, 40, 40, 20, 41, 24, 45, 20, 20, 45, 41, 49, 48, 51, 51, 48, 50, 24, 27, 49, 49, 27, 48, 27, 44, 48, 48, 44, 50, 44, 45, 50, 50, 45, 51, 24, 49, 45, 45, 49, 51};
    Mesh meshR1(vertR1, indexR1, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, ULEN, 0.0f)), glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0)) , glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0)));

    modelMeshes[0] = meshR1;
    modelMeshes[1] = meshK5;
    modelMeshes[2] = meshN5;
    modelMeshes[3] = meshO8;
    modelMeshes[4] = meshH6;

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
        line.draw(lineShader);

        // Render grid
        // Activate shader
        gridShader.use();
        gridShader.setMat4("projection", projection);
        gridShader.setMat4("view", view);
        gridShader.setMat4("world", worldOrientation);

        grid.draw(gridShader);

        // Render models
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("world", worldOrientation);

        for (int i = 0; i < 5; i++) {
            modelMeshes[i].draw(modelShader, type);
        }

        // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    line.deleteBuffers();
    grid.deleteBuffers();
    for (int i = 0; i < 5; i++) {
        modelMeshes[i].deleteBuffers();
    }

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void process_input(GLFWwindow *window) {}

// GLFW: Whenever the window size changed (by OS or user resize), this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// GLFW: Whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    auto xoffset = float(xpos - lastX);
    auto yoffset = float(lastY - ypos); // Reversed since y-coordinates go from bottom to top

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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    //Close window by pressing Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Switch rendering type
    // Pressing P = Points
    // Pressing L = Lines
    // Pressing T = Triangles
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        type = GL_POINTS;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        type = GL_LINES;
    } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        type = GL_TRIANGLES;
    }

    //Select which model to alter
    // Pressing 0 = Model 1 (H6)
    // Pressing 1 = Model 2 (K5)
    // Pressing 2 = Model 3 (N5)
    // Pressing 3 = Model 4 (O8)
    // Pressing 4 = Model 5 (R1)
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

    // Press Spacebar to reposition selected model to a random spot on the grid
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        modelMeshes[selectedModel].randomReposition();
    }

    // Press U to scale up selected model
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        modelMeshes[selectedModel].scale(ULEN, ULEN, ULEN);
    }

    // Press J to scale down selected model
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        modelMeshes[selectedModel].scale(-ULEN, -ULEN, -ULEN);
    }

    // Press Shift + W to translate selected model in the -Z direction
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        modelMeshes[selectedModel].translate(0, 0, -ULEN);
    }

    // Press Shift + A to translate selected model in the -X direction
    // Press A to rotate selected model by -5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        modelMeshes[selectedModel].translate(-ULEN, 0, 0);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        modelMeshes[selectedModel].rotate(-5.0f, glm::vec3(0.0, 1.0, 0.0));
    }

    // Press Shift + S to translate selected model in the Z direction
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        modelMeshes[selectedModel].translate(0, 0, ULEN);
    }

    // Press Shift + D to translate selected model in the X direction
    // Press D to rotate selected model by 5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        modelMeshes[selectedModel].translate(ULEN, 0, 0);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        modelMeshes[selectedModel].rotate(5.0f, glm::vec3(0.0, 1.0, 0.0));
    }

    // Press Left Arrow Key to Rx
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        worldOrientation = glm::rotate(worldOrientation, glm::radians(1.0f), glm::vec3(ULEN, 0.0f, 0.0f));
    }

    // Press Right Arrow Key to R-x
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        worldOrientation = glm::rotate(worldOrientation, glm::radians(-1.0f), glm::vec3(ULEN, 0.0f, 0.0f));
    }

    // Press Up Arrow Key to Ry
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        worldOrientation = glm::rotate(worldOrientation, glm::radians(1.0f), glm::vec3(0.0f, ULEN, 0.0f));
    }

    // Press Up Arrow Key to R-y
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        worldOrientation = glm::rotate(worldOrientation, glm::radians(-1.0f), glm::vec3(0.0f, ULEN, 0.0f));
    }

    // Reset world orientation and camera by pressing Home button
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        for (int i = 0; i < 5; i++) {
            worldOrientation = glm::mat4(1.0f);
            modelMeshes[i].reset();
        }
        camera = Camera(glm::vec3(0.0f, 0.1f, 2.0f));
    }
}