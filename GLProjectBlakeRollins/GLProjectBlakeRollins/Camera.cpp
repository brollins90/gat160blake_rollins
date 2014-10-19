#include "Camera.h"

const float MOVEMENT_SPEED = 0.2f;
const float ROTATION_SPEED = 0.5f;
const float MAX_MOVEMENT = 20.0f;

Camera::Camera()
	: position(glm::vec3(0, 1, 7)),
	direction(glm::vec3(0, 0, -1)),
	UP(glm::vec3(0,1,0))
{
	invertLook = -1;
	right = glm::cross(direction, UP);
}


glm::mat4 Camera::getWorldToViewMatrix()
{
	return glm::lookAt(position, position + direction, UP);
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseChange = newMousePosition - oldMousePosition;

	if (glm::length(mouseChange) < MAX_MOVEMENT) {
		right = glm::cross(direction, UP);
		glm::mat4 tempRotation =
			glm::rotate(mouseChange.x * ROTATION_SPEED * invertLook, UP) *
			glm::rotate(mouseChange.y * ROTATION_SPEED * invertLook, right);

		direction = glm::mat3(tempRotation) * direction;
	}
	oldMousePosition = newMousePosition;
}

void Camera::moveForward()
{
	position += direction * MOVEMENT_SPEED;
}
void Camera::moveBackward()
{
	position -= direction * MOVEMENT_SPEED;
}
void Camera::moveLeft()
{
	position -= right * MOVEMENT_SPEED;
}
void Camera::moveRight()
{
	position += right * MOVEMENT_SPEED;
}
void Camera::moveUp()
{
	position += UP * MOVEMENT_SPEED;
}
void Camera::moveDown()
{
	position -= UP * MOVEMENT_SPEED;
}

