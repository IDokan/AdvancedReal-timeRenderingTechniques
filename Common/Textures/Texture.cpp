/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: Class which manage texture
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 11/5/2021
End Header --------------------------------------------------------*/

#include <../Common/Textures/Texture.h>
#include <../Common/ppms/ppmReader.h>
#include <stb_image.h>
#include <vector>

Texture::Texture()
	: width(0), height(0), pixels(nullptr), textureNum(-1), textureHandle(0)
{
}

Texture::~Texture()
{
}

void Texture::SetupTexture(const char* path, int _textureNum, enum class TextureType tType)
{
	textureNum = _textureNum;

	switch (tType)
	{
	case Texture::TextureType::BMP:
		pixels = readBMP(path, width, height);
		break;
	case Texture::TextureType::PPM:
		pixels = readImage(path, width, height);
		break;
	case Texture::TextureType::HDR:
		int nrComponents;
		stbi_set_flip_vertically_on_load(true);
		pixels = stbi_loadf(path, &width, &height, &nrComponents, 0);
		break;
	default:
		break;
	}

	glGenTextures(1, &textureHandle);
	
	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (tType == Texture::TextureType::HDR)
	{
		if (pixels)	// If pixel data has no problem,
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			stbi_image_free(pixels);
		}
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);

		delete[] pixels;
	}
}

void Texture::SetupTexture(int width, int height, int _textureNum)
{
	this->width = width;
	this->height = height;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	std::vector<float> emptyData(width * height * 4, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::SetupCubeTexture(int width, int height, int _textureNum)
{
	this->width = width;
	this->height = height;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::vector<float> emptyData(width * height * 4, 0);
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, emptyData.data());
	}
}

void Texture::UpdateTexture(GLuint programID, const GLchar* name)
{
	if (textureNum < 0 )
	{
		// ERROR!!
		return;
	}

	glUseProgram(programID);
	
	// Textures
	glActiveTexture(GL_TEXTURE0 + textureNum);
	//glBindTexture(GL_TEXTURE_2D, textureHandle);
	
	// Uniform sampler for texture unit 0
	GLint texSamplerLoc = glGetUniformLocation(programID, name);
	glUniform1i(texSamplerLoc, textureNum);
}

void Texture::UpdateImage(GLuint programID, const GLchar* name, GLenum access)
{
	if (textureNum < 0)
	{
		return;
	}

	glUseProgram(programID);

	glBindImageTexture(textureNum, textureHandle, 0, GL_FALSE, 0, access, GL_RGBA32F);

	GLint texSamplerLoc = glGetUniformLocation(programID, name);
	glUniform1i(texSamplerLoc, textureNum);
}

GLuint Texture::GetHandle() const
{
	return textureHandle;
}

void Texture::Clear()
{
	glDeleteTextures(1, &textureHandle);
}

glm::ivec2 Texture::GetTextureSize()
{
	return glm::ivec2(width, height);
}

void Texture::SetupHdrTexture(const char* path, int textureNum)
{
}
