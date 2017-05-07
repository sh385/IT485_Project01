#version 400
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texels;

smooth out vec4 outColor;
out vec2 texCoords;
uniform sampler2D theTexture;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	texCoords = texels;
}
