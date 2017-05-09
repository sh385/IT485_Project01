#version 400
smooth in vec4 theColor;
out vec4 outputColor;
in vec2 textureCoords;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform int numLights;
uniform vec3 lightPositions[8];
uniform float lightIntensities[8];
uniform vec3 lightColors[8];

uniform sampler2D theTexture;
uniform vec3 camPos;

vec3 lightDirections[8];


void main()
{
	
	vec3 towardsCam = normalize(camPos - fragmentPosition);

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	float lightIntensity = 0.5f;

	float ambientIntensity = 0.3f;
	vec3 ambientColor =  lightColor * ambientIntensity;

	vec3 normal = normalize(fragmentNormal);
	vec3 diffuseColor;

	for (int i = 0; i < numLights; i++)
	{
		lightDirections[i] = normalize(vec3(lightPositions[i]) - fragmentPosition);

		//Diffuse Intensity = max(0.0f, dot(normal, lightDirections[lightIndex]));
		diffuseColor += lightColors[i] * max(0.0f, dot(normal, lightDirections[i])) * lightIntensities[i];
	}
	

	//float specIntensity = 0.7f;
	//vec3 specColor = specIntensity * lightColor  * pow(dot(reflect(normal, lightDir), towardsCam), 32);

	outputColor = vec4(ambientColor + diffuseColor, 1.0f) * texture(theTexture, textureCoords);
	//outputColor = texture(theTexture, textureCoords);
}

