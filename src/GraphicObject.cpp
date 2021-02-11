#include "GraphicObject.hpp"

GraphicObject::GraphicObject(std::string objectLocation)
{
    // Load obj file
    model.loadObjFile(objectLocation);

    // Create model in GPU memory
    model.initialzeModel();
}

GraphicObject::~GraphicObject()
{

}


void GraphicObject::createObjectInstance(glm::mat4& modelMat, Material material)
{
    instances.push_back(ObjectInstance{ modelMat , material });
}

void GraphicObject::deleteObjectInstance(uint8_t instance)
{
    if (instance < instances.size())
    {
        instances.erase(instances.begin() + instance);
    }
}


void GraphicObject::setObjectMat(glm::mat4& modelMat, uint8_t instance)
{
    if (instance < instances.size())
    {
        instances.at(instance).modelMat = modelMat;
    }
}