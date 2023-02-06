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

uniform float lightDepthOffset;

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
	
uniform float nearDepth;
uniform float farDepth;

float det3(vec3 a, vec3 b, vec3 c)
{
	return a.x * (b.y * c.z - b.z * c.y) + a.y * (b.z * c.x - b.x * c.z) + a.z * (b.x * c.y - b.y * c.x);
}

float CalculateG()
{
	vec4 shadowCoord = objToShadow * texture(positionBuffer, uv);
	vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;

	// Return 0 if position is out of shadow map or invalid.
	if(shadowCoord.w < 0 ||
	shadowIndex.x < 0 || shadowIndex.x >= 1.f ||
	shadowIndex.y < 0 || shadowIndex.y >= 1.f
	)
	{
		return 0.f;
	}

	float bias = 0.01;
	vec4 lightDepth = texture(shadowBuffer, shadowIndex);
	float pixelDepth = (shadowCoord.w - nearDepth) / (farDepth - nearDepth);

	vec4 halfVector = vec4(0.5, 0.5, 0.5, 0.5);
	vec4 b = (1 - bias) * lightDepth + bias * halfVector;

	// Use a Cholesky decomposition or Cramer's rule to solve for c

	// Cramer's rule for right now
	vec3 A = vec3(1, b.x, b.y);
	vec3 B = vec3(b.x, b.y, b.z);
	vec3 C = vec3(b.y, b.z, b.w);
	vec3 Z = vec3(1, pixelDepth, pixelDepth * pixelDepth);

	float d = det3(A, B, C);
	float c1 = det3(Z,B, C) / d;
	float c2 = det3(A, Z, C) / d;
	float c3 = det3(A, B, Z) / d;
	
	float z2 = (-c2 + sqrt(c2 * c2 - (4 * c3 * c1)) ) / (2 * c3);
	float z3 = (-c2 - sqrt(c2 * c2 - (4 * c3 * c1)) ) / (2 * c3);
	
	if(pixelDepth <= z2)
	{
		return 0;
	}
	else if(pixelDepth <= z3)
	{
		return (pixelDepth * z3 - b.x * (pixelDepth + z3) + b.y) / ((z3 - z2) * (pixelDepth - z2));
	}
	else
	{
		return 1 - ((z2 * z3 - b.x * (z2 + z3) + b.y) / ((pixelDepth - z2) * (pixelDepth - z3)));
	}
}

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
	
		
	// End of shadow algorithm

	return (1 - CalculateG()) * intensityLocal;
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