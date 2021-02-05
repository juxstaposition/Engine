#version 450 core

layout(location = 0) in vec3 uPos;
layout(location = 1) in vec2 uTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(uPos, 1.0);
    TexCoords = uTexCoords;
}