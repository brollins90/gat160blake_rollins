#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 vertexUvModel;

uniform mat4 modelToProjectionMatrix;

out vec4 fragmentColor;

void main()
{
    gl_Position = modelToProjectionMatrix * vertexPositionModel;
    fragmentColor = vertexColor;
}