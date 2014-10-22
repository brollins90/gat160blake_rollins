#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 vertexNormalModel;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec3 frag_color;

void main()
{
	vec4 v = vec4(vertexPositionModel, 1.0f);
    gl_Position = modelToProjectionMatrix * v;

	vec3 trans_normal = mat3(modelToWorldTransformMatrix) * vertexNormalModel;
	vec3 trans_position = vec3(modelToWorldTransformMatrix * v);

	vec3 lightVector = normalize(lightPosition - trans_position);
	float brightness = dot(lightVector, trans_normal);

	vec3 light = vec3(
	    ambientLight.x + brightness,
		ambientLight.y + brightness,
		ambientLight.z + brightness);

	frag_color = light;
}
