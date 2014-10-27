#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 vertexUvModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform vec3 lightPositionWorld;
uniform vec3 cameraPositionWorld;
uniform float specularExponent;

out vec4 fragmentColor;

void main()
{
    gl_Position = modelToProjectionMatrix * vertexPositionModel;
	
	vec3 vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
	vec3 vertexNormalWorld = mat3(modelToWorldMatrix) * vertexNormalModel;
	
	// Diffuse light
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(vertexNormalWorld));
    vec4 diffuseLightIntensity = vec4(brightness, brightness, brightness, 1.0);
	vec4 diffuseColor = diffuseLightIntensity * diffuseLightColor;

	// Specular light
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalize(vertexNormalModel));
	vec3 eyeVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specularity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specularity = pow(specularity, specularExponent);
	
	vec4 specularLightIntensity = vec4(specularity, specularity, specularity, 1.0);
	vec4 specularColor = specularLightIntensity * specularLightColor;

	fragmentColor = ambientLightColor + clamp(diffuseColor, 0, 1) + clamp(specularColor, 0, 1);
}