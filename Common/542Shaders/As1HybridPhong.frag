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
uniform sampler2D shadowBuffer;

out vec4 outColor;

in vec4 vertexPosition;
in vec2 uv;

in vec3 cPos;

uniform int width;
uniform int height;

uniform mat4 objToShadow;

uniform float ambient;
uniform float ns;
uniform float zNear;
uniform float zFar;
uniform vec3 intensityEmissive;
uniform vec3 intensityFog;
uniform vec3 attenuationConstants;

	uniform vec3 lightIntensity;
	uniform vec3 lightDirection;


vec3 CalculateDirectionalLight()
{
	vec3 vertexNormal = normalize(texture(normalBuffer, uv).rgb);
	vec3 view = normalize(cPos - texture(positionBuffer, uv).rgb);

	vec3 lightVector = -lightDirection;
	lightVector = normalize(lightVector);
	vec3 reflection = 2*dot(vertexNormal, lightVector)*vertexNormal - lightVector;

	vec3 lightDiffuse = texture(diffuseBuffer, uv).rgb * max(dot(lightVector, vertexNormal), 0) * lightIntensity;
	vec3 lightSpecular = texture(specularBuffer, uv).rgb* pow(max(dot(view, reflection), 0), ns) * lightIntensity;
	lightSpecular.r = max(lightSpecular.r, 0);
	lightSpecular.g = max(lightSpecular.g, 0);
	lightSpecular.b = max(lightSpecular.b, 0);

	vec3 intensityLocal = (lightDiffuse + lightSpecular);

	// Begin Shadow algorithm
	vec4 shadowCoord = objToShadow * texture(positionBuffer, uv);
	vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;
	
	if(shadowCoord.w > 0 &&
	shadowIndex.x >= 0 && shadowIndex.x < 1.f &&
	shadowIndex.y >= 0 && shadowIndex.y < 1.f)
	{
		float lightDepth = texture(shadowBuffer, shadowIndex).w;
		float pixelDepth = shadowCoord.w;
		
		// If the pixel is in a shadow,
		if(pixelDepth > lightDepth + 0.04)
		{
			return vec3(0);
		}
	}
	// End of shadow algorithm

	return intensityLocal;
}

void main()
{
	vec3 color = intensityEmissive + ambient;

	color += CalculateDirectionalLight();


	
	float distanceView = texture(diffuseBuffer, uv).a;
	float s = (zFar - distanceView) / (zFar - zNear);
	color = s*color + (1-s)*intensityFog;
	
	outColor = vec4(color, texture(specularBuffer, uv).a);
}