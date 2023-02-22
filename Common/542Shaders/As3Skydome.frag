///* Start Header -------------------------------------------------------
//Copyright (C) SPRING 2023 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As3Skydome.frag
//Purpose: hybrid rendering shader
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 2/16/2023
//End Header --------------------------------------------------------*/

#version 430 core

out vec4 outColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

uniform float exposure;
uniform float contrast;

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
	vec3 mapColor = pow(texture(equirectangularMap, uv, 0).rgb, vec3(2.2));

	vec3 color = pow(mapColor, vec3(1.0 / 2.2));
	color = pow((exposure * color) / ((exposure * color) + vec3(1, 1, 1)), vec3(contrast / 2.2));
	outColor = vec4(color, 1);
}