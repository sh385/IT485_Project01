#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texels;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 modelMatrix;
out vec3 fragNormal;

void main()
{
	gl_Position = proj * view * modelMatrix * vec4(position, 1.0f);
	fragNormal = normal;

}
