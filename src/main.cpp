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

void createR1Model();
void createH6Model();
void createN5Model();
void create08Model();
void createK5Model();

void resetTextures(const Shader &shader);

void renderScene(Shader &shader, Model cube, Terrain terrain);
void renderAlphanum(Shader &shader, Model cube, Model sphere);


// Settings
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;
const float ULEN = 0.1f; // Unit Length

// Terrain settings
unsigned int TERRAIN_SIZE = 1000;
int OCTAVE_COUNT = 3;
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

// Alphanumeric class
struct Alphanum {
    std::vector<glm::mat4> letterTrans; // Vector of Cube Meshes Transformations
    std::vector<glm::mat4> numTrans;    // Vector of Number Meshes Transformations

    glm::mat4 letterAdjust; // Letter Adjustment Matrix
    glm::mat4 numAdjust;    // Number Adjustment Matrix

    glm::mat4 rotation;    // Model Rotation Matrix
    glm::mat4 scale;       // Model Scale Matrix
    glm::mat4 translation; // Model Translation Matrix

    glm::mat4 sphereScale;       // Sphere Scale Matrix
    glm::mat4 sphereTranslation; // Sphere Translation Matrix

    glm::mat4 numberRotation;    // Number Rotation Matrix
    glm::mat4 numberTranslation; // Number Translation Matrix

    glm::mat4 letterRotation;    // Letter Rotation Matrix
    glm::mat4 letterTranslation; // Letter Translation Matrix

    float rotationAngle; // Model Rotation Angle

    int animationTimeValue; // Model Movement Animation Current Time Value
};

// Alphanumeric models data structure
Alphanum models[5];

// Identity matrix
glm::mat4 id(1.0f);

// World Orientation
glm::mat4 worldOrientation(1.0f);

// Sphere Scaling
glm::mat4 sphereScale = glm::scale(id, glm::vec3(5.0f, 5.0f, 5.0f));

// Sphere Translation
glm::mat4 sphereTranslation = glm::translate(id, glm::vec3(0.0f, 6.0 * ULEN, 0.0f));

glm::mat4 projection(1.0f);
glm::mat4 view(1.0f);

glm::vec3 lightPos;

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
	glDepthFunc(GL_LESS);

    // Build and Compile our Shader Programs
    Shader lineShader("../res/shaders/line.vert", "../res/shaders/line.frag");
    Shader sceneShader("../res/shaders/scene.vert", "../res/shaders/scene.frag");
    Shader shadowShader("../res/shaders/shadow.vert", "../res/shaders/shadow.frag");

	// Lines
	std::vector<float> vertLines = {
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, ULEN * 5, 0.0f, 0.0f, 1.0f
    };
    std::vector<unsigned int> indexLines = {
            0, 3,   // Red x-axis line
            1, 4,   // Green y-axis line
            2, 5    // Blue z-axis line
    };
    Line line(vertLines, indexLines);

    // Cube model
    Model cube("../res/models/cube/cube.obj");
    // Sphere model
    Model sphere("../res/models/sphere/sphere.obj");

    // Initialize terrain
	Terrain terrain(TERRAIN_SIZE, cube, OCTAVE_COUNT, FREQUENCY, PERSISTENCE);

    // Initialize alphanumeric models
    // R1
    createR1Model();
    // H6
    createH6Model();
    // N5
    createN5Model();
    // 08
    create08Model();
    // K5
    createK5Model();

    // Textures
    Texture boxTexture("res/textures/box.jpg");
    Texture groundTexture("res/textures/ground.jpg");
    Texture shinyTexture("res/textures/yellow.png");
    Texture greyTexture("res/textures/grey.png");

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

        // Set camera/view matrix
//        glm::vec3 cameraPos(0.0f, camera.Position.y, 0.0f);
//        view = glm::lookAt(cameraPos, cameraPos + camera.Front, camera.Up);
        view = camera.get_view_matrix();

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

		// Render lines
		lineShader.use();
		lineShader.setMat4("projection", projection);
		lineShader.setMat4("view", view);
		line.draw(lineShader);

		// Reset framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // GLFW: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    line.deleteBuffers();

    // Deallocate height map resources
    terrain.Free();

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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        //Random Number Between -50 and 50 Normalized
        float randomX = (rand() % 101 + (-50)) / (1 / ULEN);
        float randomZ = (rand() % 101 + (-50)) / (1 / ULEN);

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
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {
        worldPos.y -= 1.0;
    }

    // Press Shift + A to translate selected model in the -X direction
    // Press A to rotate selected model by -5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
        worldPos.x -= 1.0;
    }

    // Press Shift + S to translate selected model in the Z direction
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
        worldPos.y += 1.0;
    }

    // Press Shift + D to translate selected model in the X direction
    // Press D to rotate selected model by 5.0 degrees
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
        worldPos.x += 1.0;
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

    // Forward Left Movement
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        models[selectedModel].rotation = glm::rotate(models[selectedModel].rotation,
                                                     glm::radians(5.0f),
                                                     glm::vec3(0.0f, 1.0f, 0.0f));
        models[selectedModel].rotationAngle = models[selectedModel].rotationAngle + 5.0f;

        float translationX = sin(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;
        float translationZ = cos(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;

        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                                                           glm::vec3(translationX, 0.0f, translationZ));

        translationX = translationX * 2;
        translationZ = translationZ * 2;

        moveModelForwardAnimation(translationX, translationZ);
    }


    // Forward Right Movement
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        models[selectedModel].rotation = glm::rotate(models[selectedModel].rotation,
                                                     glm::radians(-5.0f),
                                                     glm::vec3(0.0f, 1.0f, 0.0f));
        models[selectedModel].rotationAngle = models[selectedModel].rotationAngle - 5.0f;

        float translationX = sin(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;
        float translationZ = cos(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;

        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                                                           glm::vec3(translationX, 0.0f, translationZ));

        translationX = translationX * 2;
        translationZ = translationZ * 2;

        moveModelForwardAnimation(translationX, translationZ);
    }

    // Forward Model Movement with Legs
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        float translationX = sin(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;
        float translationZ = cos(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;

        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                                                           glm::vec3(translationX, 0.0f, translationZ));

        translationX = translationX * 2;
        translationZ = translationZ * 2;

        moveModelForwardAnimation(translationX, translationZ);
    }

    // Reverse Model Movement with Legs
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        float translationX = sin(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;
        float translationZ = cos(models[selectedModel].rotationAngle * (M_PI / 180)) * ULEN;

        models[selectedModel].translation = glm::translate(models[selectedModel].translation,
                                                           glm::vec3(-translationX, 0.0f, -translationZ));

        translationX = translationX * 2;
        translationZ = translationZ * 2;

        moveModelForwardAnimation(translationX, translationZ);
    }

    // Reset world orientation and camera by pressing Home button
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        worldOrientation = glm::mat4(1.0f);
        camera = Camera(glm::vec3(0.0f, 0.1f, 2.0f));
        createR1Model();
        createH6Model();
        createN5Model();
        create08Model();
        createK5Model();
    }

    // Press X to toggle textures
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        textureOn = 1 - textureOn;
    }
}

void moveModelForwardAnimation(float translationX, float translationZ) {
    if (models[selectedModel].animationTimeValue == 0) {
        // Translate Letter Leg Forward
        models[selectedModel].letterTranslation = glm::translate(models[selectedModel].letterTranslation,
                                                                 glm::vec3(translationX, 0.0f, translationZ));
        // Rotate Letter Leg Backward to -23 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(-23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Translate Number Leg Backward
        models[selectedModel].numberTranslation = glm::translate(models[selectedModel].numberTranslation,
                                                                 glm::vec3(-translationX, 0.0f, -translationZ));
        // Rotate Number Leg Forward to 23 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 1) {
        // Rotate Letter Leg Backward to -45 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(-45.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate Number Leg Forward to 45 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(45.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 2) {
        // Rotate Letter Leg Backward to -23 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(-23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate Number Leg Forward to 23 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 3) {
        // Original Letter Translation
        models[selectedModel].letterTranslation = id;
        // Original Letter Rotation
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(0.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Original Number Translation
        models[selectedModel].numberTranslation = id;
        // Original Number Rotation
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(0.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 4) {
        // Translate Letter Leg Forward
        models[selectedModel].letterTranslation = glm::translate(models[selectedModel].letterTranslation,
                                                                 glm::vec3(-translationX, 0.0f, -translationZ));
        // Rotate Letter Leg Backward to -23 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Translate Number Leg Backward
        models[selectedModel].numberTranslation = glm::translate(models[selectedModel].numberTranslation,
                                                                 glm::vec3(translationX, 0.0f, translationZ));
        // Rotate Number Leg Forward to 23 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(-23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 5) {
        // Rotate Letter Leg Backward to -23 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(45.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate Number Leg Forward to 23 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(-45.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 6) {
        // Rotate Letter Leg Backward to -23 Degrees
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate Number Leg Forward to 23 Degrees
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(-23.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Increment Animation Time
        models[selectedModel].animationTimeValue++;
    } else if (models[selectedModel].animationTimeValue == 7) {
        // Original Letter Translation
        models[selectedModel].letterTranslation = id;
        // Original Letter Rotation
        models[selectedModel].letterRotation = glm::rotate(id,
                                                           glm::radians(0.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Original Number Translation
        models[selectedModel].numberTranslation = id;
        // Original Number Rotation
        models[selectedModel].numberRotation = glm::rotate(id,
                                                           glm::radians(0.0f),
                                                           glm::vec3(1.0f, 0.0f, 0.0f));
        // Set Time back to 0 (End of Animation)
        models[selectedModel].animationTimeValue = 0;
    }
}

void createK5Model() {
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    glm::mat4 shearKTop = {
            1.0f, 0.0f, 0.0f, 0.0f,
            2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 3.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearKTop
    );
    glm::mat4 shearKBottom = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -2.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[4].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f)) * shearKBottom
    );

    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5 * ULEN, 4.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[4].numTrans.push_back(
            id
    );
    models[4].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[4].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[4].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[4].scale = id;
    models[4].translation = glm::translate(id, glm::vec3(20 * ULEN, 0.0f, -20 * ULEN));
    models[4].rotation = id;
    models[4].rotationAngle = 0.0f;
    models[4].letterTranslation = id;
    models[4].numberTranslation = id;
    models[4].sphereScale = sphereScale;
    models[4].sphereTranslation = sphereTranslation;
    models[4].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[4].animationTimeValue = 0;
    models[4].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void create08Model() {
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].letterTrans.push_back(
            id
    );
    models[3].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );

    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(-1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(1 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[3].numTrans.push_back(
            id
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );
    models[3].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );

    models[3].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[3].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[3].scale = id;
    models[3].translation = glm::translate(id, glm::vec3(-20 * ULEN, 0.0f, -20 * ULEN));
    models[3].rotation = id;
    models[3].letterTranslation = id;
    models[3].numberTranslation = id;
    models[3].rotationAngle = 0.0f;
    models[3].sphereScale = sphereScale;
    models[3].sphereTranslation = sphereTranslation;
    models[3].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[3].animationTimeValue = 0;
    models[3].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void createN5Model() {
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    glm::mat4 shearN = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -3.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[2].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.5 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)) * shearN
    );

    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(0.5 * ULEN, 4.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(2.0f, 1.0f, 1.0f))
    );
    models[2].numTrans.push_back(
            id
    );
    models[2].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[2].letterAdjust = glm::translate(id, glm::vec3(-2.5 * ULEN, 0.0f, 0.0f));
    models[2].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[2].scale = id;
    models[2].translation = glm::translate(id, glm::vec3(-20 * ULEN, 0.0f, 20 * ULEN));
    models[2].rotation = id;
    models[2].letterTranslation = id;
    models[2].numberTranslation = id;
    models[2].rotationAngle = 0.0f;
    models[2].sphereScale = sphereScale;
    models[2].sphereTranslation = sphereTranslation;
    models[2].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[2].animationTimeValue = 0;
    models[2].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void createH6Model() {
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2 * ULEN, 0.0f))
    );
    models[1].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );

    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(-1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 2.0 * ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(0.0f, 4.0 * ULEN, 0.0f))
    );
    models[1].numTrans.push_back(
            id
    );
    models[1].numTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );

    models[1].letterAdjust = glm::translate(id, glm::vec3(-2.0 * ULEN, 0.0f, 0.0f));
    models[1].numAdjust = glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f));

    models[1].scale = id;
    models[1].translation = glm::translate(id, glm::vec3(20 * ULEN, 0.0f, 20 * ULEN));
    models[1].rotation = id;
    models[1].letterTranslation = id;
    models[1].numberTranslation = id;
    models[1].rotationAngle = 0.0f;
    models[1].sphereScale = sphereScale;
    models[1].sphereTranslation = sphereTranslation;
    models[1].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[1].animationTimeValue = 0;
    models[1].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void createR1Model() {
    models[0].letterTrans.push_back(
            glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 4.0f * ULEN, 0.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0 * ULEN, 2.0 * ULEN, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 3.0f, 1.0f))
    );
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(1.0 * ULEN, 2.0 * ULEN, 0.0f))
    );
    glm::mat4 shearR = {
            1.0f, 0.0f, 0.0f, 0.0f,
            -1.0, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    models[0].letterTrans.push_back(
            glm::translate(id, glm::vec3(2.0 * ULEN, 0.0f, 0.0f)) *
            glm::scale(id, glm::vec3(1.0f, 2.0f, 1.0f)) * shearR
    );

    models[0].numTrans.push_back(glm::scale(id, glm::vec3(1.0f, 5.0f, 1.0f)));

    models[0].letterAdjust = glm::translate(id, glm::vec3(-3 * ULEN, 0.0f, 0.0f));
    models[0].numAdjust = glm::translate(id, glm::vec3(ULEN, 0.0f, 0.0f));

    models[0].scale = id;
    models[0].translation = id;
    models[0].rotation = id;
    models[0].letterTranslation = id;
    models[0].numberTranslation = id;
    models[0].rotationAngle = 0.0f;
    models[0].sphereScale = sphereScale;
    models[0].sphereTranslation = sphereTranslation;
    models[0].numberRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    models[0].animationTimeValue = 0;
    models[0].letterRotation = glm::rotate(id, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


