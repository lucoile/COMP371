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

std::vector<float> read_csv(std::string filename);

// Settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
const float ULEN = 0.1f; // Unit Length

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Render Type
GLenum type = GL_TRIANGLES;

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COMP371 Project", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

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
            0.0f, 0.0f, 0.0f,   0.0f, 0.407f, 0.478f,  // Point 0
            ULEN, 0.0f, 0.0f,   0.0f, 0.407f, 0.478f,  // Point 1
            0.0f, 0.0f, ULEN,   0.0f, 0.407f, 0.478f,  // Point 2
            ULEN, 0.0f, ULEN,   0.0f, 0.407f, 0.478f   // Point 3
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
        for ( int j = -50; j < 50; j++) {
            gridPositions[i+50][j+50] = glm::vec3((float) i / (1/ULEN), 0.0f, (float) j / (1/ULEN));
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float linesVertices[] = {
//          Vertices                  Colors
            0.0f,   0.0f,   0.0f,     1.0f, 0.0f, 0.0f,       // red x-axis line
            0.0f,   0.0f,   0.0f,     0.0f, 1.0f, 0.0f,       // green y-axis line
            0.0f,   0.0f,   0.0f,     0.0f, 0.0f, 1.0f,       // blue z-axis line
            ULEN*5, 0.0f,   0.0f,     1.0f, 0.0f, 0.0f,
            0.0f,   ULEN*5, 0.0f,     0.0f, 1.0f, 0.0f,
            0.0f,   0.0f,   ULEN*5,   0.0f, 0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<float> modelVAO1 = read_csv("../res/models/alphanumeric_vertices.csv");
    std::vector<float> modelEBO1 = read_csv("../res/models/alphanumeric_elements.csv");

    // H6 model data
    float modelVertices[] = {
            0.074075, -0.050000, 0.050000, 0.174075, -0.050000, 0.050000, 0.074075, 0.050000, 0.050000, 0.174075, 0.050000, 0.050000, 0.074075, 0.050000, -0.050000, 0.174075, 0.050000, -0.050000, 0.074075, -0.050000, -0.050000, 0.174075, -0.050000, -0.050000, 0.274075, -0.050000, -0.050000, 0.274075, -0.050000, 0.050000, 0.274075, 0.050000, -0.050000, 0.274075, 0.050000, 0.050000, 0.374075, -0.050000, -0.050000, 0.374075, -0.050000, 0.050000, 0.374075, 0.050000, -0.050000, 0.374075, 0.050000, 0.050000, 0.074075, 0.150000, 0.050000, 0.174075, 0.150000, 0.050000, 0.174075, 0.150000, -0.050000, 0.074075, 0.150000, -0.050000, 0.074075, 0.250000, 0.050000, 0.174075, 0.250000, 0.050000, 0.174075, 0.250000, -0.050000, 0.074075, 0.250000, -0.050000, 0.074075, 0.350000, 0.050000, 0.174075, 0.350000, 0.050000, 0.174075, 0.350000, -0.050000, 0.074075, 0.350000, -0.050000, 0.074075, 0.450000, 0.050000, 0.174075, 0.450000, 0.050000, 0.174075, 0.450000, -0.050000, 0.074075, 0.450000, -0.050000, 0.274075, 0.350000, 0.050000, 0.274075, 0.350000, -0.050000, 0.274075, 0.450000, -0.050000, 0.274075, 0.450000, 0.050000, 0.374075, 0.350000, 0.050000, 0.374075, 0.350000, -0.050000, 0.374075, 0.450000, -0.050000, 0.374075, 0.450000, 0.050000, 0.274075, 0.350000, 0.050000, 0.274075, 0.350000, -0.050000, 0.374075, 0.350000, -0.050000, 0.374075, 0.350000, 0.050000, 0.274075, 0.150000, 0.050000, 0.274075, 0.150000, -0.050000, 0.274075, 0.250000, -0.050000, 0.274075, 0.250000, 0.050000, 0.374075, 0.150000, 0.050000, 0.374075, 0.150000, -0.050000, 0.374075, 0.250000, -0.050000, 0.374075, 0.250000, 0.050000, -0.495108, -0.050000, 0.050000, -0.395108, -0.050000, 0.050000, -0.495108, 0.250000, 0.050000, -0.395108, 0.150000, 0.050000, -0.495108, 0.250000, -0.050000, -0.395108, 0.150000, -0.050000, -0.495108, -0.050000, -0.050000, -0.395108, -0.050000, -0.050000, -0.395108, 0.250000, 0.050000, -0.395108, 0.250000, -0.050000, -0.495108, 0.450000, 0.050000, -0.395108, 0.450000, 0.050000, -0.395108, 0.450000, -0.050000, -0.495108, 0.450000, -0.050000, -0.095108, 0.150000, 0.050000, -0.095108, 0.150000, -0.050000, -0.195108, 0.250000, 0.050000, -0.195108, 0.250000, -0.050000, -0.195108, 0.150000, -0.050000, -0.195108, 0.150000, 0.050000, -0.195108, 0.450000, 0.050000, -0.195108, 0.450000, -0.050000, -0.095108, 0.450000, 0.050000, -0.095108, 0.450000, -0.050000, -0.195108, -0.050000, -0.050000, -0.195108, -0.050000, 0.050000, -0.095108, -0.050000, -0.050000, -0.095108, -0.050000, 0.050000
    };
    unsigned int modelIndices[] = {
            0, 1, 2, 2, 1, 3, 28, 29, 31, 31, 29, 30, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 13, 12, 15, 15, 12, 14, 6, 0, 4, 4, 0, 2, 1, 7, 9, 9, 7, 8, 7, 5, 8, 8, 5, 10, 5, 3, 10, 10, 3, 11, 3, 1, 11, 11, 1, 9, 9, 8, 13, 13, 8, 12, 8, 10, 12, 12, 10, 14, 11, 9, 15, 15, 9, 13, 2, 3, 16, 16, 3, 17, 3, 5, 17, 17, 5, 18, 5, 4, 18, 18, 4, 19, 4, 2, 19, 19, 2, 16, 16, 17, 20, 20, 17, 21, 48, 49, 51, 51, 49, 50, 18, 19, 22, 22, 19, 23, 19, 16, 23, 23, 16, 20, 20, 21, 24, 24, 21, 25, 21, 22, 25, 25, 22, 26, 22, 23, 26, 26, 23, 27, 23, 20, 27, 27, 20, 24, 24, 25, 28, 28, 25, 29, 36, 37, 39, 39, 37, 38, 26, 27, 30, 30, 27, 31, 27, 24, 31, 31, 24, 28, 25, 26, 32, 32, 26, 33, 26, 30, 33, 33, 30, 34, 30, 29, 34, 34, 29, 35, 29, 25, 35, 35, 25, 32, 40, 41, 43, 43, 41, 42, 33, 34, 37, 37, 34, 38, 34, 35, 38, 38, 35, 39, 35, 32, 39, 39, 32, 36, 40, 32, 41, 32, 33, 41, 41, 33, 42, 33, 37, 42, 42, 37, 43, 37, 36, 43, 43, 36, 40, 36, 32, 40, 17, 18, 44, 44, 18, 45, 18, 22, 45, 45, 22, 46, 22, 21, 46, 46, 21, 47, 21, 17, 47, 47, 17, 44, 45, 46, 49, 49, 46, 50, 46, 47, 50, 50, 47, 51, 47, 44, 51, 51, 44, 48, 10, 11, 45, 45, 11, 44, 11, 15, 44, 44, 15, 48, 15, 14, 48, 48, 14, 49, 14, 10, 49, 49, 10, 45, 53, 55, 52, 52, 55, 54, 62, 63, 65, 65, 63, 64, 56, 57, 58, 58, 57, 59, 58, 59, 52, 52, 59, 53, 53, 59, 55, 55, 59, 57, 58, 52, 56, 56, 52, 54, 54, 55, 60, 57, 56, 61, 54, 60, 62, 62, 60, 63, 60, 61, 63, 63, 61, 64, 61, 56, 64, 64, 56, 65, 56, 54, 65, 65, 54, 62, 73, 72, 75, 75, 72, 74, 70, 69, 67, 77, 76, 79, 79, 76, 78, 68, 71, 66, 61, 60, 69, 69, 60, 68, 57, 61, 70, 70, 61, 69, 55, 57, 71, 71, 57, 70, 60, 55, 68, 68, 55, 71, 69, 68, 73, 73, 68, 72, 66, 74, 68, 68, 74, 72, 66, 67, 74, 74, 67, 75, 67, 69, 75, 75, 69, 73, 71, 70, 77, 77, 70, 76, 70, 67, 76, 76, 67, 78, 67, 66, 78, 78, 66, 79, 66, 71, 79, 79, 71, 77
    };
    // Models
    unsigned int modelEBO, modelVAO, modelVBO;
    glGenVertexArrays(1, &modelVAO);
    glGenBuffers(1, &modelVBO);
    glGenBuffers(1, &modelEBO);

    glBindVertexArray(modelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertices), modelVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Load and Create a texture
    Texture texture1("res/textures/dirt.jpg");

    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    gridShader.use();

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Per Frame Time Logic
        float currentFrame = glfwGetTime();
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

        glBindVertexArray(squareVAO);
        for (unsigned int i = 0; i < 100; i++) {
            for (unsigned int j = 0; j < 100; j++) {
                glm::mat4 model = glm::mat4(1.0f); //Use Identity Matrix to bring back to original
                model = glm::translate(model, gridPositions[i][j]);
                gridShader.setMat4("model", model);

                glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
            }
        }
        glBindVertexArray(0);

        // Render models
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        glBindVertexArray(modelVAO);
        glDrawElements(GL_TRIANGLES, sizeof(modelVertices), GL_UNSIGNED_INT, 0);
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

std::vector<float> read_csv(std::string filename) {
    using namespace std;
    vector<string>  models;
    string model;
    vector<float> result;
    string line, element, temp;

    // input filestream
    ifstream myFile(filename);

    // open file
    if(!myFile.is_open()) throw runtime_error("Error: File Was Not Opened");

    while(getline(myFile, model, ',')){
        if (model.compare("\n") == 0){
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        type = GL_POINTS;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        type = GL_LINES;
    } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        type = GL_TRIANGLES;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        camera.process_mouse_movement(xoffset, yoffset, PAN);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.process_mouse_movement(0, yoffset, ZOOM);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        camera.process_mouse_movement(xoffset, yoffset, TILT);
    }
}