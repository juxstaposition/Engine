#version 450 core

layout(location = 0) in vec3 viPosition;
layout(location = 1) in vec2 viUvs;
layout(location = 2) in vec3 viNormals;

out vec3 vertexPos;
out vec3 Normal;
out vec3 FragPos;  

// Values that stay constant for the whole mesh.
uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;


void main()
{
    
    FragPos = vec3(modelMat * vec4(viPosition, 1.0) );
    Normal = viNormals;

    gl_Position = projMat * viewMat * modelMat * vec4(viPosition, 1.0); 
    vertexPos = viPosition;
}