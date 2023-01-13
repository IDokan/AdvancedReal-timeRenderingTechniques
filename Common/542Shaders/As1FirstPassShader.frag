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
//Creation date: 1/29/2022
//End Header --------------------------------------------------------*/

#version 430 core

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 diffuse;
uniform vec3 specular;

// Writing the output data into our GBuffer
layout (location = 0) out vec4 positionBuffer;
layout (location = 1) out vec4 normalBuffer;
layout (location = 2) out vec4 diffuseBuffer;
layout (location = 3) out vec4 specularBuffer;		// a value is alpha

void main()
{
	positionBuffer.xyz = fragPos;
	normalBuffer.xyz = normalize(fragNormal);
	diffuseBuffer = vec4(diffuse, 1);
	
	specularBuffer = vec4(specular, 1);
}