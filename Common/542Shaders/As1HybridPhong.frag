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
uniform sampler2D shadowBufferSAT;
uniform sampler2D shadowBufferMap;
uniform sampler2D irradianceMap;

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
	
uniform float nearDepth;
uniform float farDepth;

uniform float bias;

uniform int blurStrength;
uniform int shadowMapSize;


const vec2 invAtan = vec2(0.1591, 0.3183);
const float PI = 3.1415926538;

bool CalculateG(vec2 shadowIndex, float pixelDepth, int strength, float adjustedBias, inout float G)
{

	vec4 lightDepth = 
	texture(shadowBufferSAT, shadowIndex + (vec2(strength, strength) / shadowMapSize))
	 - texture(shadowBufferSAT, shadowIndex + (vec2(strength, -strength - 1) / shadowMapSize))
	 + texture(shadowBufferSAT, shadowIndex + (vec2(-strength - 1, -strength - 1) / shadowMapSize))
	 - texture(shadowBufferSAT, shadowIndex + (vec2(-strength - 1, strength) / shadowMapSize));
	int numOfPixels = (2 * strength + 1) * (2 * strength + 1);
	if(strength == 0)
	{
		lightDepth = texture(shadowBufferMap, shadowIndex);
	}
	else
	{
		lightDepth = lightDepth / numOfPixels;
	}

	vec4 halfVector = vec4(0.5, 0.5, 0.5, 0.5);
	vec4 sampledDepth = (1 - adjustedBias) * lightDepth + adjustedBias * halfVector;

	// Use a Cholesky decomposition or Cramer's rule to solve for c


	// Cholesky's rule for right now
	float a = 1;
	float b = sampledDepth.x;
	float c = sampledDepth.y;
	float d = sampledDepth.y - (b*b);
	if(d <= 0)
	{
		return false;
	}
	else
	{
		d = sqrt(d);
	}
	float e = (sampledDepth.z - (b*c)) / d;
	float f = sampledDepth.w - (c*c) - (e*e);
	if(f <= 0)
	{
		return false;
	}
	else
	{
		f = sqrt(f);
	}
	
	float c1Hat = 1/a;
	float c2Hat = (pixelDepth - (b * c1Hat)) / d;
	float c3Hat = ((pixelDepth * pixelDepth) - (c*c1Hat)-(e*c2Hat)) / f;

	float c3 = c3Hat / f;
	float c2 = (c2Hat - (e*c3)) / d;
	float c1 = (c1Hat - (b*c2) - (c*c3))/a;
	
	float z2 = (-c2 - sqrt(c2 * c2 - (4 * c3 * c1)) ) / (2 * c3);
	float z3 = (-c2 + sqrt(c2 * c2 - (4 * c3 * c1)) ) / (2 * c3);
	if(z2 > z3)
	{
		float tmp = z2;
		z2 = z3;
		z3 = tmp;
	}

	if(pixelDepth <= z2)
	{
		G = 0;
	}
	else if(pixelDepth <= z3)
	{
		G = (pixelDepth * z3 - sampledDepth.x * (pixelDepth + z3) + sampledDepth.y) / ((z3 - z2) * (pixelDepth - z2));
	}
	else
	{
		G = 1 - ((z2 * z3 - sampledDepth.x * (z2 + z3) + sampledDepth.y) / ((pixelDepth - z2) * (pixelDepth - z3)));
	}

	return true;
}

vec3 GetIrradianceColor(vec3 n)
{
	
	vec2 equirectangularUV = vec2(atan(n.z, n.x), asin(n.y));
	equirectangularUV *= invAtan;
	equirectangularUV += 0.5f;

	return texture(irradianceMap, equirectangularUV).xyz;
}

vec3 CalculateDirectionalLight()
{
	vec4 Kd = texture(diffuseBuffer, uv);
	if(Kd.a < -400)
	{
		return Kd.xyz;
	}

	vec3 vertexNormal = normalize(texture(normalBuffer, uv).rgb);
	vec3 view = normalize(cPos - texture(positionBuffer, uv).rgb);

	vec3 lightVector = -lightDirection;
	lightVector = normalize(lightVector);
	vec3 reflection = 2*dot(vertexNormal, lightVector)*vertexNormal - lightVector;

	vec3 lightDiffuse = Kd.xyz / PI * GetIrradianceColor(vertexNormal);
	vec3 lightSpecular = vec3(0);//texture(specularBuffer, uv).rgb* pow(max(dot(view, reflection), 0), ns) * lightIntensity;
	lightSpecular.r = max(lightSpecular.r, 0);
	lightSpecular.g = max(lightSpecular.g, 0);
	lightSpecular.b = max(lightSpecular.b, 0);

	vec3 intensityLocal = (lightDiffuse + lightSpecular);



	float G = 0.f;
	int strength = blurStrength;
	bool result = false;

	
	vec4 shadowCoord = objToShadow * texture(positionBuffer, uv);
	vec2 shadowIndex = shadowCoord.xy / shadowCoord.w;
	float pixelDepth = (shadowCoord.w - nearDepth) / (farDepth - nearDepth);
	float shadowMapDepth = texture(shadowBufferMap, shadowIndex).x;
	float biasSampled = bias;

	// Return 0 if position is out of shadow map or invalid.
	if(shadowCoord.w < 0 ||
	shadowIndex.x < 0 || shadowIndex.x >= 1.f ||
	shadowIndex.y < 0 || shadowIndex.y >= 1.f
	)
	{
		G = 0.f;
	}
	else
	{
		do
		{
			result = CalculateG(shadowIndex, pixelDepth, strength++, biasSampled, G);
		} while(result != true);
	}

	return (1.f - G) * intensityLocal;
}

void main()
{
	vec3 color = intensityEmissive + ambient;

	color += CalculateDirectionalLight();


	
	// float distanceView = texture(diffuseBuffer, uv).a;
	// float s = (zFar - distanceView) / (zFar - zNear);
	// color = s*color + (1-s)*intensityFog;
	
	outColor = vec4(color, texture(specularBuffer, uv).a);
}