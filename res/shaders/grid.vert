#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 vertexColor;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 world;



void main()
{
	gl_Position =  projection * view * world * model * vec4(aPos, 1.0f);
	vertexColor = aColor;
	TexCoords = aTexCoords;
}