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
	if (glm::length(mouseDelta) < 50.0f)
	{
		vecRight = glm::cross(viewDirection, UP);
		glm::mat4 rotator =
			glm::rotate(mouseDelta.x * ROTATIONAL_SPEED * invertView, UP) *
			glm::rotate(mouseDelta.y * ROTATIONAL_SPEED * invertView, vecRight);
		viewDirection = glm::mat3(rotator) * viewDirection;
	}
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
	position -= MOVEMENT_SPEED * vecRight;
}
void Camera::strafeRight()
{
	position += MOVEMENT_SPEED * vecRight;
}