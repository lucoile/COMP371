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

    float squareVertices[] = {
//          Vertices            Colors
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  //0
            0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  //1
            0.0f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f,  //2
            0.5f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f   //3
    };

    unsigned int squareIndices[] = {
            0, 1,
            0, 2,
            2, 3,
            1, 3,
    };

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
//          Vertices            Colors
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,       // red x-axis line
            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,       // green y-axis line
            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,       // blue z-axis line
            0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f
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

    float modelVertices[] = {
            1.000000, -0.500000, 0.500000, 2.000000, -0.500000, 0.500000, 1.000000, 0.900321, 0.500000, 1.000000, 0.900321, -0.500000, 2.000000, -0.500000, -0.500000, 1.000000, -0.500000, -0.500000, 1.000000, 4.500000, 0.500000, 2.000000, 4.500000, 0.500000, 2.000000, 4.500000, -0.500000, 1.000000, 4.500000, -0.500000, -2.774137, -0.500000, 0.500000, -1.774137, -0.500000, 0.500000, -2.774137, 4.500000, 0.500000, -1.774137, 3.012429, 0.500000, -2.774137, 4.500000, -0.500000, -1.774137, 3.012429, -0.500000, -2.774137, -0.500000, -0.500000, -1.774137, -0.500000, -0.500000, -1.774137, 4.500000, 0.500000, -1.774137, 4.500000, -0.500000, 6.500000, -0.500000, 0.500000, 6.500000, 0.500000, 0.500000, 6.500000, 0.500000, -0.500000, 6.500000, -0.500000, -0.500000, 5.500000, 0.500000, 0.500000, 5.500000, 0.500000, -0.500000, 5.500000, -0.500000, -0.500000, 5.500000, -0.500000, 0.500000, 5.500000, 1.500000, 0.500000, 6.500000, 1.500000, 0.500000, 6.500000, 1.500000, -0.500000, 5.500000, 1.500000, -0.500000, 5.500000, 2.500000, 0.500000, 6.500000, 2.500000, 0.500000, 6.500000, 2.500000, -0.500000, 5.500000, 2.500000, -0.500000, 4.500000, 1.500000, 0.500000, 4.500000, 1.500000, -0.500000, 4.500000, 2.500000, 0.500000, 4.500000, 2.500000, -0.500000, 3.500000, 1.500000, 0.500000, 3.500000, 1.500000, -0.500000, 3.500000, 2.500000, 0.500000, 3.500000, 2.500000, -0.500000, 4.500000, 3.500000, 0.500000, 4.500000, 3.500000, -0.500000, 3.500000, 3.500000, -0.500000, 3.500000, 3.500000, 0.500000, 4.500000, 4.500000, 0.500000, 4.500000, 4.500000, -0.500000, 3.500000, 4.500000, -0.500000, 3.500000, 4.500000, 0.500000, 6.500000, 3.500000, 0.500000, 6.500000, 3.500000, -0.500000, 6.500000, 4.500000, -0.500000, 6.500000, 4.500000, 0.500000, 3.500000, 0.500000, -0.500000, 3.500000, -0.500000, -0.500000, 3.500000, -0.500000, 0.500000, 3.500000, 0.500000, 0.500000
    };

    unsigned int modelIndices[] = {
            1, 2, 3, 7, 8, 10, 10, 8, 9, 4, 5, 6, 6, 5, 1, 1, 5, 2, 2, 8, 3, 3, 8, 7, 2, 5, 8, 8, 5, 9, 5, 4, 9, 9, 4, 10, 4, 3, 10, 10, 3, 7, 12, 14, 11, 11, 14, 13, 13, 19, 15, 15, 19, 20, 15, 16, 17, 17, 16, 18, 17, 18, 11, 11, 18, 12, 12, 18, 14, 14, 18, 16, 17, 11, 15, 15, 11, 13, 13, 14, 19, 16, 15, 20, 3, 4, 19, 19, 4, 20, 6, 16, 4, 4, 16, 20, 6, 1, 16, 16, 1, 14, 1, 3, 14, 14, 3, 19, 21, 24, 22, 22, 24, 23, 33, 34, 36, 36, 34, 35, 27, 26, 24, 24, 26, 23, 28, 27, 21, 21, 27, 24, 25, 28, 22, 22, 28, 21, 25, 22, 29, 29, 22, 30, 22, 23, 30, 30, 23, 31, 23, 26, 31, 31, 26, 32, 26, 25, 32, 32, 25, 29, 29, 30, 33, 33, 30, 34, 30, 31, 34, 34, 31, 35, 31, 32, 35, 35, 32, 36, 42, 41, 44, 44, 41, 43, 32, 29, 38, 38, 29, 37, 29, 33, 37, 37, 33, 39, 33, 36, 39, 39, 36, 40, 36, 32, 40, 40, 32, 38, 38, 37, 42, 42, 37, 41, 37, 39, 41, 41, 39, 43, 49, 50, 52, 52, 50, 51, 40, 38, 44, 44, 38, 42, 39, 40, 45, 45, 40, 46, 40, 44, 46, 46, 44, 47, 44, 43, 47, 47, 43, 48, 43, 39, 48, 48, 39, 45, 53, 54, 56, 56, 54, 55, 46, 47, 50, 50, 47, 51, 47, 48, 51, 51, 48, 52, 48, 45, 52, 52, 45, 49, 45, 46, 53, 53, 46, 54, 46, 50, 54, 54, 50, 55, 50, 49, 55, 55, 49, 56, 49, 45, 56, 56, 45, 53, 57, 58, 60, 60, 58, 59, 26, 27, 57, 57, 27, 58, 27, 28, 58, 58, 28, 59, 28, 25, 59, 59, 25, 60, 25, 26, 60, 60, 26, 57

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

    //
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
//    glEnableVertexAttribArray(0);
//    // Color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
//    glEnableVertexAttribArray(1);


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
        glClearColor(0.529f, 0.808f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
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

        glBindVertexArray(modelVAO);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for (unsigned int i = 0; i < 10; i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, glm::vec3((i , 0.0f, 0.0f)));
            gridShader.setMat4("model", model);
            glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
        }
        //glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteBuffers(1, &lineEBO);

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