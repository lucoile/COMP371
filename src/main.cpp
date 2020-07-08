#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
#include "camera.h"
#include "texture.h"
#include <iostream>
/*
 *If the user want to resize the window
 *FrameBuffer size Whenever the window changes in size ,GLFW calls this function and
 *fills the proper arguments for us
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Input to to control GLFW
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// Main Function to instantiate GLWF
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create a window Requires width , height , name ,
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    //Unable to create the window
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //GLAD manages and link functions for openGL
    //Call GLAD function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //Before rendering we need to setup a glViewport ,_,_,Width,Height of the rendering window
    glViewport(0, 0, 800, 600);
    //Create vertices
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };




    //We do have to tell GLFW we want to call this function on every window resize by registering it
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Declaring a Vertex Buffer object
    unsigned int VBO;
    //Assigning an ID to it
    glGenBuffers(1, &VBO);
    //Binding buffers at once as long as they have a different buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData function that copies the previously defined vertex data into the buffer's memory:
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Compiling a shader
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    // In order for OpenGL to use the shader it has to dynamically compile it at run-time from its source cod
    unsigned int vertexShader;
    //We store the vertex shader as an unsigned int and create the shader with glCreateShader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //Check if compilation was successful
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &vertexShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
    // If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    //To keep the window on we need to keep drawing the image
    //i.e mainloop
    while(!glfwWindowShouldClose(window))
    {
        //Process input
        processInput(window);
        //Check events
        glfwPollEvents();
        /** Changing colors test then apply glfwSwapBuffer**/

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //Swap Buffers
        glfwSwapBuffers(window);

    }
    //We need to reallocate the resources after we are done from the mainloop
    glfwTerminate();

    return 0;
}

//Shaders definition : small programs that run in parallel on GPU
//The main focus of openGL is to transform the 3D coordinates into 2D window
//Second main purpose is to transform the 2D coordinates into actual colored pixels

/**Steps in OpenGl
 * 1) Vertex shader (3D coordinates of vertices)
 * 2) Primitive assembly ex a triangle the output is passed to geometry shader
 * the geometry shader has the ability to generate other shapes by emitting new
 * vertices to form new primitive
 * 3) Fragment shader : to calculate the final color of the pixel **/

/**
 * Vertex buffer objects (VBO) can store a large number of vertices in the GPU's
 * memory.The advantage of VBO is to send large chunk of data into the GPU
 * This buffer has a unique ID corresponding to that buffer, so we can generate one with a buffer ID using the glGenBuffers function:
 */
