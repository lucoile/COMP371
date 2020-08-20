// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../lib/libnoise/include/noise/noise.h"
#include "../lib/libnoise/include/noise/noiseutils.h"

#include "Utilities/shader_m.h"
#include "Utilities/camera.h"
#include "Utilities/model.h"
#include "Utilities/texture.h"
#include "Utilities/terrain.h"


#include "Line/line.h"

#include <iostream>
#include <istream>
#include <vector>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void process_input(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void moveModelForwardAnimation(float translationX, float translationZ);

void resetTextures(const Shader &shader);

void renderScene(Shader &shader, Model cube, Terrain terrain);
void renderAlphanum(Shader &shader, Model cube, Model sphere);


// Settings
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;
const float ULEN = 0.1f; // Unit Length

// Terrain settings
unsigned int TERRAIN_SIZE = 1000;
unsigned int RENDER_SIZE = 100;
int OCTAVE_COUNT = 6;
float FREQUENCY = 5.0;
float PERSISTENCE = 0.25;
glm::vec2 worldPos(0.0f);

// Camera
Camera camera(glm::vec3(0.0f, 10.0 * ULEN, 2.0f));
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
unsigned int textureOn = 1;

// Alphanumeric models data structure
Alphanum models[5];

// Identity matrix
glm::mat4 id(1.0f);

// World Orientation
glm::mat4 worldOrientation(1.0f);
glm::mat4 worldRotation(1.0f);

// Sphere Scaling
glm::mat4 sphereScale = glm::scale(id, glm::vec3(5.0f, 5.0f, 5.0f));

// Sphere Translation
glm::mat4 sphereTranslation = glm::translate(id, glm::vec3(0.0f, 6.0 * ULEN, 0.0f));

glm::mat4 projection(1.0f);
glm::mat4 view(1.0f);

glm::vec3 lightPos;

Camera_Movement lastMove;

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
	glDepthFunc(GL_LESS);

    // Build and Compile our Shader Programs
    Shader sceneShader("../res/shaders/scene.vert", "../res/shaders/scene.frag");
    Shader shadowShader("../res/shaders/shadow.vert", "../res/shaders/shadow.frag");

    // Cube model
    Model cube("../res/models/cube/cube.obj");
    // Sphere model
    Model sphere("../res/models/sphere/sphere.obj");

    // Initialize terrain
	Terrain terrain(TERRAIN_SIZE, cube, sphere, OCTAVE_COUNT, FREQUENCY, PERSISTENCE);


    // Textures
    Texture boxTexture("res/textures/box.jpg");
    Texture groundTexture("res/textures/ground.jpg");
    Texture shinyTexture("res/textures/yellow.png");
    Texture greyTexture("res/textures/grey.png");
    Texture dirtTexture("res/textures/dirt.jpg");
    Texture grassTexture("res/textures/grass.png");
    Texture leavesTextures("res/textures/leaves.jpg");
    Texture treeTextures("res/textures/tree-trunk.jpg");

	// bind textures
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	boxTexture.bind();

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	groundTexture.bind();

	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	shinyTexture.bind();

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	greyTexture.bind();

    glActiveTexture(GL_TEXTURE5);
    glEnable(GL_TEXTURE_2D);
    dirtTexture.bind();

    glActiveTexture(GL_TEXTURE6);
    glEnable(GL_TEXTURE_2D);
    grassTexture.bind();

    glActiveTexture(GL_TEXTURE7);
    glEnable(GL_TEXTURE_2D);
    leavesTextures.bind();

    glActiveTexture(GL_TEXTURE8);
    glEnable(GL_TEXTURE_2D);
    treeTextures.bind();


	// Depth map frame buffer
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// Create texture for the depth map
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Attach depth map to the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	// Set color buffer to none
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Check that framebuffer is complete
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, depthMap);


    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Per Frame Time Logic
        auto currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        process_input(window);

        glClearColor(0.0f, 0.0784f, 0.1607f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set projection matrix
        projection = glm::perspective(45.0f, (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,100.0f);

        // Make sure camera isn't colliding with anything
		int cameraX = int(round(camera.Position.x));
		int cameraZ = int(round(camera.Position.z));

        // Update worldPos
        worldPos.x = cameraX;
		worldPos.y = cameraZ;

        // Set camera/view matrix
		float terrainHeight = terrain.GetValue(worldPos.x, worldPos.y);
		glm::vec3 Position = glm::vec3(0.0f,(round(terrainHeight * 10.0f) / 10.0f) + (5.0f * ULEN),0.0f);
		view = glm::lookAt(Position, Position + camera.Front, camera.Up);

		// Set orthographic frustum for shadows
		float near_plane = 1.0f, far_plane = 5.0f;
		glm::mat4 lightProjection = glm::frustum(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		// View matrix from light perspective
		// Shadows only render correctly if the light is offset by some amount >0
		lightPos = glm::vec3(0.001f, 50.0 * ULEN, 0.001f);
		glm::mat4 lightView = glm::lookAt(lightPos,
				glm::vec3( 0.0f, 0.0f,  0.0f),
				glm::vec3( 0.0f, 1.0f,  0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		shadowShader.use();
		shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// Bind grey texture
		// Not sure why but if we don't bind it here it just renders as red
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		greyTexture.bind();

        glActiveTexture(GL_TEXTURE8);
        glEnable(GL_TEXTURE_2D);
        treeTextures.bind();

		// Set viewport size and bind depth map frame buffer
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene with shadow/depth map shader
		renderScene(shadowShader, cube, terrain);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Reset window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate scene shader
        sceneShader.use();
        sceneShader.setMat4("projection", projection);
		sceneShader.setMat4("view", view);
		sceneShader.setMat4("world", worldOrientation);
		sceneShader.setVec3("viewPos", camera.Position);
		sceneShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// Set shadow map
		sceneShader.setInt("shadowMap", 4);

		// Render the scene using shadow map
		renderScene(sceneShader, cube, terrain);

		// Reset framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void renderScene(Shader &shader, Model cube, Terrain terrain)
{
//	renderAlphanum(shader, cube, sphere);

	resetTextures(shader);
	shader.setInt("material.diffuse", 3);
	shader.setVec3("material.ambient", glm::vec3(1.0f));
	shader.setVec3("light.ambient", glm::vec3(1.0f));

	terrain.Render(shader, worldOrientation, worldPos);
}

void renderAlphanum(Shader &shader, Model cube, Model sphere)
{
	if (textureOn == 1) {
		shader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
		shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
		shader.setFloat("material.shininess", 32.0f);

		// light properties
		shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		shader.setVec3("light.position", 0.0f, 30.0 * ULEN, 0.0f);
	} else {
		resetTextures(shader);
	}

	// render each alphanumeric pair by looping through the array of models
	for (unsigned int j = 0; j < 5; j++) {
		// add box texture
		if (textureOn == 1) {
			shader.setInt("material.diffuse", 0);
		} else {
			shader.setInt("material.diffuse", 3);
			resetTextures(shader);
		}
		// draw the letter
		for (unsigned int i = 0; i < models[j].letterTrans.size(); i++) {
			glm::mat4 model =
					worldOrientation * models[j].translation * models[j].letterTranslation * models[j].rotation *
					models[j].scale * models[j].letterAdjust * models[j].letterRotation * models[j].letterTrans[i];
			shader.setMat4("model", model);

			cube.Draw(shader, type);
		}

		// add shiny texture
		if (textureOn == 1) {
			shader.setInt("material.diffuse", 2);
			shader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("material.shininess", 64.0f);
		} else {
			shader.setInt("material.diffuse", 3);
			resetTextures(shader);
		}

		// draw the number
		for (unsigned int i = 0; i < models[j].numTrans.size(); i++) {
			glm::mat4 model =
					worldOrientation * models[j].translation * models[j].numberTranslation * models[j].rotation *
					models[j].scale * models[j].numAdjust * models[j].numberRotation * models[j].numTrans[i];
			shader.setMat4("model", model);

			cube.Draw(shader, type);
		}

		// Change to grey diffuse texture and reset material
		resetTextures(shader);
		shader.setInt("material.diffuse", 3);

		// Draw Sphere
		for (unsigned int i = 0; i < models[j].letterTrans.size(); i++) {
			glm::mat4 model =
					worldOrientation * models[j].translation * models[j].rotation *
					models[j].scale * models[j].sphereTranslation * models[j].sphereScale;
			shader.setMat4("model", model);

			sphere.Draw(shader, type);
		}
	}

}

void resetTextures(const Shader &shader) {
    shader.setVec3("material.ambient", 0.3f, 0.3f, 0.3f);
    shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
    shader.setFloat("material.shininess", 32.0f);

    // light properties
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.position", lightPos);
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
    if (firstMouse)
    {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    auto xoffset = float(xpos - lastX);
    auto yoffset = float(lastY - ypos); // Reversed since y-coordinates go from bottom to top

    lastX = float(xpos);
    lastY = float(ypos);

	camera.process_mouse_movement(xoffset, yoffset);
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.process_keyboard_input(FORWARD, deltaTime); lastMove = FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.process_keyboard_input(BACKWARD, deltaTime); lastMove = BACKWARD;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.process_keyboard_input(LEFT, deltaTime); lastMove = LEFT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.process_keyboard_input(RIGHT, deltaTime); lastMove = RIGHT;

    // Press X to toggle textures
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        textureOn = 1 - textureOn;
    }
}