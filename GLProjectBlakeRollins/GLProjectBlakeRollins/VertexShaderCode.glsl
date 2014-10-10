#version 430

in layout(location=0) vec3 v_position;

uniform mat4 fullTransformMatrix;
uniform vec3 uniformColor;
uniform float vertexDepth;

out vec3 frag_color;

void main()
{
    vec4 v = fullTransformMatrix * vec4(v_position, 1.0f);
	v.z = vertexDepth;
    gl_Position = v;
    frag_color = uniformColor;
}