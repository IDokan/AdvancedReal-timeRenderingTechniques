///* Start Header -------------------------------------------------------
//Copyright (C) FALL2021 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As4MainObjGbuffer.frag
//Purpose: hybrid rendering shader
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 1/25/2023
//End Header --------------------------------------------------------*/

#version 430 core

uniform mat4 objToWorld;
uniform mat4 worldToNDC;

layout(location = 0) in vec3 objPosition;

out vec4 fragPos;

void main()
{
	fragPos = (worldToNDC * objToWorld * vec4(objPosition, 1.0f));

	gl_Position = fragPos;
}