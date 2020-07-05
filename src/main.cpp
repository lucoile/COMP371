// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "filesystem.h"
#include "shader_m.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

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
    glfwSetScrollCallback(window, scroll_callback);

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
    Shader ourShader("../res/shaders/camera.vert", "../res/shaders/camera.frag");
    Shader lineShader("../res/shaders/line.vert", "../res/shaders/line.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            -0.1f, -0.1f, -0.1f, 0.0f, 0.0f,
            0.1f, -0.1f, -0.1f, 1.0f, 0.0f,
            0.1f, 0.1f, -0.1f, 1.0f, 1.0f,
            0.1f, 0.1f, -0.1f, 1.0f, 1.0f,
            -0.1f, 0.1f, -0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, -0.1f, 0.0f, 0.0f,

            -0.1f, -0.1f, 0.1f, 0.0f, 0.0f,
            0.1f, -0.1f, 0.1f, 1.0f, 0.0f,
            0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
            0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
            -0.1f, 0.1f, 0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, 0.1f, 0.0f, 0.0f,

            -0.1f, 0.1f, 0.1f, 1.0f, 0.0f,
            -0.1f, 0.1f, -0.1f, 1.0f, 1.0f,
            -0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
            -0.1f, -0.1f, 0.1f, 0.0f, 0.0f,
            -0.1f, 0.1f, 0.1f, 1.0f, 0.0f,

            0.1f, 0.1f, 0.1f, 1.0f, 0.0f,
            0.1f, 0.1f, -0.1f, 1.0f, 1.0f,
            0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, -0.1f, 0.1f, 0.0f, 0.0f,
            0.1f, 0.1f, 0.1f, 1.0f, 0.0f,

            -0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, -0.1f, -0.1f, 1.0f, 1.0f,
            0.1f, -0.1f, 0.1f, 1.0f, 0.0f,
            0.1f, -0.1f, 0.1f, 1.0f, 0.0f,
            -0.1f, -0.1f, 0.1f, 0.0f, 0.0f,
            -0.1f, -0.1f, -0.1f, 0.0f, 1.0f,

            -0.1f, 0.1f, -0.1f, 0.0f, 1.0f,
            0.1f, 0.1f, -0.1f, 1.0f, 1.0f,
            0.1f, 0.1f, 0.1f, 1.0f, 0.0f,
            0.1f, 0.1f, 0.1f, 1.0f, 0.0f,
            -0.1f, 0.1f, 0.1f, 0.0f, 0.0f,
            -0.1f, 0.1f, -0.1f, 0.0f, 1.0f
    };

    float linesVertices[] = {
//          Vertices            Colors
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,       // red x-axis line
            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,       // green y-axis line
            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,       // blue z-axis line
            5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 5.0f,   1.0f, 0.0f, 0.0f
    };
    unsigned int linesIndices[] = {
            0, 3,   // red x-axis line
            1, 4,   // green y-axis line
            2, 5    // blue z-axis line
    };

    // World space positions of our cubes
    glm::vec3 cubePositions[100][100];

    for (unsigned int i = 0; i < 100; i++) {
        for (unsigned int j = 0; j < 100; j++) {
            cubePositions[i][j] = glm::vec3((float) i / 5, 0.0f, (float) j / 5);
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Cube
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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


    // Load and Create a texture
    unsigned int texture1;

    // Texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("res/textures/dirt.jpg").c_str(), &width, &height, &nrChannels,
                                    0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader.use();
    ourShader.setInt("texture1", 0);

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Per Frame Time Logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.529f, 0.808f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render lines
        // Activate line shader
        lineShader.use();
        // Draw lines
        glLineWidth(1.0f);
        glBindVertexArray(lineVAO);
        glDrawElements(GL_LINES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        // Activate shader
        ourShader.use();

        // Pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        ourShader.setMat4("projection", projection);

        // Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // Render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 100; i++) {
            for (unsigned int j = 0; j < 100; j++) {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, cubePositions[i][j]);
                //float angle = 20.0f * i;
                //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                ourShader.setMat4("model", model);
                glDrawArrays(type, 0, 250);
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &lineEBO);

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        type = GL_POINTS;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        type = GL_LINES;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        type = GL_TRIANGLES;
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}