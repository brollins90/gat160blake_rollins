#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 vecRight;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
	int invertView;
public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	void mouseUpdate(const glm::vec2& newMousePosition);
	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();
	void strafeLeft();
	void strafeRight();
};

