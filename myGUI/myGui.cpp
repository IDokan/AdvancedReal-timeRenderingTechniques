/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: myGUI.cpp
Purpose: myGUI implementation!
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/23/2021
End Header --------------------------------------------------------*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include <../myGUI/myGui.h>

#include <../Common/Meshes/MyObjReader.h>
#include <../Common/Meshes/Mesh.h>
#include <../Common/shader.hpp>

#include <../Common/Input.h>

namespace
{
	constexpr GLuint vertexAttribute = 0;
	constexpr GLuint normalAttribute = 1;
	constexpr float boardZ = 0.f;
	constexpr float flagButtonSize = 100.f;
	constexpr float boardSize = 200.f;
	constexpr float handlerSize = 20.f;
}

myGUI::myGUI()
	: guiWindow(nullptr), quad(nullptr), programID(0), vertexBuffer(0), indexBuffer(0), vertexArrayID(0), pressedFlag(nullptr), hoveredFlag(nullptr), highestY(1.f), lowestY(-1.f), deltas(nullptr)
{
	flags.clear();
}

myGUI::~myGUI()
{
	CleanUp();
	delete quad;
}

void myGUI::SetupGUI(GLFWwindow* window)
{

	guiWindow = window;

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	worldToNDC = glm::scale(glm::vec3(2.f / width, 2.f / height, 1.f));
	highestY = height / 2.f;
	lowestY = -height / 2.f;

	if (guiWindow == nullptr)
	{
		return;
	}

	InitGraphics();
}

void myGUI::Update()
{
	HandleInputs();

	preRender();

	DrawHandler();
	DrawFlags();

	postRender();
}

void myGUI::CleanUp()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
}

void myGUI::SetupNanoGUI(GLFWwindow*)
{
}

void myGUI::AddFlagParameter(bool* flag)
{
	flags.push_back(flag);
}

int myGUI::preRender()
{
	glClearColor(0.2f, 0.3f, 0.75f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return 0;
}

int myGUI::postRender()
{
	return 0;
}

void myGUI::ConnectHandlerDisplacement(glm::vec2* parameter)
{
	deltas = parameter;
}

void myGUI::InitGraphics()
{
	quad = new Mesh();
	MyObjReader reader;
	reader.ReadObjFile("../Common/Meshes/models/quad.obj", quad);


	programID = LoadShaders("../Common/shaders/GUIShader.vert",
		"../Common/shaders/GUIShader.frag");

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, quad->getVertexNormalCount() * sizeof(GLfloat) * 3, quad->getVertexNormalsForDisplay(), GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad->getIndexBufferSize() * sizeof(GLuint), quad->getIndexBuffer(), GL_STATIC_DRAW);
}

void myGUI::DrawHandler()
{

	glUseProgram(programID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableVertexAttribArray(vertexAttribute);
	glEnableVertexAttribArray(normalAttribute);

	glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3) * 2, (void*)0);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3) * 2, (void*)(3 * sizeof(float)));

	GLint vTransformLoc = glGetUniformLocation(programID, "vertexTransform");
	GLint vColorLoc = glGetUniformLocation(programID, "uniformColor");

	// Calculate Y position;
	const unsigned int numOfItems = GetNumOfAllParameter();
	float interval = (highestY - lowestY) / (numOfItems + 1);
	float yPosition = lowestY + (interval * (numOfItems));

	// Draw Outer Board ************************************
	glm::mat4 modelMat = glm::mat4(1.f);
	glm::vec3 scaleVector = glm::vec3(boardSize);
	glm::vec3 centroid = glm::vec3(-0.5f, -0.5f, boardZ);
	glm::vec3 translate = glm::vec3(0.f, yPosition, 0.f);
	modelMat = worldToNDC * glm::translate(translate) * glm::rotate(0.f, glm::vec3(0.f, 0.f, 1.f)) * glm::scale(scaleVector) * glm::translate(centroid);

	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);

	glm::vec3 color;
	color.r = 0.25f;
	color.g = 0.25f;
	color.b = 0.25f;
	glUniform3fv(vColorLoc, 1, &color[0]);

	glDrawElements(GL_TRIANGLES, quad->getIndexBufferSize(), GL_UNSIGNED_INT, 0);
	// Outer board drawing is end..

	// Draw Inner handler ************************************
	scaleVector = glm::vec3(handlerSize);
	modelMat = worldToNDC * glm::translate(translate + glm::vec3(*deltas, 0.f)) * glm::rotate(0.f, glm::vec3(0.f, 0.f, 1.f)) * glm::scale(scaleVector) * glm::translate(centroid);

	glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);

	color.r = 1.f;
	color.g = 1.f;
	color.b = 1.f;
	glUniform3fv(vColorLoc, 1, &color[0]);

	glDrawElements(GL_TRIANGLES, quad->getIndexBufferSize(), GL_UNSIGNED_INT, 0);
	// Drawing Inner handle is end..

	glDisableVertexAttribArray(vertexAttribute);
	glDisableVertexAttribArray(normalAttribute);
}

void myGUI::DrawFlags()
{
	glUseProgram(programID);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableVertexAttribArray(vertexAttribute);
	glEnableVertexAttribArray(normalAttribute);

	glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3) * 2, (void*)0);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3) * 2, (void*)(3 * sizeof(float)));

	GLint vTransformLoc = glGetUniformLocation(programID, "vertexTransform");
	GLint vColorLoc = glGetUniformLocation(programID, "uniformColor");


	const unsigned int numOfFlag = GetNumOfAllParameter();
	float interval = (highestY - lowestY) / (numOfFlag + 1);
	float currentY = lowestY;
	for (const bool* flag : flags)
	{
		currentY += interval;
		glm::mat4 modelMat = glm::mat4(1.f);
		glm::vec3 scaleVector = glm::vec3(flagButtonSize, flagButtonSize, 1.f);
		glm::vec3 centroid = glm::vec3(-0.5f, -0.5f, 0.f);
		glm::vec3 translation = glm::vec3(0.f, currentY, objectZ);
		modelMat = worldToNDC * glm::translate(translation) * glm::scale(scaleVector) * glm::translate(centroid);

		glUniformMatrix4fv(vTransformLoc, 1, GL_FALSE, &modelMat[0][0]);

		glm::vec3 color;
		if (*flag)
		{
			color.r = 0.2f;
			color.g = 0.75f;
			color.b = 0.f;
		}
		else
		{
			color.r = 0.8f;
			color.g = 0.3f;
			color.b = 0.3f;
		}

		if (flag == pressedFlag)
		{
			color = glm::vec3(0.f);
		}
		else if(flag == hoveredFlag)
		{
			color *= 0.75f;
		}
		glUniform3fv(vColorLoc, 1, &color[0]);

		glDrawElements(GL_TRIANGLES, quad->getIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(vertexAttribute);
	glDisableVertexAttribArray(normalAttribute);
}

void myGUI::HandleInputs()
{
	glm::vec2 pos = input.GetMousePosition();

	int numOfAllItems = GetNumOfAllParameter();
	int numOfFlag = static_cast<int>(flags.size());
	float interval = (highestY - lowestY) / (numOfAllItems + 1);
	float y = lowestY;

	bool isMouseOverObject = false;
	bool isMousePressedObject = false;

	for (int i = 0; i < numOfFlag; ++i)
	{
		y += interval;

		if (CheckAABBCollision(pos, glm::vec2(0.f, y), glm::vec2(flagButtonSize)))
		{
			// Flag handle
			isMouseOverObject = true;

			bool* ptrFlag = flags.at(i);

			hoveredFlag = ptrFlag;
			
			if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				isMousePressedObject = true;
				pressedFlag = ptrFlag;
			}

			if (input.IsMouseButtonTriggered(GLFW_MOUSE_BUTTON_LEFT))
			{
				*ptrFlag = !(*ptrFlag);
			}
		}
	}

	// Initialize pointers when user did nothing.
	if (isMouseOverObject == false)
	{
		hoveredFlag = nullptr;
	}
	if (isMousePressedObject == false)
	{
		pressedFlag = nullptr;
	}

	y += interval;
	// Handle of the case of handler
	glm::vec2 handlerPosition = glm::vec2(0.f, y);
	// Check if the button is pressed
	if (CheckAABBCollision(pos, handlerPosition + (*deltas), glm::vec2(handlerSize + glm::vec2(30.f))))
	{
		if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (CheckAABBCollision(pos, handlerPosition, glm::vec2(boardSize)))
			{
				*deltas = pos - handlerPosition;
			}
		}
	}
}

bool myGUI::CheckAABBCollision(glm::vec2 mousePos, glm::vec2 objPos, glm::vec2 objSize)
{
	return (mousePos.x >= objPos.x - (objSize.x / 2.f)) &&
		(mousePos.x <= objPos.x + (objSize.x / 2.f)) &&
		(mousePos.y >= objPos.y - (objSize.y / 2.f)) &&
		(mousePos.y <= objPos.y + (objSize.y / 2.f));
}

unsigned int myGUI::GetNumOfAllParameter() const
{
	// items + only one handler item
	return static_cast<unsigned int>(flags.size() + 1);
}
