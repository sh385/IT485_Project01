#version 400
out vec4 outputColor;
in vec3 fragNormal;
uniform vec3 camDir;

void main()
{
	float val =  abs(dot(-camDir, fragNormal));
	outputColor = vec4(0.7f, 0.0f, 0.0f, 1.0f - val*step(0.2f,val));
}

