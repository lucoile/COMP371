#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 Color;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model = mat4(1.0f);
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * world * model * vec4(aPos, 1.0f);
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Color = aColor;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}