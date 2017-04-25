#version 400
smooth in vec4 theColor;
out vec4 outputColor;
in vec2 textureCoords;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform int numLights;
uniform vec4 lightPosition;
uniform vec3 lightPositions[8];
uniform float lightIntensities[8];
uniform vec3 lightColors[8];
uniform sampler2D theTexture;

vec3 lightDirections[8];

void main()
{
	vec3 lightDir = normalize(vec3(lightPosition) - fragmentPosition);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	float ambientIntensity = 0.3f;
	vec3 ambientColor =  lightColor * ambientIntensity;

	vec3 normal = normalize(fragmentNormal);

	vec3 diffuseColor;
	float diffuseIntensity;

	for (int i = 0; i < numLights; i++)
	{
		lightDirections[i] = normalize(vec3(lightPositions[i]) - fragmentPosition);

		//Diffuse Intensity = max(0.0f, dot(normal, lightDirections[lightIndex]));
		diffuseColor += lightColors[i] * max(0.0f, dot(normal, lightDirections[i])) * lightIntensities[i];
		diffuseIntensity += max(0.0f, dot(normal, lightDirections[i]));
	}

	if (diffuseIntensity > 0.95)
	{
		//diffuseColor = vec3(1.0f, 1.0f, 1.0f);
		if (diffuseColor.x > 0.8f)
		{
			diffuseColor.x = 1.0f;
		}
		if (diffuseColor.y > 0.8f)
		{
			diffuseColor.y = 1.0f;
		}
		if (diffuseColor.z > 0.8f)
		{
			diffuseColor.z = 1.0f;
		}
	}
	else if (diffuseIntensity > 0.5)
	{
		//diffuseColor = vec3(0.8, 0.8, 0.8);
		if (diffuseColor.x > 0.5f)
		{
			diffuseColor.x = 0.8f;
		}
		if (diffuseColor.y > 0.5f)
		{
			diffuseColor.y = 0.8f;
		}
		if (diffuseColor.z > 0.5f)
		{
			diffuseColor.z = 0.8f;
		}
	}
	else if (diffuseIntensity > 0.25)
	{
		//diffuseColor = vec3(0.6, 0.6, 0.6);
		if (diffuseColor.x > 0.3f)
		{
			diffuseColor.x = 0.6f;
		}
		if (diffuseColor.y > 0.3f)
		{
			diffuseColor.y = 0.6f;
		}
		if (diffuseColor.z > 0.3f)
		{
			diffuseColor.z = 0.6f;
		}
	}
	else
	{
		//diffuseColor = vec3(0.4, 0.4, 0.4);
		diffuseColor.x = 0.0f;
		diffuseColor.y = 0.0f;
		diffuseColor.z = 0.0f;
	}

	outputColor = vec4(ambientColor + diffuseColor, 1.0f) * texture(theTexture, textureCoords); 
}
