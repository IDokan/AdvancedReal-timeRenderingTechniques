///* Start Header -------------------------------------------------------
//Copyright (C) SPRING 2023 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As3Skydome.vert
//Purpose: hybrid rendering shader
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 2/16/2023
//End Header --------------------------------------------------------*/

#version 430 core

uniform mat4 mappingMatrix;
uniform mat4 objToWorld;
uniform mat4 worldToNDC;

layout(location = 0) in vec3 objPosition;

out vec3 localPos;

void main()
{
	localPos = (mappingMatrix * vec4(objPosition, 1.0f)).xyz;
	gl_Position = (worldToNDC * objToWorld * vec4(localPos, 1.0f));
}