/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: Class which manage texture
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 11/5/2021
End Header --------------------------------------------------------*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Texture
{
public:
	enum class TextureType {
		BMP,
		PPM,
		HDR,
	} TextureType;
public:
	Texture();
	~Texture();
	void SetupTexture(const char* path, int textureNum, enum class TextureType tType);
	void SetupTexture(int width, int height, int textureNum);
	void SetupCubeTexture(int width, int height, int textureNum);
	void CopyTexture(int width, int height, int textureNum, GLuint copiedTextureHandle);

	void UpdateTexture(GLuint programID, const GLchar* name);
	void UpdateImage(GLuint programID, const GLchar* name, GLenum access);

	GLuint GetHandle() const;

	void Clear();

	// (width, height)
	glm::ivec2 GetTextureSize() const;

	glm::vec4 ReadPixelData(int x, int y);
private:
	void SetupHdrTexture(const char* path, int textureNum);
private:
	int width;
	int height;
	float* pixels;
	int textureNum;

	GLuint textureHandle;
};