#version 430

in layout(location=0) vec3 v_position;
in layout(location=1) vec4 v_color;

out vec3 frag_color;

void main()
{
	vec4 v = vec4(v_position, 1.0f);
    gl_Position = v;
	frag_color = v_color;
}