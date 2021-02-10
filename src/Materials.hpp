#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};


constexpr Material EMERALD
{
	glm::vec3(0.0215f,0.1745f,0.0215f),
	glm::vec3(0.07568f,0.61424f,0.07568f),
	glm::vec3(0.633f,0.727811f,0.633f),
	0.6f,
};


constexpr Material WHITE
{
	glm::vec3(1.f),
	glm::vec3(1.f),
	glm::vec3(1.f),
	1.f,
};

constexpr Material GREENRUBBER
{
	glm::vec3(0.0f, 0.05f, 0.0f),
	glm::vec3(0.4f, 0.5f, 0.4f),
	glm::vec3(0.04f, 0.7f, 0.04f),
	.078125f,
};



constexpr Material METALIC
{
	glm::vec3(0.83f, 0.83f, 0.83f),
	glm::vec3(0.83f, 0.83f, 0.83f),
	glm::vec3(0.83f, 0.83f, 0.83f),
	.078125f,
};