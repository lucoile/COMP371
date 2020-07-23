// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Utilities/shader_m.h"
#include "Utilities/camera.h"
#include "Utilities/model.h"
#include "Utilities/texture.h"

#include "Grid/grid.h"
#include "Line/line.h"

#include <iostream>
#include <istream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void process_input(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void resetTextures(const Shader &shader);

// Settings
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;
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

// Texture Toggle
unsigned int textureOn = 0;

// Alphanumeric class
struct Alphanum {
    std::vector<glm::mat4> letterTrans;
    std::vector<glm::mat4> numTrans;
    glm::mat4 letterAdjust;
    glm::mat4 numAdjust;
    glm::mat4 rotation;
    glm::mat4 scale;
    glm::mat4 translation;
};

// Alphanumeric models data structure
Alphanum models[5];

// Identity matrix
glm::mat4 id(1.0f);

// World Orientation
glm::mat4 worldOrientation(1.0f);


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
    glfwSetInputMode(window, GLFW_CURSOR, false);

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
    Shader cubeShader("../res/shaders/cube.vert", "../res/shaders/cube.frag");


    std::vector<float> vertGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f, 0.0f, 0.0f,
                                  ULEN, 0.0f, 0.0f, 0.0f, 0.407f, 0.478f, 1.0f, 0.0f,
                                  0.0f, 0.0f, ULEN, 0.0f, 0.407f, 0.478f, 0.0f, 1.0f,
                                  ULEN, 0.0f, ULEN, 0.0f, 0.407f, 0.478f, 1.0f, 1.0f};

    std::vector<unsigned int> indexGridTri = {0, 2, 3, 3, 1, 0};
	std::vector<unsigned int> indexGridLine = {0, 1, 0, 2, 2, 3, 1, 3};
    Grid gridTri(vertGrid, indexGridTri);
    Grid gridLine(vertGrid, indexGridLine);

//    if (textureOn == 0){
//        std::vector<unsigned int> indexGrid = {0, 1, 0, 2, 2, 3, 1, 3};
//        Grid grid(vertGrid, indexGrid);
//    }else if ((textureOn == 1)){
//        std::vector<unsigned int> indexGrid = {0, 2, 3, 3, 1, 0};
//        Grid grid(vertGrid, indexGrid);
//    }

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

    // Cube model
    Model cube("../res/models/cube/cube.obj");

    // glm::mat4 transformations = projection * view * worldOrientation * translation * rotation * scale;

    // R1
    models[0].letterTrans.push_back(
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 4.0f*ULEN, 0.0f))
            );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0*ULEN, 2.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
            );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 2.0*ULEN, 0.0f))
            );
    glm::mat4 shearR = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -1.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearR
    );

    models[0].numTrans.push_back(glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)));

    models[0].letterAdjust = glm::translate(id, glm::vec3(-3*ULEN, 0.0f, 0.0f));
    models[0].numAdjust = glm::translate(id, glm::vec3(ULEN, 0.0f, 0.0f));

    models[0].scale = id;
    models[0].translation = id;
    models[0].rotation = id;

    // H6
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2*ULEN, 0.0f))
            );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );

    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0*ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0*ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            id
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[1].letterAdjust = glm::translate(id, glm::vec3(-2.0*ULEN, 0.0f, 0.0f));
    models[1].numAdjust = glm::translate(id, glm::vec3(2.0*ULEN, 0.0f, 0.0f));

    models[1].scale = id;
    models[1].translation = glm::translate(id, glm::vec3(40 * ULEN, 0.0f, 40 * ULEN));
    models[1].rotation = id;

    // N5
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.5*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    glm::mat4 shearN = {
        1.0f, 0.0f, 0.0f, 0.0f,
        -3.0, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)) * shearN
    );

    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 0.0f, 0.0f))
            );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 2.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0*ULEN, 0.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5*ULEN, 4.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            id
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[2].letterAdjust = glm::translate(id, glm::vec3(-2.5*ULEN, 0.0f, 0.0f));
    models[2].numAdjust = glm::translate(id, glm::vec3(2.0*ULEN, 0.0f, 0.0f));

    models[2].scale = id;
    models[2].translation = glm::translate(id, glm::vec3(-40 * ULEN, 0.0f, 40 * ULEN));
    models[2].rotation = id;

    // 08
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(1*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    models[3].letterTrans.push_back(
            id
    );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0*ULEN, 0.0f))
    );

    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(-1*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(1*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            id
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0*ULEN, 0.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0*ULEN, 0.0f))
    );

    models[3].letterAdjust = glm::translate(id, glm::vec3(-2.0*ULEN, 0.0f, 0.0f));
    models[3].numAdjust = glm::translate(id, glm::vec3(2.0*ULEN, 0.0f, 0.0f));

    models[3].scale = id;
    models[3].translation = glm::translate(id, glm::vec3(-40 * ULEN, 0.0f, -40 * ULEN));
    models[3].rotation = id;

    // K5
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
            );
    glm::mat4 shearKTop = {
            1.0f, 0.0f, 0.0f, 0.0f,
            2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 3.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearKTop
    );
    glm::mat4 shearKBottom = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f)) * shearKBottom
    );

    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 0.0f, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0*ULEN, 2.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0*ULEN, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5*ULEN, 4.0*ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            id
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0*ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[4].letterAdjust = glm::translate(id, glm::vec3(-2.0*ULEN, 0.0f, 0.0f));
    models[4].numAdjust = glm::translate(id, glm::vec3(2.0*ULEN, 0.0f, 0.0f));

    models[4].scale = id;
    models[4].translation = glm::translate(id, glm::vec3(40 * ULEN, 0.0f, -40 * ULEN));
    models[4].rotation = id;

    // Textures
	Texture boxTexture("res/textures/box.jpg");
	Texture groundTexture("res/textures/ground.jpg");
	Texture shinyTexture("res/textures/yellow.png");
	Texture greyTexture("res/textures/grey.png");

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

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        groundTexture.bind();

        if (textureOn == 1){
            // bind ground texture
            gridTri.draw(gridShader, GL_TRIANGLES);
        }else{
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            greyTexture.bind();
            gridLine.draw(gridShader, GL_LINES);
        }

        cubeShader.use();

        // bind box texture
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        boxTexture.bind();

		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		shinyTexture.bind();

		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		greyTexture.bind();

        // material properties
        if(textureOn == 1){
            cubeShader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
            cubeShader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
            cubeShader.setFloat("material.shininess", 32.0f);

            // light properties
            cubeShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
            cubeShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
            cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
            cubeShader.setVec3("light.position", 0.0f, 30.0*ULEN, 0.0f);
        } else{
            resetTextures(cubeShader);
        }

        // render each alphanumeric pair by looping through the array of models
        for (unsigned int j = 0; j < 5; j++)
        {
            // add box texture
            if(textureOn == 1){
                cubeShader.setInt("material.diffuse", 1);
            }else{
				cubeShader.setInt("material.diffuse", 3);
                resetTextures(cubeShader);
            }
            // draw the letter
            for (unsigned int i = 0; i < models[j].letterTrans.size(); i++)
            {
                glm::mat4 model = worldOrientation * models[j].translation * models[j].rotation * models[j].scale * models[j].letterAdjust;
                glm::mat4 transformations = projection * view * model *  models[j].letterTrans[i];
                cubeShader.setMat4("transformations", transformations);
                cubeShader.setMat4("model", model);

                cube.Draw(cubeShader, type);
            }

            // add shiny texture
            if(textureOn == 1){
                cubeShader.setInt("material.diffuse", 2);
                cubeShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
                cubeShader.setFloat("material.shininess", 64.0f);
            } else {
				cubeShader.setInt("material.diffuse", 3);
				resetTextures(cubeShader);
            }

            // draw the number
            for (unsigned int i = 0; i < models[j].numTrans.size(); i++)
            {
                glm::mat4 model = worldOrientation * models[j].translation * models[j].rotation * models[j].scale * models[j].numAdjust;
                glm::mat4 transformations = projection * view * model * models[j].numTrans[i];
                cubeShader.setMat4("transformations", transformations);
                cubeShader.setMat4("model", model);

                cube.Draw(cubeShader, type);
            }
        }

        // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    line.deleteBuffers();
    gridTri.deleteBuffers();
    gridLine.deleteBuffers();

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void resetTextures(const Shader &shader) {
    shader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
    shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
    shader.setFloat("material.shininess", 32.0f);

    // light properties
    shader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.position", 0.0f, 30.0*ULEN, 0.0f);
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void process_input(GLFWwindow *window) {}

// GLFW: Whenever the window size changed (by OS or user resize), this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
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
    // Pressing 0 = Model 1 (R1)
    // Pressing 1 = Model 2 (H6)
    // Pressing 2 = Model 3 (N5)
    // Pressing 3 = Model 4 (O8)
    // Pressing 4 = Model 5 (K5)
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
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        //Random Number Between -50 and 50 Normalized
        float randomX = (rand() % 101 + (-50))/(1/ULEN);
        float randomZ = (rand() % 101 + (-50))/(1/ULEN);

        //Translate to the new random position on the grid.
        translation = glm::translate(translation, glm::vec3(randomX, 0, randomZ));
        models[selectedModel].translation = translation;
    }

    // Press U to scale up selected model
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        models[selectedModel].scale = glm::scale(models[selectedModel].scale,
                glm::vec3(1.0f + ULEN, 1.0f + ULEN, 1.0f + ULEN));
    }

    // Press J to scale down selected model
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        models[selectedModel].scale = glm::scale(models[selectedModel].scale,
                glm::vec3(1.0f - ULEN, 1.0f - ULEN, 1.0f - ULEN));
    }

    // Press Shift + W to translate selected model in the -Z direction
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                glm::vec3(0.0f, 0.0f, -ULEN));
    }

    // Press Shift + A to translate selected model in the -X direction
    // Press A to rotate selected model by -5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                glm::vec3(-ULEN, 0.0f, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        models[selectedModel].rotation = glm::rotate(models[selectedModel].rotation,
                glm::radians(-5.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // Press Shift + S to translate selected model in the Z direction
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                glm::vec3(0.0f, 0.0f, ULEN));
    }

    // Press Shift + D to translate selected model in the X direction
    // Press D to rotate selected model by 5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        && ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))) {
        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                glm::vec3(ULEN, 0.0f, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        models[selectedModel].rotation = glm::rotate(models[selectedModel].rotation,
                glm::radians(5.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
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
        worldOrientation = glm::mat4(1.0f);
        camera = Camera(glm::vec3(0.0f, 0.1f, 2.0f));
    }

    // Press X to toggle textures
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        textureOn = 1-textureOn;
    }
}