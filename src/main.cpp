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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
            1, 2, 3, 3, 2, 4, 29, 30, 32, 32, 30, 31, 5, 6, 7, 7, 6, 8, 7, 8, 1, 1, 8, 2, 14, 13, 16, 16, 13, 15, 7, 1, 5, 5, 1, 3, 2, 8, 10, 10, 8, 9, 8, 6, 9, 9, 6, 11, 6, 4, 11, 11, 4, 12, 4, 2, 12, 12, 2, 10, 10, 9, 14, 14, 9, 13, 9, 11, 13, 13, 11, 15, 12, 10, 16, 16, 10, 14, 3, 4, 17, 17, 4, 18, 4, 6, 18, 18, 6, 19, 6, 5, 19, 19, 5, 20, 5, 3, 20, 20, 3, 17, 17, 18, 21, 21, 18, 22, 49, 50, 52, 52, 50, 51, 19, 20, 23, 23, 20, 24, 20, 17, 24, 24, 17, 21, 21, 22, 25, 25, 22, 26, 22, 23, 26, 26, 23, 27, 23, 24, 27, 27, 24, 28, 24, 21, 28, 28, 21, 25, 25, 26, 29, 29, 26, 30, 37, 38, 40, 40, 38, 39, 27, 28, 31, 31, 28, 32, 28, 25, 32, 32, 25, 29, 26, 27, 33, 33, 27, 34, 27, 31, 34, 34, 31, 35, 31, 30, 35, 35, 30, 36, 30, 26, 36, 36, 26, 33, 41, 42, 44, 44, 42, 43, 34, 35, 38, 38, 35, 39, 35, 36, 39, 39, 36, 40, 36, 33, 40, 40, 33, 37, 41, 33, 42, 33, 34, 42, 42, 34, 43, 34, 38, 43, 43, 38, 44, 38, 37, 44, 44, 37, 41, 37, 33, 41, 18, 19, 45, 45, 19, 46, 19, 23, 46, 46, 23, 47, 23, 22, 47, 47, 22, 48, 22, 18, 48, 48, 18, 45, 46, 47, 50, 50, 47, 51, 47, 48, 51, 51, 48, 52, 48, 45, 52, 52, 45, 49, 11, 12, 46, 46, 12, 45, 12, 16, 45, 45, 16, 49, 16, 15, 49, 49, 15, 50, 15, 11, 50, 50, 11, 46, 54, 56, 53, 53, 56, 55, 63, 64, 66, 66, 64, 65, 57, 58, 59, 59, 58, 60, 59, 60, 53, 53, 60, 54, 54, 60, 56, 56, 60, 58, 59, 53, 57, 57, 53, 55, 55, 56, 61, 58, 57, 62, 55, 61, 63, 63, 61, 64, 61, 62, 64, 64, 62, 65, 62, 57, 65, 65, 57, 66, 57, 55, 66, 66, 55, 63, 74, 73, 76, 76, 73, 75, 71, 70, 68, 78, 77, 80, 80, 77, 79, 69, 72, 67, 62, 61, 70, 70, 61, 69, 58, 62, 71, 71, 62, 70, 56, 58, 72, 72, 58, 71, 61, 56, 69, 69, 56, 72, 70, 69, 74, 74, 69, 73, 67, 75, 69, 69, 75, 73, 67, 68, 75, 75, 68, 76, 68, 70, 76, 76, 70, 74, 72, 71, 78, 78, 71, 77, 71, 68, 77, 77, 68, 79, 68, 67, 79, 79, 67, 80, 67, 72, 80, 80, 72, 78    };

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
        glDrawElements(GL_TRIANGLES, sizeof(modelVertices)/sizeof(float), GL_UNSIGNED_INT, 0);
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