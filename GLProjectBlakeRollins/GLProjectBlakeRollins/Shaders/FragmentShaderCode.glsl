#version 430

in vec3 frag_normal;
in vec3 frag_position;

out vec4 out_color;

uniform vec3 lightPosition;

void main()
{
	vec3 lightVector = normalize(lightPosition - frag_position);
	float brightness = dot(lightVector, frag_normal);
    out_color = vec4(brightness, brightness, brightness, 1.0f);
}