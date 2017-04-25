#version 400
layout(location = 0) in vec3 position;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 modelMatrix;

uniform vec3 camUp;
uniform vec3 camPos;
void main()
{
	vec3 look = normalize(camPos - vec3(modelMatrix * vec4(position, 1.0f)));
	vec3 right = cross(camUp, look);
	vec3 up = cross(look, right);

	mat4 billboard;
	billboard[0][0] = right.x;
	billboard[0][1] = right.y;
	billboard[0][2] = right.z;

	billboard[1][0] = up.x;
	billboard[1][1] = up.y;
	billboard[1][2] = up.z;

	billboard[2][0] = look.x;
	billboard[2][1] = look.y;
	billboard[2][2] = look.z;

	billboard[3][3] = 1.0f;
	gl_Position = proj * view * modelMatrix * billboard * vec4(position, 1.0f);
}
