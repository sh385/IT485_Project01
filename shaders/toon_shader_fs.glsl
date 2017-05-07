#version 400
smooth in vec4 theColor;
out vec4 outputColor;
in vec2 textureCoords;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform int baseColor;
uniform int numLights;
uniform vec4 lightPosition;
uniform vec3 lightPositions[8];
uniform float lightIntensities[8];
uniform vec3 lightDirections[8];
uniform vec3 lightColors[8];
uniform float lightRanges[8];
uniform sampler2D theTexture;

vec3 directionToLights[8];

void main()
{
	if (baseColor == 0)
	{
		vec3 lightDir = normalize(vec3(lightPosition) - fragmentPosition);
		vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		float ambientIntensity = 0.3f;
		vec3 ambientColor =  lightColor * ambientIntensity;

		vec3 normal = normalize(fragmentNormal);

		vec3 diffuseColor = vec3(0.0f, 0.0f, 0.0f);
		float diffuseIntensity = 0.0f;
		float lightDot = 0.0f;

		for (int i = 0; i < numLights; i++)
		{
			directionToLights[i] = normalize(vec3(lightPositions[i]) - fragmentPosition);
			lightDot = dot(lightDirections[i], directionToLights[i]);

			if (lightDot <= -lightRanges[i])
			{
			//Diffuse Intensity = max(0.0f, dot(normal, directionToLights[lightIndex]));
				diffuseColor += lightColors[i] * max(0.0f, dot(normal, directionToLights[i])) * lightIntensities[i];
				diffuseIntensity += max(0.0f, dot(normal, directionToLights[i]));
			}
		}
		
		if (diffuseIntensity > 0.95)
		{
			diffuseColor.x = max(step(0.8f, diffuseColor.x), diffuseColor.x);
			diffuseColor.y = max(step(0.8f, diffuseColor.y), diffuseColor.y);
			diffuseColor.z = max(step(0.8f, diffuseColor.z), diffuseColor.z);
		}
		else if (diffuseIntensity > 0.5)
		{
			diffuseColor.x = max(step(0.5f, diffuseColor.x) * 0.8f, diffuseColor.x);
			diffuseColor.y = max(step(0.5f, diffuseColor.y) * 0.8f, diffuseColor.y);
			diffuseColor.z = max(step(0.5f, diffuseColor.z) * 0.8f, diffuseColor.z);
		}
		else if (diffuseIntensity > 0.25)
		{
			//diffuseColor = vec3(0.6, 0.6, 0.6);
			diffuseColor.x = max(step(0.3f, diffuseColor.x) * 0.6f, diffuseColor.x);
			diffuseColor.y = max(step(0.3f, diffuseColor.y) * 0.6f, diffuseColor.y);
			diffuseColor.z = max(step(0.3f, diffuseColor.z) * 0.6f, diffuseColor.z);
		}
		else
		{
			diffuseColor.x = 0.0f;
			diffuseColor.y = 0.0f;
			diffuseColor.z = 0.0f;
		}
		outputColor = vec4(ambientColor + diffuseColor, 1.0f) * texture(theTexture, textureCoords); 

	}
	else
	{
		outputColor= texture(theTexture, textureCoords);
	}

	
}
