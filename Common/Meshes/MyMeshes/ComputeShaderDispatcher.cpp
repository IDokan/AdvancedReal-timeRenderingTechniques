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
	:uniformBlockBuffer(0), uniformBlockIndex(0)
{
	shader = LoadShaders(compute_file_path);
}

ComputeShaderDispatcher::~ComputeShaderDispatcher()
{
	glDeleteProgram(shader);
}

void ComputeShaderDispatcher::PrepareDrawing()
{
	glUseProgram(shader);
}

void ComputeShaderDispatcher::SendUniformInt(const char* uniformName, const int uniformData)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	glUniform1i(loc, uniformData);
}

void ComputeShaderDispatcher::SendUniformFloat(const char* uniformName, const float uniformData)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	glUniform1f(loc, uniformData);
}

void ComputeShaderDispatcher::SendUniformFloat3(const char* uniformName, const float* uniformData)
{
	GLint loc = glGetUniformLocation(shader, uniformName);
	glUniform3f(loc, uniformData[0], uniformData[1], uniformData[2]);
}

bool ComputeShaderDispatcher::SendUniformBlock(const GLchar* blockName, const GLsizei blockSize, const float* blockData)
{
	glUseProgram(shader);
	// Step 2.	First get the block index
	uniformBlockIndex = glGetUniformBlockIndex(shader, blockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, uniformBlockIndex == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	// Step 5.	Create OpenGL buffer to manage this uniform block
	if (uniformBlockBuffer == 0)
	{
		glGenBuffers(1, &uniformBlockBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBuffer);
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBlockBuffer);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockData, GL_STATIC_DRAW);

	glUniformBlockBinding(shader, uniformBlockIndex, uniformBlockIndex);

	return true;
}

void ComputeShaderDispatcher::Dispatch(GLuint x, GLuint y, GLuint z)
{
	glDispatchCompute(x, y, z);
}

GLuint ComputeShaderDispatcher::GetShader()
{
	return shader;
}
