#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OrtographicCamera
{
public:
	OrtographicCamera(float left, float right, float bottom, float top);
	~OrtographicCamera();


	void resize(float left, float right, float bottom, float top);
	void setPosition(const glm::vec3& position);
	void setRotation(const float& rotation);
	void setDistance(const float& value);
	void addYPosition(const float& dy);
	void addXPosition(const float& dx);
	void addXYPosition(const glm::vec2&  value);

	glm::vec3& getPosition() { return _position; };
	float getRotation() { return _rotation; };
	float getDistance() { return _distance; };
	glm::mat4 getProjMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjViewMatrix();

private:
	void compute();

	glm::mat4 _projMatrix;
	glm::mat4 _viewMatrix;
	glm::mat4 _projViewMatrix;

	glm::vec3 _position = { 0.0f,0.0f,0.0f };
	float _rotation = 0.0f;
	float _distance = 1.0f;

	bool _recompute = true;
};