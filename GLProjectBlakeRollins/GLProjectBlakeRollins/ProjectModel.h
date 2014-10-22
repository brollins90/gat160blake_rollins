#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H

#include <glm\glm.hpp>

struct ProjectModel
{
	glm::vec4 ambientLightColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
	glm::vec4 diffuseLightColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
	glm::vec4 specularLightColor = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(5.0f, 5.0f, -2.0f);
	float specularExponent = 10.0;
};

#endif
