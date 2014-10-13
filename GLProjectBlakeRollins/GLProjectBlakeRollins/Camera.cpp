#include "Camera.h"

const float ROTATIONAL_SPEED = 0.5f;

Camera::Camera() 
	: viewDirection(0.0f, 0.0f, -1.0f),
	UP(0.0f,1.0f,0.0f)
{
	invertView = -1;
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	glm::vec3 cameraChangeX = viewDirection = glm::mat3(glm::rotate(mouseDelta.x * ROTATIONAL_SPEED * invertView, UP)) * viewDirection;
	glm::vec3 toRotateAroundY = glm::cross(cameraChangeX, UP);
	viewDirection = glm::mat3(glm::rotate(mouseDelta.y * ROTATIONAL_SPEED * invertView, toRotateAroundY)) * viewDirection;
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}
