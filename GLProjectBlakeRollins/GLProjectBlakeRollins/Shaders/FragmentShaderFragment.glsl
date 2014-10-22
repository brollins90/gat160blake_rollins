#version 430

in vec3 fragmentNormalWorld;
in vec3 fragmentPositionWorld;

uniform vec4 ambientLightColor;
uniform vec3 lightPositionWorld;

out vec4 out_color;

void main()
{
	vec3 lightVector = normalize(lightPositionWorld - fragmentPositionWorld);
	float brightness = dot(lightVector, fragmentNormalWorld);
    vec4 diffuseLightIntensity = vec4(brightness, brightness, brightness, 1.0);

	out_color = clamp(diffuseLightIntensity, 0, 1) + ambientLightColor;
}