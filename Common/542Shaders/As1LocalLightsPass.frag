///* Start Header -------------------------------------------------------
//Copyright (C) FALL2021 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As2PhongShading.frag
//Purpose:	 This is the implementation of the Phong model 
//					 where the lighting computations are implemented in the fragment shader.
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 1/29/2022
//End Header --------------------------------------------------------*/

#version 410 core

uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D diffuseBuffer;
uniform sampler2D specularBuffer;

uniform int width;
uniform int height;

uniform float ns;
uniform vec3 attenuationConstants;


struct TheStruct
{
	vec3 lightPosition;
	vec3 intensityAmbient;
	vec3 intensityDiffuse;
	vec3 intensitySpecular;
	float lightType;
	vec3 lightDirection;
	vec2 anglesInnerOuter;
	float spotFalloff;
};

in TheStruct lightInfo;
in vec3 view;
out vec4 outColor;


vec3 CalculatePointLight(TheStruct item, vec2 uv)
{
	vec3 worldPos  = texture(positionBuffer, uv).rgb;
	vec3 vertexNormal = normalize(texture(normalBuffer, uv).rgb);
	
	vec3 lightVector = item.lightPosition - worldPos;
	float distanceLight = length(lightVector);
	lightVector = normalize(lightVector);
	vec3 reflection = 2*dot(vertexNormal, lightVector)*vertexNormal - lightVector;

	vec3 lightDiffuse = texture(diffuseBuffer, uv).rgb * max(dot(vertexNormal, lightVector), 0) * item.intensityDiffuse;
	vec3 lightSpecular = texture(specularBuffer, uv).rgb* pow(max(dot(reflection, view), 0), ns) * item.intensitySpecular;
	lightSpecular.r = max(lightSpecular.r, 0);
	lightSpecular.g = max(lightSpecular.g, 0);
	lightSpecular.b = max(lightSpecular.b, 0);

	
	int range = 50;
	float attenuation = min(1/(attenuationConstants.x + attenuationConstants.y*distanceLight + attenuationConstants.z * distanceLight*distanceLight), 1);
	attenuation = max(attenuation, 0);
	vec3 intensityLocal = attenuation * (lightDiffuse + lightSpecular);

	return intensityLocal;
}

vec3 CalculateSpotlightEffect(TheStruct item, float alpha, vec2 uv)
{
	vec3 worldPos  = texture(positionBuffer, uv).rgb;
	vec3 vertexNormal = normalize(texture(normalBuffer, uv).rgb);

	vec3 lightVector = item.lightPosition - worldPos;
	float distanceLight = length(lightVector);
	lightVector = normalize(lightVector);
	vec3 reflection = 2*dot(vertexNormal, lightVector)*vertexNormal - lightVector;

	vec3 lightDiffuse = texture(diffuseBuffer, uv).rgb * max(dot(lightVector, vertexNormal), 0) * item.intensityDiffuse;
	vec3 lightSpecular = texture(specularBuffer, uv).rgb* pow(max(dot(view, reflection), 0), ns) * item.intensitySpecular;
	lightSpecular.r = max(lightSpecular.r, 0);
	lightSpecular.g = max(lightSpecular.g, 0);
	lightSpecular.b = max(lightSpecular.b, 0);

	
	int range = 50;
	float attenuation = min(1/(attenuationConstants.x + attenuationConstants.y*distanceLight + attenuationConstants.z * distanceLight*distanceLight), 1);

	float spotlightEffect = pow((alpha - cos(item.anglesInnerOuter.y)) / (cos(item.anglesInnerOuter.x) - cos(item.anglesInnerOuter.y)), item.spotFalloff);
	vec3 intensityLocal = attenuation * ((spotlightEffect * (lightDiffuse + lightSpecular)));
	
	return intensityLocal;
}

vec3 CalculateSpotLight(TheStruct item, vec2 uv)
{
	vec3 worldPos  = texture(positionBuffer, uv).rgb;

	vec3 direction = normalize(item.lightDirection);
	vec3 toVertex = normalize(worldPos - item.lightPosition);
	float ld = dot(direction, toVertex);

	if(ld < cos(item.anglesInnerOuter.y))
	{
		return vec3(0, 0, 0);
	}
	else if(ld > cos(item.anglesInnerOuter.x))
	{
		return CalculatePointLight(item, uv);
	}
	else 
	{
		return CalculateSpotlightEffect(item, ld, uv);
	}

	return vec3(0, 0, 0);
}

vec3 CalculateIntensityFinal(TheStruct item, vec2 uv)
{
	if(item.lightType >= -5.f && item.lightType <= 5.f)
	{
		return CalculatePointLight(item, uv);
	}
	else 
	{
		return CalculateSpotLight(item, uv);
	}
}

void main()
{
	int range = 50;

	// Checks if the world-position is within the range of the light.
	vec2 uv = gl_FragCoord.xy / vec2(width, height);
	vec3 position = texture(positionBuffer, uv).xyz;
	if(length(position - lightInfo.lightPosition) > range)
	{
		// If not, output 0
		outColor = vec4(0, 0, 0, 1);
		return;
	}

	// If it is in range, uses lighting equation
	outColor = vec4(CalculateIntensityFinal(lightInfo, uv), 0);
	// An attenuation is 1/d^2 - 1/r^2
}