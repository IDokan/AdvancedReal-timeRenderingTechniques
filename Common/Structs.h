/* Start Header -------------------------------------------------------
Copyright (C) SPRING 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Structs.h
Purpose: Class which compiles shader
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS562
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 2/17/2023
End Header --------------------------------------------------------*/
#pragma once
#include <GL/glew.h>

struct Hammersley{
	Hammersley();

	const GLchar* GetBlockName();
	const GLsizei GetBlockDataSize();
	const float* GetData();

	constexpr static float N = 40;
	float hammersley[static_cast<int>(2 * N)+1];
};