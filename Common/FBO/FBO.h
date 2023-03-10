/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FBO.h
Purpose: Class which manage Frame buffer object
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 11/28/2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>

class TextureManager;

class FBO
{
public:
	FBO();


	void Initialize(TextureManager* tManager, std::string firstTextureName, glm::vec3 color = glm::vec3(0.f));
	void Initialize(TextureManager* tManager, std::string firstTextureName, int width, int height);
	// textureNames sequence should be match with shader layout
	void InitializeCustomBuffer(TextureManager* tManager, std::vector<std::string> textureNames);
	void InitializeCustomBuffer(TextureManager* tManager, std::vector<std::string> textureNames, std::vector<std::pair<int, int>> textureResolution);
	void InitializeCubemap(TextureManager* tManager, std::string cubeTextureName, int width, int height);
	void AddTexture(std::string newTextureName);
	void ApplyFBO();
	void ApplyCubeFBO(std::string cubeMapName, int i);
	void BindTexture(std::string textureName);
	void RestoreDefaultFrameBuffer();
	void Clear();

	void UseTexture(GLuint programID, const GLchar* name);

	void SetClearColor(glm::vec3 color);

	void CopyDepthInfo();

private:
	int windowWidth;
	int windowHeight;
private:
	GLuint frameBufferHandle;
	GLuint depthRenderBuffer;

	glm::vec3 clearColor;

	TextureManager* textureManager;

	bool customBufferMode;
};