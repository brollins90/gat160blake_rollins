#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 vertexUvModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 fragmentNormalWorld;
out vec3 fragmentPositionWorld;
out vec2 fragmentUV;

void main()
{
	vec4 v = vec4(vertexPositionModel, 1.0f);
    gl_Position = modelToProjectionMatrix * v;
	fragmentNormalWorld = vec3(modelToWorldMatrix * vec4(vertexNormalModel, 0));
	fragmentPositionWorld = vec3(modelToWorldMatrix * v);
	fragmentUV = vertexUvModel;
}