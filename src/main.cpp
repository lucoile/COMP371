// COMP371 - Project
// main.cpp
// Created by Thomas Buffard on 6/26/20.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct ShaderProgramSource{
    std::string vertexSource;
    std::string fragmentSource;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static unsigned int compile_shader(unsigned int type, const std::string& source);
static unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader);
static ShaderProgramSource parse_shader(const std::string& filepath);

// Variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, const char * argv[]) {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS

    // Create GLFW window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Register framebuffer_size_callback function with window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Make sure GLAD is initialized
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ShaderProgramSource source = parse_shader("../res/shaders/SimpleRed.shader");
    unsigned int shaderProgram = create_shader(source.vertexSource, source.fragmentSource);

    float vertices[] = {
            // first triangle
            -1.0f, -0.5f, 0.0f,  // p1
            -0.5f,  0.5f, 0.0f,  // p2
            -0.5f,  0.5f, 0.0f,  // p2
             0.0f, -0.5f, 0.0f,  // p3
             0.0f, -0.5f, 0.0f,  // p3
             0.5f,  0.5f, 0.0f,  // p4
             0.5f,  0.5f, 0.0f,  // p4
             1.0f, -0.5f, 0.0f,  // p5
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Render loop
    // Check if window has been instructed to close
    while(!glfwWindowShouldClose(window))
    {
        // Process
        processInput(window);

        // Render Here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 8);
        // glBindVertexArray(0); // no need to unbind it every time

        // Swap Front and Back Buffers
        glfwSwapBuffers(window);
        // Poll for and Process Events
        glfwPollEvents();
    }

    // Clean all the GLFW resources
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// Function called when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Set the viewport window
    glViewport(0, 0, width, height);
}

// Function to process input
void processInput(GLFWwindow* window)
{
    // Close the window when user presses escape
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Function that compiles a shader
static unsigned int compile_shader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    //Error Handling if Shader does not compile properly
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// Function that creates Vertex and Fragment shaders
static unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

    //Attach Shaders to Program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //Link and Validate Program
    glLinkProgram(program);
    glValidateProgram(program);

    //Delete Shaders (program has them now)
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// Parse shaders from file
static ShaderProgramSource parse_shader(const std::string& filepath){
    std::ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; //one for vertex, one for fragment
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}
