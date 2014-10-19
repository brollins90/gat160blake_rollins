#version 430

in layout(location=0) vec3 v_position;
in layout(location=1) vec4 v_color;
in layout(location=2) vec3 v_normal;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform mat4 fullTransformMatrix;

out vec3 frag_normal;
out vec3 frag_position;

void main()
{
	vec4 v = vec4(v_position, 1.0f);
    gl_Position = fullTransformMatrix * v;
	frag_normal = v_normal;
	frag_position = v_position;
}