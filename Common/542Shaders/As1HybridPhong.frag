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
uniform sampler2D environmentMap;
uniform samplerCube SkyCubeMap;
uniform sampler2D equirectangularMap;

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

uniform float nearDepth;
uniform float farDepth;

uniform float bias;

uniform int blurStrength;
uniform int shadowMapSize;

uniform int skydomeImageWidth;
uniform int skydomeImageHeight;

const vec2 invAtan = vec2(0.1591, 0.3183);
const float PI = 3.1415926538;

uniform float exposure;
uniform float contrast;

uniform mat4 tmp1;
uniform mat4 tmp2;


// Uniformly distributed random numbers by Hammersley rule.
uniform HammersleyBlock
{
	float N;
	float tmp[2*100];
} Hammersley;

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
	
	return pow(texture(environmentMap, equirectangularUV).xyz, vec3(2.2));
}

int Characteristic(float d)
{
	if(d > 0)
	{
		return 1;
	}
	else return 0;
}

// roughness is alpha value
float Distribution(vec3 half, vec3 normal, float roughness)
{
	float dotResult = dot(half, normal);
	float tanThetaM = sqrt((1.0 - pow(dotResult, 2))) / dotResult;
	float roughnessSquared = pow(roughness, 2);
	return Characteristic(dotResult) * (roughnessSquared) / (PI * pow(dotResult, 4) * pow(roughnessSquared + pow(tanThetaM, 2), 2));
}

vec3 FresnelReflection(float d, vec3 Ks)
{
	return Ks + (1 - Ks) * pow(clamp(1 - abs(d), 0.0, 1.0), 5.0);
}

vec3 GetHDRColor(vec3 n, float distribution)
{
	vec2 equirectangularUV = vec2(atan(n.z, n.x), asin(n.y));
	equirectangularUV *= invAtan;
	equirectangularUV += 0.5f;

	float level = (0.5 * log2(512 * 512 / Hammersley.N) - (0.5*log2(distribution/4)));
	return pow(textureLod(equirectangularMap, equirectangularUV, level).xyz, vec3(2.2));
}

float G1(vec3 v1, vec3 v2, vec3 normal, float alpha)
{
	// Phong BRDF
	float vDotNormal = dot(v1, normal);
	float result = Characteristic(dot(v1, v2) / vDotNormal);
	// if(vDotNormal > 1.0)
	// {
	// 	return 1.0f;
	// }

	float tanThetaV = sqrt(1 - pow(vDotNormal, 2)) / vDotNormal;
	// if(tanThetaV <= 0.01f)
	// {
	// 	return 1.0f;
	// }

	return result * (2 / (1 + sqrt(1 + (pow(alpha, 2) * pow(tanThetaV, 2)))));
}

float GTermCalculationBRDF(vec3 view, vec3 reflection, vec3 normal, vec3 half, float roughness)
{
	return G1(view, half, normal, roughness) * G1(reflection, half, normal, roughness);
}

vec2 GetDistributionFloats(float e1, float e2, float a)
{
	// GGX BRDF
	e2 = 1 - sin(e2 * PI);
	float theta = atan(a * sqrt(e2) / sqrt(1 - e2));
	//if(theta <= 0.001)
	//{
	//	theta = 0.001;
	//}
	return vec2(e1, (theta) / PI);
}

vec3 InverseLogitudeLatitudeSphereCoorinatesToVector(float u, float v)
{
	vec3 result;
	result.x = cos(2 * PI * (0.5 - u)) * sin(PI * v);
	result.y = sin(2 * PI * (0.5 - u)) * sin(PI * v);
	result.z = cos(PI * v);

	return result;
}

vec3 CalculateDirectionalLight()
{
	vec4 Kd = texture(diffuseBuffer, uv);
	vec4 Ks = texture(specularBuffer, uv);

	vec3 vertexNormal = normalize(texture(normalBuffer, uv).rgb);
	vec3 view = normalize(cPos - texture(positionBuffer, uv).rgb);

	vec3 reflection = normalize(2*dot(vertexNormal, view)*vertexNormal - view);
	vec3 A = normalize(vec3(-reflection.y, reflection.x, 0));
	vec3 B = normalize(cross(reflection, A));

	vec3 lightSpecular = vec3(0);

	// Choose N random directions light in vector according to probability p(wk) = D(H)
	for(int i = 0; i < Hammersley.N; i++)
	{
		// I'm trying to debug it from the first Hammersley random number.
			// If the first random number is -1 of the hemisphere, result is not wrong.
		// distribution2 == (u, v) for which space????
		vec2 distribution2 = GetDistributionFloats(Hammersley.tmp[i * 2], Hammersley.tmp[i * 2 + 1], Ks.a);
		vec3 ZOrientedSampledLightInVectorFromSky = normalize(InverseLogitudeLatitudeSphereCoorinatesToVector(distribution2.x, distribution2.y));
		// return vec3(abs(ZOrientedSampledLightInVectorFromSky));
		vec3 lightIn = normalize(ZOrientedSampledLightInVectorFromSky.x * A + ZOrientedSampledLightInVectorFromSky.y * B + ZOrientedSampledLightInVectorFromSky.z * reflection);
		// return abs(lightIn);
		vec3 half = normalize(view + lightIn);
		float distribution = Distribution(half, vertexNormal, Ks.a);

		lightSpecular += GetHDRColor(lightIn, distribution) * FresnelReflection(dot(view, half), Ks.rgb)
		* GTermCalculationBRDF(view, reflection, vertexNormal, half, Ks.a) / (4.f * dot(view, vertexNormal));
	}


	vec3 lightDiffuse = Kd.xyz / PI * GetIrradianceColor(vertexNormal);

	lightSpecular.r = max(lightSpecular.r, 0);
	lightSpecular.g = max(lightSpecular.g, 0);
	lightSpecular.b = max(lightSpecular.b, 0);
	lightSpecular /= Hammersley.N;

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
	vec3 color = vec3(0);

	color += CalculateDirectionalLight();
	// outColor = vec4(color, 1);
	// return;
	// float distanceView = texture(diffuseBuffer, uv).a;
	// float s = (zFar - distanceView) / (zFar - zNear);
	// color = s*color + (1-s)*intensityFog;
	

	// linear to sRGB
	color = pow(color, vec3(1.0 / 2.2));
	// tone mapping
	color = pow((exposure * color) / ((exposure * color) + vec3(1, 1, 1)), vec3(contrast / 2.2));

	outColor = vec4(color, texture(specularBuffer, uv).a);
}