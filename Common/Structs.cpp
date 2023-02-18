
/* Start Header -------------------------------------------------------
Copyright (C) SPRING 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Structs.cpp
Purpose: Class which compiles shader
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS562
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 2/17/2023
End Header --------------------------------------------------------*/
#include "Structs.h"
Hammersley::Hammersley()
	:hammersley()
{
	int pos = 0;
	hammersley[pos++] = N;
	for (int k = 0; k < N; k++)
	{
		float p = 0.5f;
		float u = 0.f;
		for (int kk = k; kk; p *= 0.5f, kk >>= 1)
		{
			if (kk & 1)
			{
				u += p;
			}
			float v = (k + 0.5f) / N;
			hammersley[pos++] = u;
			hammersley[pos++] = v;
		}
	}
}

const GLchar* Hammersley::GetBlockName()
{
	return "HammersleyBlock";
}

const GLsizei Hammersley::GetBlockDataSize()
{
	return static_cast<GLsizei>((2 * N + 1) * sizeof(float));
}

const float* Hammersley::GetData()
{
	return hammersley;
}
