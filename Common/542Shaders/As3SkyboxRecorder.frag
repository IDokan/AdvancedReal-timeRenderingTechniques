///* Start Header -------------------------------------------------------
//Copyright (C) SPRING 2023 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As3SkyboxRecorder.frag
//Purpose: hybrid rendering shader
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 3/2/2023
//End Header --------------------------------------------------------*/

#version 430 core

out vec4 outColor;
in vec3 localPos;

uniform sampler2D skydomeImage;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5f;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(localPos));
	vec3 mapColor = pow(texture(skydomeImage, uv, 0).rgb, vec3(2.2));

	outColor = vec4(mapColor, 1);
}