#include "OrtographicCamera.hpp"

OrtographicCamera::OrtographicCamera(float left, float right, float bottom, float top) :
	_projMatrix(glm::ortho(left, right, bottom, top, -300.0f, 300.0f)), 
	_viewMatrix(glm::mat4(1.0f) )
{
	_projViewMatrix = _projMatrix * _viewMatrix;
} 

OrtographicCamera::~OrtographicCamera()
{
}

void OrtographicCamera::resize(float left, float right, float bottom, float top)
{
	_projMatrix = glm::ortho(left, right, bottom, top, -300.0f, 300.0f);
	_viewMatrix = glm::mat4(1.0f);
}

glm::mat4 OrtographicCamera::getProjMatrix()
{
	if (_recompute) 
	{
		compute();
		_recompute = false;
	}
	return _projMatrix;
};
glm::mat4 OrtographicCamera::getViewMatrix()
{
	if (_recompute) 
	{
		compute();
		_recompute = false;
	}
	return _viewMatrix;
};
glm::mat4 OrtographicCamera::getProjViewMatrix()
{
	if (_recompute)
	{
		compute();
		_recompute = false;
	}
	return _projViewMatrix; 
};

void OrtographicCamera::setPosition(const glm::vec3& position) 
{ 
	_position = position;
	_recompute = true;
};

void OrtographicCamera::setDistance(const float& value)
{
	// check if scale value is larger than 0.f 
	_distance += value;
	
	_projMatrix = glm::scale(_projMatrix, glm::vec3(value, value, value));
}

void OrtographicCamera::addXPosition(const float& dx)
{
	_position.x += dx;
	_recompute = true;
}

void OrtographicCamera::addYPosition(const float& dy)
{
	_position.y += dy;
	_recompute = true;
}

void OrtographicCamera::addXYPosition(const glm::vec2& value)
{
	addXPosition(value.x);
	addYPosition(value.y);
}


void OrtographicCamera::setRotation(const float& rotation) 
{ 
	_rotation = rotation;
	_recompute = true;
};


void OrtographicCamera::compute()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
		glm::rotate(glm::mat4(1.0f), _rotation, glm::vec3(0, 0, 1) );

	_viewMatrix = glm::inverse(transform);

}