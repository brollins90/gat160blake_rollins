#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include <glm\glm.hpp>

struct ProjectModel
{
	glm::vec3 ambientLightColor = glm::vec3(0.2f, 0.2f, 0.3f);
	glm::vec3 diffuseLightColor = glm::vec3(0.2f, 0.2f, 0.3f);
	glm::vec3 specularLightColor = glm::vec3(0.2f, 0.2f, 0.3f);
	glm::vec3 lightPosition = glm::vec3(5.0f, 5.0f, -2.0f);
};

#endif
