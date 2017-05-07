#version 400
in vec2 texCoords;
out vec4 outputColor;
uniform sampler2D theTexture;
float offset = 2.0f / 600.0f;

void main()
{
	vec2 offsets[9] = {
		vec2(-offset, offset), 
		vec2(0.0f, offset), 
		vec2(offset, offset), 
		vec2(-offset, 0.0f), 
		vec2(0.0f, 0.0f), 
		vec2(offset, 0.0f), 
		vec2(-offset, -offset),
		vec2(0.0f, -offset), 
		vec2(offset, -offset) 
	};
	int edgeDetectKernel[9] = {
		-1, -1, -1,
		- 1, 8, -1,
		- 1, -1, -1
	};
	vec3 tempTex[9];
	vec3 color;
	for (int i = 0; i < 9; i++)
	{
		tempTex[i] = vec3(texture(theTexture, texCoords + offsets[i]));
		color += tempTex[i] * edgeDetectKernel[i];
	}
	float avg = (color.x + color.y + color.z) / 3.0f;
	color = vec3(avg, avg, avg);
	outputColor = vec4(0.0f, 0.0f, 0.0f, step(0.2f, avg));
}

