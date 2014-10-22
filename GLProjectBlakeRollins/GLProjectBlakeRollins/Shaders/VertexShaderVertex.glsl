#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;

uniform vec4 ambientLightColor;
uniform vec3 lightPositionWorld;
uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec4 fragmentColor;

void main()
{
    gl_Position = modelToProjectionMatrix * vertexPositionModel;

	vec3 trans_normal = mat3(modelToWorldMatrix) * vertexNormalModel;
	vec3 trans_position = vec3(modelToWorldMatrix * vertexPositionModel);

	vec3 lightVector = normalize(lightPositionWorld - trans_position);
	float brightness = dot(lightVector, trans_normal);
    vec4 diffuseLightIntensity = vec4(brightness, brightness, brightness, 1.0);

	fragmentColor = clamp(diffuseLightIntensity, 0, 1) + ambientLightColor;

}
