#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
private:
	int invertView;
	glm::vec2 oldMousePosition;
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
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

