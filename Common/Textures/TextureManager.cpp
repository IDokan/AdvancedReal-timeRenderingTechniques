/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Purpose: Class which manage textures
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 11/5/2021
End Header --------------------------------------------------------*/
#include <../Common/Textures/TextureManager.h>

TextureManager::TextureManager()
	: textureSize(0)
{
	textures.clear();
}

void TextureManager::AddTexture(const char* path, std::string textureName, enum class Texture::TextureType t)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupTexture(path, textureSize++, t);
	textures.insert(newTex);
}

void TextureManager::AddTexture(int width, int height, std::string textureName)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupTexture(width, height, textureSize++);
	textures.insert(newTex);
}

void TextureManager::AddCubeTexture(int width, int height, std::string textureName)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupCubeTexture(width, height, textureSize++);
	textures.insert(newTex);
}

void TextureManager::ActivateTexture(GLuint programID, std::string activateName)
{
	ActivateTexture(programID, activateName, activateName);
}

void TextureManager::ActivateTexture(GLuint programID, std::string textureName, std::string activateName)
{
	textures.find(textureName)->second.UpdateTexture(programID, activateName.c_str());
}

void TextureManager::ActivateImage(GLuint programID, std::string activateName, GLenum access)
{
	ActivateImage(programID, activateName, activateName, access);
}

void TextureManager::ActivateImage(GLuint programID, std::string textureName, std::string activateName, GLenum access)
{
	textures.find(textureName)->second.UpdateImage(programID, activateName.c_str(), access);
}

void TextureManager::CopyTexture(std::string newTextureName, std::string oldTextureName)
{
	const Texture& oldTex = textures.find(oldTextureName)->second;
	glBindTexture(GL_TEXTURE_2D, oldTex.GetHandle());
	glm::ivec2 texSize = oldTex.GetTextureSize();
	std::pair<std::string, Texture> newTex;
	newTex.first = newTextureName;
	newTex.second.CopyTexture(texSize.x, texSize.y, textureSize++, oldTex.GetHandle());
	textures.insert(newTex);
}

GLuint TextureManager::GetTextureHandle(std::string textureName)
{
	return textures.find(textureName)->second.GetHandle();
}

glm::ivec2 TextureManager::GetTextureSize(std::string textureName)
{
	return textures.find(textureName)->second.GetTextureSize();
}

glm::vec4 TextureManager::ReadPixelData(std::string textureName, int x, int y)
{
	return textures.find(textureName)->second.ReadPixelData(x, y);
}

void TextureManager::AddPerlineNoise1D(std::string textureName, int width, double frequency, int octaves)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupPerlineTexture1D(width, textureSize++, frequency, octaves);
	textures.insert(newTex);
}

void TextureManager::UpdatePerlinNoise1D(std::string textureName, double frequency, int octaves)
{
	textures[textureName].UpdatePerlineTexture1D(frequency, octaves);
}

void TextureManager::AddPerlineNoise2D(std::string textureName, int width, int height, double frequency, int octaves)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupPerlineTexture2D(width, height, textureSize++, frequency, octaves);
	textures.insert(newTex);
}

void TextureManager::UpdatePerlinNoise2D(std::string textureName, double frequency, int octaves)
{
	textures[textureName].UpdatePerlineTexture2D(frequency, octaves);
}

void TextureManager::AddPerlineNoise3D(std::string textureName, int width, int height, int depth, double frequency, int octaves)
{
	std::pair<std::string, Texture> newTex;
	newTex.first = textureName;
	newTex.second.SetupPerlineTexture3D(width, height, depth, textureSize++, frequency, octaves);
	textures.insert(newTex);
}

void TextureManager::UpdatePerlinNoise3D(std::string textureName, double frequency, int octaves)
{
	textures[textureName].UpdatePerlineTexture3D(frequency, octaves);
}

void TextureManager::Clear()
{
	for (auto t = textures.begin(); t != textures.end(); t++)
	{
		t->second.Clear();
	}
}
