#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
};

