#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;

uniform mat4 modelToProjectionMatrix;

out vec4 frag_color;

void main()
{
	vec4 v = vec4(vertexPositionModel, 1.0f);
    gl_Position = modelToProjectionMatrix * v;
    frag_color = vertexColor;
}