/* Start Header -------------------------------------------------------
Copyright (C) 2023 Spring DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ComputeShaderDispatcher.h
Purpose: Class of dispatcher of a compute shader
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS562_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 1/27/2023
End Header --------------------------------------------------------*/
#pragma once
#include <gl/glew.h>

class ComputeShaderDispatcher
{
public:
	ComputeShaderDispatcher(const char* compute_file_path);
	virtual ~ComputeShaderDispatcher();

	void PrepareDrawing();
	// Not yet confirmed it works.
	bool SendUniformBlock(const GLchar* blockName, const GLsizei blockPropertyCount, const GLchar* const* blockPropertyNames, const float** blockPropertyData);
	void Dispatch(GLuint x, GLuint y, GLuint z);

	GLuint GetShader();

private:
	GLuint shader;

	GLuint  uniformBlockBuffer;
	GLuint uniformBlockIndex;
};