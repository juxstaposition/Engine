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
    instancesNum++;
    instances.push_back(ObjectInstance{ modelMat , material });

}

void GraphicObject::deleteObjectInstance(uint8_t instance)
{
    if (instance < instancesNum)
    {
        instances.erase(instances.begin() + instance);
        instancesNum--;
    }
}


void GraphicObject::setObjectMat(uint8_t instance, glm::mat4& modelMat)
{
    if (instance < instancesNum)
    {
        instances.at(instance).modelMat = modelMat;
    }
}