#version 400
layout(location = 0) in vec2 position;
smooth out vec4 theColor;

uniform mat4 ortho;
uniform mat4 view;

void main()
{
	gl_Position = ortho * view * vec4(position, -1.0f, 1.0f);
}
