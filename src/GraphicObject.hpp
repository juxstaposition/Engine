#pragma once
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include "Model.hpp"


#include "Materials.hpp"

class GraphicObject
{
public:
	GraphicObject(std::string objectLocation);
	~GraphicObject();



	void createObjectInstance(glm::mat4& modelMat, Material material);
	void deleteObjectInstance(uint8_t instance);
	void setObjectMat(glm::mat4& modelMat, uint8_t instance);

	struct ObjectInstance {
		glm::mat4 modelMat;
		Material material;
	};

	Model model;
	std::vector<ObjectInstance> instances;

};

