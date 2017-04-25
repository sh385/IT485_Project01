#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texels;
smooth out vec4 theColor;

out vec3 fragmentNormal;
out vec2 textureCoords;
out vec3 fragmentPosition;

uniform mat4 view;
uniform mat4 proj;
uniform vec4 lightPosition;
uniform mat4 modelMatrix;

void main()
{
	theColor = vec4(0.5, 0.5, 1.0, 1.0f);
	gl_Position = proj * view * modelMatrix * vec4(position, 1.0f);
	fragmentPosition = (modelMatrix*vec4(position, 1.0f)).xyz;
	textureCoords = texels;
	fragmentNormal = normal;
}
