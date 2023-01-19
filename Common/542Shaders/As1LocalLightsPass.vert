///* Start Header -------------------------------------------------------
//Copyright (C) FALL2021 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As4HybridPhong.vert
//Purpose: shader which draw rendered texture by FBO to confirm.
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 1/29/2022
//End Header --------------------------------------------------------*/

#version 410 core

uniform mat4 objToWorld;
uniform mat4 worldToNDC;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 objNormal;
layout (location = 2) in vec2 vUV;

uniform vec3 cameraPosition;

out vec4 vertexPosition;

out vec3 view;


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

layout(std140) uniform Block
{
	TheStruct item[99];
} data;

out TheStruct lightInfo;

void main()
{
	int range = 10;
	vertexPosition = objToWorld * vec4(position, 1.0f);
	vertexPosition = vec4(data.item[gl_InstanceID].lightPosition + (vertexPosition.xyz * range), vertexPosition.w);
	
	view = cameraPosition - vec3(vertexPosition.xyz);
	view = normalize(view);

	vertexPosition = worldToNDC * vertexPosition;

	lightInfo = data.item[gl_InstanceID];

	gl_Position = vertexPosition;
}