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
#include "../Common/Noise/PerlinNoise.hpp"

static siv::PerlinNoise::seed_type seed = 1;

Texture::Texture()
	: width(0), height(0), depth(0), pixels(nullptr), textureNum(-1), textureHandle(0)
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

void Texture::CopyTexture(int width, int height, int _textureNum, GLuint copiedTextureHandle)
{
	this->width = width;
	this->height = height;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glCopyImageSubData(copiedTextureHandle, GL_TEXTURE_2D, 0, 0, 0, 0, textureHandle, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);

	glBindTexture(GL_TEXTURE_2D, 0);
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

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindImageTexture(textureNum, textureHandle, 0, GL_FALSE, 0, access, GL_RGBA32F);

	GLint texSamplerLoc = glGetUniformLocation(programID, name);
	glUniform1i(texSamplerLoc, textureNum);
}

void Texture::SetupPerlineTexture1D(int width, int _textureNum, double frequency, int octaves, const std::function<float(float)>& f)
{
	this->width = width;
	this->height = 1;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_1D, textureHandle);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	std::vector<float> emptyData(width * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const double fx = frequency / width;
	for (int i = 0; i < width; i++)
	{
		float noise = static_cast<float>(perlin.octave1D_01(i * fx, octaves));

		noise = f(noise);

		emptyData[(i * 4)] = noise;
		emptyData[(i * 4) + 1] = noise;
		emptyData[(i * 4) + 2] = noise;
		emptyData[(i * 4) + 3] = noise;
	}

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, width, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::UpdatePerlineTexture1D(double frequency, int octaves, const std::function<float(float)>& f)
{
	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_1D, textureHandle);

	std::vector<float> emptyData(width * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const double fx = frequency / width;
	for (int i = 0; i < width; i++)
	{
		float noise = static_cast<float>(perlin.octave1D_01(i * fx, octaves));
		noise = f(noise);
		emptyData[(i * 4)] = noise;
		emptyData[(i * 4) + 1] = noise;
		emptyData[(i * 4) + 2] = noise;
		emptyData[(i * 4) + 3] = noise;
	}

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, width, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::SetupPerlineTexture2D(int width, int height, int _textureNum, double frequency, int octaves, const std::function<float(float)>& f)
{
	this->width = width;
	this->height = height;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	std::vector<float> emptyData(width * height * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const int channel = 4;
	const double fx = frequency / width;
	const double fy = frequency / height;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float noise = static_cast<float>(perlin.octave2D_01(x * fx, y * fy, octaves));
			noise = f(noise);
			emptyData[(x * channel) + (y*width* channel)] = noise;
			emptyData[(x * channel) + (y*width* channel) + 1] = noise;
			emptyData[(x * channel) + (y*width* channel) + 2] = noise;
			emptyData[(x * channel) + (y*width* channel) + 3] = noise;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::UpdatePerlineTexture2D(double frequency, int octaves, const std::function<float(float)>& f)
{
	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	std::vector<float> emptyData(width * height * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const int channel = 4;
	const double fx = frequency / width;
	const double fy = frequency / height;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float noise = static_cast<float>(perlin.octave2D_01(x * fx, y * fy, octaves));
			noise = f(noise);
			emptyData[(x * channel) + (y * width * channel)] = noise;
			emptyData[(x * channel) + (y * width * channel) + 1] = noise;
			emptyData[(x * channel) + (y * width * channel) + 2] = noise;
			emptyData[(x * channel) + (y * width * channel) + 3] = noise;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::SetupPerlineTexture3D(int width, int height, int depth, int _textureNum, double frequency, int octaves, const std::function<float(float)>& f)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	textureNum = _textureNum;

	glGenTextures(1, &textureHandle);

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_3D, textureHandle);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

	std::vector<float> emptyData(width * height * depth * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const int channel = 4;
	const double fx = frequency / width;
	const double fy = frequency / height;
	const double fz = frequency / depth;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				float noise = static_cast<float>(perlin.octave3D_01(x * fx, y * fy, z * fz, octaves));
				noise = f(noise);
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel)] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 1] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 2] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 3] = noise;
			}
		}
	}

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

void Texture::UpdatePerlineTexture3D(double frequency, int octaves, const std::function<float(float)>& f)
{

	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_3D, textureHandle);

	std::vector<float> emptyData(width * height * depth * 4, 0);

	// May seed value be changed.
	seed += textureNum;
	const siv::PerlinNoise perlin{ seed };
	const int channel = 4;
	const double fx = frequency / width;
	const double fy = frequency / height;
	const double fz = frequency / depth;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				float noise = static_cast<float>(perlin.octave3D_01(x * fx, y * fy, z * fz, octaves));
				noise = f(noise);
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel)] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 1] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 2] = noise;
				emptyData[(x * channel) + (y * width * channel) + (z * width * height * channel) + 3] = noise;
			}
		}
	}

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, emptyData.data());
}

GLuint Texture::GetHandle() const
{
	return textureHandle;
}

void Texture::Clear()
{
	glDeleteTextures(1, &textureHandle);
}

glm::ivec2 Texture::GetTextureSize() const
{
	return glm::ivec2(width, height);
}

glm::vec4 Texture::ReadPixelData(int x, int y)
{
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	std::vector<float> pixelData(width * height * 4);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixelData.data());
	
	glm::vec4 result;
	result.x = pixelData[4 * (y * width + x) + 0];
	result.y = pixelData[4 * (y * width + x) + 1];
	result.z = pixelData[4 * (y * width + x) + 2];
	result.w = pixelData[4 * (y * width + x) + 3];

	return result;
}

void Texture::SetupHdrTexture(const char* path, int textureNum)
{
}
