///* Start Header -------------------------------------------------------
//Copyright (C) SPRING 2023 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior written
//consent of DigiPen Institute of Technology is prohibited.
//File Name: As3SkyboxRecorder.vert
//Purpose: hybrid rendering shader
//Language: GLSL
//Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
//Project: sinil.kang_CS350_1
//Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
//Creation date: 3/2/2023
//End Header --------------------------------------------------------*/

#version 430 core

uniform mat4 mapping;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 objPosition;
layout(location = 1) in vec3 objNormal;
layout(location = 2) in vec2 objUV;

out vec3 localPos;
out vec2 localUV;

void main()
{
	localPos = (mapping * vec4(objPosition, 1.0f)).xyz;
	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(localPos, 1.0);
	gl_Position = (projection * view * vec4(localPos, 1.0f));

	localUV = objUV;
}