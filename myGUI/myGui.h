/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: myGUI.h
Purpose: myGUI implementation!
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/23/2021
End Header --------------------------------------------------------*/

#include <vector>
#include <../Common/Scene.h>

struct GLFWwindow;
class Mesh;

class myGUI : public Scene
{
public:
	myGUI();
	virtual ~myGUI();

	// techniqually works like Init();
	void SetupGUI(GLFWwindow* window);
	void Update();
	void CleanUp();

	void SetupNanoGUI(GLFWwindow*);

	void AddFlagParameter(bool* flag);

	int preRender();
	// update member variable in here.
	int postRender();

	// [(-100.f, -100.f), (100.f, 100.f)]
	void ConnectHandlerDisplacement(glm::vec2* parameter);
private:
	void InitGraphics();
	void DrawHandler();
	void DrawFlags();
	void HandleInputs();
	bool CheckAABBCollision(glm::vec2 mousePos, glm::vec2 objPos, glm::vec2 objSize);

	unsigned int GetNumOfAllParameter() const;
private:
	GLFWwindow* guiWindow;

	Mesh* quad;

	GLuint programID;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayID;

	glm::mat4x4 worldToNDC;

	std::vector<bool*> flags;
	bool* pressedFlag;
	bool* hoveredFlag;

	float highestY;
	float lowestY;

	const float objectZ = -0.1f;

	glm::vec2* deltas;
};