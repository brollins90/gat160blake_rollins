#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
private:
	const glm::vec3 UP;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec2 oldMousePosition;
	float invertLook;

public:
	Camera();
	glm::mat4 getWorldToViewMatrix();
	void mouseUpdate(const glm::vec2& newMousePosition);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	
};

