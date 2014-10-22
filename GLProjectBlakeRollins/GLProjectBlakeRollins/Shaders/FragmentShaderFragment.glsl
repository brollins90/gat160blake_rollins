#version 430

in vec3 fragmentNormalWorld;
in vec3 fragmentPositionWorld;

uniform vec3 ambientLight;
uniform vec3 lightPosition;

out vec4 out_color;

void main()
{
	vec3 lightVector = normalize(lightPosition - fragmentPositionWorld);
	float brightness = dot(lightVector, fragmentNormalWorld);
    out_color = vec4(brightness, brightness, brightness, 1.0);
}