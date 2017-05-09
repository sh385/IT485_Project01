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
uniform vec4 objectColor;
uniform sampler2D theTexture;

vec3 directionToLights[8];

void main()
{
	if (baseColor == 0)
	{
		vec3 lightDir = normalize(vec3(lightPosition) - fragmentPosition);
		vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);
		float ambientIntensity = 0.3f;
		vec3 ambientColor =  lightColor * ambientIntensity;

		vec3 normal = normalize(fragmentNormal);

		vec3 diffuseColor = vec3(0.0f, 0.0f, 0.0f);
		float diffuseIntensity = 0.0f;
		float lightDot = 0.0f;

		for (int i = 0; i < numLights; i++)
		{
			directionToLights[i] = normalize(vec3(lightPositions[i]) - fragmentPosition);
			lightDot = dot(normalize(lightDirections[i]), directionToLights[i]);

			if (lightDot <= -lightRanges[i])
			{
			//vec4 diffuseColor = lightColor * diffuseIntensity * lightStrength;
			//Diffuse Intensity = max(0.0f, dot(normal, directionToLights[lightIndex]));

				diffuseColor += lightColors[i] * max(0.0f, dot(normal, directionToLights[i])) * lightIntensities[i];
				diffuseIntensity += max(0.0f, dot(normal, directionToLights[i]));
			}
		}
		 
		if (diffuseIntensity >= 0.95f)
		{
			diffuseColor = vec3(0.85f, 0.85f, 0.85f);
		}
		else if (diffuseIntensity >= 0.65f)
		{
			diffuseColor = vec3(0.55f, 0.55f, 0.55f);
		}
		else if (diffuseIntensity >= 0.35f)
		{
			diffuseColor = vec3(0.25f, 0.25f, 0.25f);
		}
		else 
		{
			diffuseColor = vec3(0.0f, 0.0f, 0.0f);
		}
		float step1 = step(0.95f, diffuseIntensity);
		float diffValue = (step1 * 0.85) + (step(0.45f, diffuseIntensity) - step1) * 0.45f;
		diffuseColor = vec3(diffValue, diffValue, diffValue);
		outputColor = vec4(ambientColor + diffuseColor, 1.0f) * texture(theTexture, textureCoords);
	}
	else
	{
		outputColor = objectColor;
	}

	
}
