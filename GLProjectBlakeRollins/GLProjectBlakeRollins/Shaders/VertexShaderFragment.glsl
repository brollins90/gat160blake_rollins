#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec3 frag_normal;
out vec3 frag_position;

void main()
{
	vec4 v = vec4(vertexPositionModel, 1.0f);
    gl_Position = modelToProjectionMatrix * v;
	frag_normal = mat3(modelToWorldTransformMatrix) * vertexNormalModel;
	frag_position = vec3(modelToWorldTransformMatrix * v);
}