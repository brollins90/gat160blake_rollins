#include "Camera.h"

const float ROTATIONAL_SPEED = 0.5f;
const float MOVEMENT_SPEED = 0.1f;

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

void Camera::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}
void Camera::moveBackward()
{
	position -= MOVEMENT_SPEED * viewDirection;
}
void Camera::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}
void Camera::moveDown()
{
	position -= MOVEMENT_SPEED * UP;
}
void Camera::strafeLeft()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position -= MOVEMENT_SPEED * strafeDirection;
}
void Camera::strafeRight()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += MOVEMENT_SPEED * strafeDirection;
}