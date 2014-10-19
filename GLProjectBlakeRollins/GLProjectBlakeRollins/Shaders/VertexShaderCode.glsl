#version 430

in layout(location=0) vec3 v_position;
in layout(location=1) vec3 v_color;
in layout(location=1) vec3 v_normal;

uniform vec3 ambientLight;
uniform mat4 fullTransformMatrix;

out vec3 frag_color;

void main()
{
    gl_Position = fullTransformMatrix * vec4(v_position, 1.0f);
    frag_color = v_normal;
}