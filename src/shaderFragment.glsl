#version 450 core

in vec3 Normal;  
in vec3 FragPos;  
in vec3 vertexPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 objectColor;

void main()
{
    float specularStrength = 0.5;
    vec3 ambient = vec3(0.5);
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * vec3(1.0); 

    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    
    fragColor = vec4(resultColor, 1.0);
}