#version 430

in layout(location=0) vec3 v_position;
in layout(location=1) vec3 v_color;

uniform mat4 fullTransformMatrix;
uniform vec3 uniformColor;

out vec3 frag_color;

void main()
{
    vec4 v = vec4(v_position, 1.0f);
    gl_Position = fullTransformMatrix * v;
    frag_color = uniformColor;
}