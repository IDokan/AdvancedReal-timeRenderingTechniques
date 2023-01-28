/* Start Header -------------------------------------------------------
Copyright (C) 2023 Spring DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ComputeShaderDispatcher.cpp
Purpose: Class of dispatcher of a compute shader
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS562_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 1/27/2023
End Header --------------------------------------------------------*/
#include <iostream>
#include "ComputeShaderDispatcher.h"
#include "shader.hpp"

ComputeShaderDispatcher::ComputeShaderDispatcher(const char* compute_file_path)
{
	shader = LoadShaders(compute_file_path);
}

ComputeShaderDispatcher::~ComputeShaderDispatcher()
{
}

void ComputeShaderDispatcher::PrepareDrawing()
{
	glUseProgram(shader);
}

bool ComputeShaderDispatcher::SendUniformBlock(const GLchar* blockName, const GLsizei blockPropertyCount, const GLchar* const* blockPropertyNames, const float** blockPropertyData)
{	// Step 1.	Design layout of the uniform Blocks

	glUseProgram(shader);
	// Step 2.	First get the block index
	uniformBlockIndex = glGetUniformBlockIndex(shader, blockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, uniformBlockIndex == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	// Step 3.	Allocate the block, get block indices and offsets
	GLint uniformBlockSize = 0;
	glGetActiveUniformBlockiv(shader, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

	GLuint* blockPropertyIndices = new GLuint[blockPropertyCount];
	glGetUniformIndices(shader, blockPropertyCount, blockPropertyNames, blockPropertyIndices);

	if (blockPropertyIndices[0] == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, blockPropertyIndices[0] == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	GLint* offsets = new GLint[blockPropertyCount];
	glGetActiveUniformsiv(shader, blockPropertyCount, blockPropertyIndices, GL_UNIFORM_OFFSET, offsets);

	// Step 4.	Copy data into the buffer from CPU memory
	char* buffer = new char[uniformBlockSize];
	for (int i = 0; i < blockPropertyCount; ++i)
	{
		if (buffer + offsets[i] < &buffer[uniformBlockSize])
		{
			memcpy_s(buffer + offsets[i], sizeof(float) * 3, &blockPropertyData[i], sizeof(float) * 3);
		}
	}

	// Step 5.	Create OpenGL buffer to manage this uniform block
	if (uniformBlockBuffer == 0)
	{
		glGenBuffers(1, &uniformBlockBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBuffer);
	}
	glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBlockBuffer);

	delete[] buffer;

	return true;
}

void ComputeShaderDispatcher::Dispatch(GLuint x, GLuint y, GLuint z)
{
	glDispatchCompute(x, y, z);
	glUseProgram(0);
}

GLuint ComputeShaderDispatcher::GetShader()
{
	return shader;
}
