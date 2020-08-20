#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 lightSpaceMatrix;
uniform mat4 model = mat4(1.0f);

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}