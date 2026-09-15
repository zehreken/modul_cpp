#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vLocalPos;
out vec2 vUV;

void main()
{
    vLocalPos = aPos;
    vUV = aUV;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}