#version 430

in vec3 fragmentNormalWorld;
in vec3 fragmentPositionWorld;

uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform vec3 lightPositionWorld;
uniform vec3 cameraPositionWorld;

out vec4 out_color;

void main()
{
	// Diffuse light
	vec3 lightVectorWorld = normalize(lightPositionWorld - fragmentPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(fragmentNormalWorld));
    vec4 diffuseLightIntensity = vec4(brightness, brightness, brightness, 1.0);
	vec4 diffuseColor = diffuseLightIntensity * diffuseLightColor;

	// Specular light
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalize(fragmentNormalWorld));
	vec3 eyeVectorWorld = normalize(cameraPositionWorld - fragmentPositionWorld);
	float specularity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specularity = pow(specularity, 25);
	
	vec4 specularLightIntensity = vec4(specularity, specularity, specularity, 1.0);
	vec4 specularColor = specularLightIntensity * specularLightColor;

	out_color = ambientLightColor + clamp(diffuseColor, 0, 1) + clamp(specularColor, 0, 1);
}