/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: as1Scene.cpp
Purpose: main scene source code of this assignment
Language: C++
Platform: Compiler version: -1, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/18/2021
End Header --------------------------------------------------------*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <../Scenes/cs542Scene1.h>
#include <../Common/Meshes/MyObjReader.h>
#include <../Common/Meshes/Mesh.h>
#include <../Common/shader.hpp>
#include <../Common/Meshes/MeshGenerator.h>

#include <../Common/Meshes/MyMeshes/LineMesh.h>
#include <../Common/Meshes/MyMeshes/ObjectMesh.h>

#include <../Common/Cameras/Projection.h>
#include <../Common/Cameras/Affine.h>
#include <../Common/Input.h>

// include ImGUI
#include <../myGUI/myImGUI.h>

#include <../Common/Meshes/models/Model.h>
#include <../Common/shaders/AssimpShader.h>

Scene1::Scene1(int width, int height)
	:Scene(width, height), vertexAttribute(0), normalAttribute(1), numOfFloatVertex(3),
	angleOfRotate(0), vertexNormalFlag(false), faceNormalFlag(false),
	oldX(0.f), oldY(0.f), cameraMovementOffset(0.004f), shouldReload(false), buf("../Common/Meshes/models/bunny.obj"), flip(false), uvImportType(Mesh::UVType::CUBE_MAPPED_UV),
	calculateUVonCPU(true), reloadShader(false), gbufferRenderTargetFlag(false), depthWriteFlag(true)
{
	sphereMesh = new Mesh();
	centralMesh = new Mesh();
	orbitMesh = new Mesh();
	floorMesh = new Mesh();

	centralMatrix = glm::mat4(1.f);
	modelMatrix = glm::mat4(1.f);

	handlerDisplacement = glm::vec2(0.f);

	normalMesh = new LineMesh();
	faceNormalMesh = new LineMesh();
	mainObjMesh = new ObjectMesh();
	spheres = new ObjectMesh();
	sphereOrbit = new LineMesh();
	floorObjMesh = new ObjectMesh();

 	sphericalViewPoint = Point(1.f, 0.f, 3.14f);
	cartesianViewVector = Vector(0.f, 0.f, -1.f);
	cartesianRightVector = Vector(1.f);
	cartesianUpVector = Vector(0.f, 1.f, 0.f);


	eyePoint = Point(0.f, 0.f, 5.f);
	targetPoint = Point(0.f, 0.f, 0.f);
	fov = static_cast<float>(M_PI_2);
	width = _windowWidth;
	height = _windowHeight;
	aspect = static_cast<float>(width) / height;
	nearDistance = 0.01f;
	farDistance = 200.f;
	relativeUp = Vector(0.f, 1.f, 0.f);

	ambient = glm::vec3(0.7f);
	diffuse = glm::vec3(1.f);
	specular = glm::vec3(0.3f);
	ns = 1.f;
	intensityEmissive = glm::vec3(0.04f);

	// Lights
	lightManager.SetLightSize(2);

	intensityFog = glm::vec3(0.8f);
	attenuationConstants = glm::vec3(1.f, 0.5f, 0.25f);

	currentShader = 0;

	myReader = new MyObjReader();
}

Scene1::~Scene1()
{
}

int Scene1::Init()
{
#ifndef _DEBUG
	MeshGenerator::GenerateSphereMesh(*sphereMesh, 0.05f, 16);
	MeshGenerator::GenerateOrbitMesh(*orbitMesh, 1.f, 32);

	myReader->ReadObjFile("../Common/Meshes/models/bunny.obj", centralMesh, false, Mesh::UVType::CUBE_MAPPED_UV);
	myReader->ReadObjFile("../Common/Meshes/models/quad.obj", floorMesh, true, Mesh::UVType::CUBE_MAPPED_UV);

	model = new Model("../Common/Meshes/models/Armadillo.ply");
#endif

	AddMembersToGUI();

	LoadAllShaders();

	InitLights();

	InitGraphics();

	SetupCamera();

	std::vector<std::string> textureNames{ "positionBuffer" , "normalBuffer" , "diffuseBuffer", "specularBuffer" };
	frameBuffer.InitializeCustomBuffer(&textureManager, textureNames);

	return Scene::Init();
}

void Scene1::LoadAllShaders()
{
	programID = LoadShaders("../Common/shaders/As1DiffuseShader.vert",
		"../Common/shaders/As1DiffuseShader.frag");

	phongShading = LoadShaders("../Common/shaders/As2PhongShading.vert",
		"../Common/shaders/As2PhongShading.frag");

	normalDisplayProgramID = LoadShaders("../Common/shaders/normalDisplayShader.vert",
		"../Common/shaders/normalDisplayShader.frag");

	fboCheckShader = LoadShaders("../Common/shaders/CheckRenderedTextureByFBO.vert",
		"../Common/shaders/CheckRenderedTextureByFBO.frag");

	hybridFirstPass = LoadShaders("../Common/542Shaders/As1FirstPassShader.vert",
		"../Common/542Shaders/As1FirstPassShader.frag");
	assimpHybridFirstPass = new AssimpShader(hybridFirstPass);

	hybridPhong = LoadShaders("../Common/542Shaders/As1HybridPhong.vert",
		"../Common/542Shaders/As1HybridPhong.frag");
}

int Scene1::preRender()
{
	ReloadShader();
	ReloadMesh();

	glClearColor(intensityFog.x, intensityFog.y, intensityFog.z, 1.f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::vec3 scaleVector = glm::vec3(1.f);
	const float displacementToPi = glm::pi<float>() / 180.f;
	centralMatrix = glm::rotate(handlerDisplacement.x * displacementToPi, glm::vec3(0.f, 1.f, 0.f)) *
		glm::rotate(handlerDisplacement.y * displacementToPi, glm::vec3(1.f, 0.f, 0.f)) *
		glm::scale(scaleVector) * centralMesh->calcAdjustBoundingBoxMatrix();
	modelMatrix = 
		glm::rotate(180.f * displacementToPi, glm::vec3(0.f, 1.f, 0.f)) *
		glm::scale(scaleVector) * model->CalcAdjustBoundingBoxMatrix();
	floorMatrix = glm::translate(glm::vec3(0.f, -1.f, 0.f)) * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f)) * glm::scale(glm::vec3(10.f, 10.f, 1.f)) * floorMesh->calcAdjustBoundingBoxMatrix();

	for (int i = 0; i < 4; i++)
	{
		gbufferRenderTargetsMatrix[i] = glm::translate(glm::vec3(-1.f + (0.5f*i), -1.f, 0.f)) * glm::scale(glm::vec3(0.5f));
	}

	UpdateCamera();

	Matrix wTN = (CameraToNDC(camera) * WorldToCamera(camera));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			worldToNDC[i][j] = wTN[i][j];
		}
	}
	worldToNDC = glm::transpose(worldToNDC);

	return 0;
}

int Scene1::Render()
{
	HybridRendering();

	return 0;
}

int Scene1::postRender()
{
	constexpr float rotatePerFrame = 0.015f;
	angleOfRotate += rotatePerFrame;
	return 0;
}

void Scene1::CleanUp()
{
	glDeleteProgram(programID);
	glDeleteProgram(phongShading);
	glDeleteProgram(normalDisplayProgramID);
	glDeleteProgram(fboCheckShader);
	glDeleteProgram(hybridFirstPass);
	glDeleteProgram(hybridPhong);


	MyImGUI::ClearImGUI();

	textureManager.Clear();
	frameBuffer.Clear();

	delete sphereMesh;
	delete centralMesh;
	delete normalMesh;
	delete faceNormalMesh;
	delete mainObjMesh;
	delete spheres;
	delete sphereOrbit;
	delete orbitMesh;

	delete myReader;

	delete assimpHybridFirstPass;
}

void Scene1::SetupNanoGUI(GLFWwindow* window)
{
	MyImGUI::InitImGUI(window);
}

void Scene1::UpdateGUI()
{
	MyImGUI::UpdateImGUI();
	UpdateGUIShader();
}

void Scene1::InitGraphics()
{
	textureManager.AddTexture(
		"../Common/ppms/metal_roof_diff_512x512.ppm",
		"diffuseTexture", Texture::TextureType::PPM);
	textureManager.AddTexture(
		"../Common/ppms/metal_roof_spec_512x512.ppm",
		"specularTexture", Texture::TextureType::PPM);

	mainObjMesh->SetShader(phongShading);
	mainObjMesh->Init(centralMesh->getVertexCount(), centralMesh->getVertexBuffer(), centralMesh->getVertexNormals(), centralMesh->getVertexUVs(),
		centralMesh->getIndexBufferSize(), centralMesh->getIndexBuffer());

	std::vector<glm::vec2> uvs;
	uvs.push_back(glm::vec2(0.f, 0.f));
	uvs.push_back(glm::vec2(1.f, 0.f));
	uvs.push_back(glm::vec2(1.f, 1.f));
	uvs.push_back(glm::vec2(0.f, 1.f));
	floorObjMesh->SetShader(phongShading);
	floorObjMesh->Init(floorMesh->getVertexCount(), floorMesh->getVertexBuffer(), floorMesh->getVertexNormals(), reinterpret_cast<GLfloat*>(uvs.data()),
		floorMesh->getIndexBufferSize(), floorMesh->getIndexBuffer());


	// normal inits
	normalMesh->SetShader(normalDisplayProgramID);
	normalMesh->Init(model->GetVertexNormalCount(), model->GetVertexNormalsForDisplay());

	// Face Inits
	faceNormalMesh->SetShader(normalDisplayProgramID);
	faceNormalMesh->Init(model->GetFaceNormalCount(), model->GetFaceNormalsForDisplay());


	spheres->SetShader(programID);
	spheres->Init(sphereMesh->getVertexCount(), sphereMesh->getVertexBuffer(), sphereMesh->getVertexNormals(), sphereMesh->getVertexUVs(),
		sphereMesh->getIndexBufferSize(), sphereMesh->getIndexBuffer());

	sphereOrbit->SetShader(normalDisplayProgramID);
	sphereOrbit->Init(orbitMesh->getVertexBufferSize(), orbitMesh->getVertexBuffer());


}

void Scene1::AddMembersToGUI()
{
	MyImGUI::SetNormalDisplayReferences(&vertexNormalFlag, &faceNormalFlag);
	MyImGUI::SetMaterialReferences(&ambient.x, &diffuse.x, &specular.x, reinterpret_cast<float*>(&intensityEmissive));
	MyImGUI::SetLightReferences(&lightManager);
	MyImGUI::SetEnvironmentReferences(reinterpret_cast<float*>(&intensityFog), reinterpret_cast<float*>(&attenuationConstants));
	MyImGUI::SetShaderReferences(&currentShader, &reloadShader);
	MyImGUI::SetCentralMesh(centralMesh, mainObjMesh, &shouldReload, buf, &flip, &uvImportType, &calculateUVonCPU);
	MyImGUI::SetHybridDebugging(&gbufferRenderTargetFlag, &depthWriteFlag);
}
void Scene1::Draw2ndPass()
{
	UpdateLights();

	 
	 
	floorObjMesh->SetShader(hybridPhong);
	floorObjMesh->PrepareDrawing();

	textureManager.ActivateTexture(floorObjMesh->GetShader(), "diffuseBuffer");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "specularBuffer");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "positionBuffer");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "normalBuffer");

	glm::mat4 diffuseObjToWorld = glm::translate(glm::vec3(-1.f, -1.f, 0.f)) * glm::scale(glm::vec3(2.f));
	floorObjMesh->SendUniformFloatMatrix4("objToWorld", &diffuseObjToWorld[0][0]);
	
	Point c = camera.Eye();

	floorObjMesh->SendUniformFloat3("cameraPosition", &c.x);

	floorObjMesh->SendUniformInt("width", width);
	floorObjMesh->SendUniformInt("height", height);
	floorObjMesh->SendUniformFloat3("ambient", &ambient.x);
	floorObjMesh->SendUniformFloat3("diffuse", &diffuse.x);
	floorObjMesh->SendUniformFloat3("specular", &specular.x);
	floorObjMesh->SendUniformFloat("ns", ns);
	floorObjMesh->SendUniformFloat("zNear", nearDistance);
	floorObjMesh->SendUniformFloat("zFar", farDistance);
	floorObjMesh->SendUniformFloat3("intensityEmissive", &intensityEmissive.x);
	floorObjMesh->SendUniformFloat3("intensityFog", &intensityFog.x);
	floorObjMesh->SendUniformFloat3("attenuationConstants", &attenuationConstants.x);
	floorObjMesh->SendUniformInt("lightSize", *lightManager.GetCurrentLightSizeReference());
	
	// uniform block data
	// Uniform Block Update Starts Here
	floorObjMesh->SendUniformBlockVector3s("Block", lightManager.GetLightUniformDataSize(),
		lightManager.GetLightUniformBlockNames(), lightManager.GetLightUniformBlockData());

	floorObjMesh->Draw(floorMesh->getIndexBufferSize());
	// End here..
}

void Scene1::DrawVertexNormals()
{
	normalMesh->PrepareDrawing();

	glm::vec3 lineColor;
	lineColor.r = 1.f;
	lineColor.g = 1.f;
	lineColor.b = 1.f;
	normalMesh->SendUniformFloat3("lineColor", reinterpret_cast<float*>(&lineColor));
	normalMesh->SendUniformFloatMatrix4("objToWorld", &modelMatrix[0][0]);
	normalMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	normalMesh->Draw(model->GetVertexNormalCount());
}

void Scene1::DrawFaceNormals()
{
	faceNormalMesh->PrepareDrawing();

	glm::vec3 lineColor;
	lineColor.r = 1.f;
	lineColor.g = 1.f;
	lineColor.b = 0.f;
	faceNormalMesh->SendUniformFloat3("lineColor", &lineColor[0]);
	faceNormalMesh->SendUniformFloatMatrix4("objToWorld", &modelMatrix[0][0]);
	faceNormalMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	faceNormalMesh->Draw(model->GetFaceNormalCount());
}
void Scene1::DrawSpheresAndOrbit(glm::vec3 position, glm::vec3 rotateAxis, float rotateScale, float initRotate, glm::vec3 diffuseColor, glm::mat4 matrix)
{
	spheres->PrepareDrawing();

	sphereMatrix = matrix;
	glm::vec3 orbitFaceNormal = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 orbitRotateAxis = glm::cross(orbitFaceNormal, rotateAxis);
	float orbitAngle = acos(glm::dot(orbitFaceNormal, rotateAxis) / (glm::length(orbitFaceNormal) * glm::length(rotateAxis)));
	glm::mat4 orbitMatrix;
	if (glm::length(orbitRotateAxis) < 1.f)
	{
		orbitMatrix = glm::scale(glm::vec3(glm::length(position)));
	}
	else
	{
		orbitMatrix = glm::rotate(orbitAngle, orbitRotateAxis) * glm::scale(glm::vec3(glm::length(position)));
	}

	spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	spheres->SendUniformFloatMatrix4("objToWorld", &sphereMatrix[0][0]);
	spheres->SendUniformFloat3("diffuseColor", &diffuseColor[0]);
	spheres->Draw(sphereMesh->getIndexBufferSize());

	sphereOrbit->PrepareDrawing();
	glm::vec3 lineColor = glm::vec3(1.f);
	sphereOrbit->SendUniformFloat3("lineColor", &lineColor[0]);
	sphereOrbit->SendUniformFloatMatrix4("objToWorld", &orbitMatrix[0][0]);
	sphereOrbit->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	sphereOrbit->Draw(orbitMesh->getVertexBufferSize());
}

void Scene1::Draw1stPass()
{
	frameBuffer.ApplyFBO();

	//////////////////////////////////////////////////////////////////////////////////// Draw starts
	//mainObjMesh->SetShader(hybridFirstPass);

	//mainObjMesh->PrepareDrawing();

	//mainObjMesh->SendUniformFloatMatrix4("objToWorld", &centralMatrix[0][0]);
	//mainObjMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);

	//mainObjMesh->Draw(centralMesh->getIndexBufferSize());

	floorObjMesh->SetShader(hybridFirstPass);
	floorObjMesh->PrepareDrawing();

	floorObjMesh->SendUniformFloatMatrix4("objToWorld", &floorMatrix[0][0]);
	floorObjMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	glm::vec3 floorDiffuse(1.f, 1.f, 1.f);
	glm::vec3 floorSpecular(0.3f, 0.3f, 0.3f);
	floorObjMesh->SendUniformFloat3("diffuse", &floorDiffuse[0]);
	floorObjMesh->SendUniformFloat3("specular", &floorSpecular[0]);

	floorObjMesh->Draw(floorMesh->getIndexBufferSize());

	////////////////////////////////////////////////////////////////////////////////////// Draw ends

	assimpHybridFirstPass->Use();
	assimpHybridFirstPass->SendUniformFloatMatrix4("objToWorld", &modelMatrix[0][0]);
	assimpHybridFirstPass->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	assimpHybridFirstPass->SendUniformFloat3("diffuse", &diffuse[0]);
	assimpHybridFirstPass->SendUniformFloat3("specular", &specular[0]);
	model->Draw(hybridFirstPass);

	

	frameBuffer.RestoreDefaultFrameBuffer();
}

void Scene1::SetupCamera()
{
	camera = Camera(eyePoint, targetPoint - eyePoint, relativeUp, fov, aspect, nearDistance, farDistance);
}

void Scene1::MoveViewOfCamera(int x, int y)
{

	sphericalViewPoint.z -= (x - oldX) * cameraMovementOffset;
	oldX = static_cast<float>(x);
	sphericalViewPoint.y += (y - oldY) * cameraMovementOffset;
	if (sphericalViewPoint.y >= static_cast<float>(M_PI_2))
	{
		sphericalViewPoint.y = static_cast<float>(M_PI_2);
	}
	else if (sphericalViewPoint.y <= -static_cast<float>(M_PI_2))
	{
		sphericalViewPoint.y = -static_cast<float>(M_PI_2);
	}
	oldY = static_cast<float>(y);

	cartesianViewVector.x = cos(sphericalViewPoint.y) * sin(sphericalViewPoint.z);
	cartesianViewVector.y = sin(sphericalViewPoint.y);
	cartesianViewVector.z = cos(sphericalViewPoint.y) * cos(sphericalViewPoint.z);

	cartesianViewVector = normalize(cartesianViewVector);
	cartesianRightVector = cross(cartesianViewVector, relativeUp);
	cartesianUpVector = cross(cartesianRightVector, cartesianViewVector);

	targetPoint = eyePoint + cartesianViewVector;

	camera = Camera(eyePoint, targetPoint - eyePoint, relativeUp, fov, aspect, nearDistance, farDistance);
}

void Scene1::MovePositionOfCamera(float speed, short moveDirection)
{
	Hcoord displacementOfEye;

	if (moveDirection & FORWARD)
	{
		displacementOfEye = displacementOfEye + (speed * cartesianViewVector);
	}
	if (moveDirection & BACKWARD)
	{
		displacementOfEye = displacementOfEye + (-speed * cartesianViewVector);
	}
	if (moveDirection & RIGHTWARD)
	{
		displacementOfEye = displacementOfEye + (speed * cartesianRightVector);
	}
	if (moveDirection & LEFTWARD)
	{
		displacementOfEye = displacementOfEye + (-speed * cartesianRightVector);
	}
	if (moveDirection & UPWARD)
	{
		displacementOfEye = displacementOfEye + (speed * cartesianUpVector);
	}
	if (moveDirection & DOWNWARD)
	{
		displacementOfEye = displacementOfEye + (-speed * cartesianUpVector);
	}

	eyePoint.x += displacementOfEye.x;
	eyePoint.y += displacementOfEye.y;
	eyePoint.z += displacementOfEye.z;

	targetPoint = eyePoint + cartesianViewVector;


	camera = Camera(eyePoint, targetPoint - eyePoint, relativeUp, fov, aspect, nearDistance, farDistance);
}

void Scene1::UpdateCamera()
{
	float speed = 0.01f;

	if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		speed = 0.03f;
	}

	short moveDirection = 0;

	if (input.IsKeyPressed(GLFW_KEY_W))
	{
		moveDirection |= FORWARD;
	}
	if (input.IsKeyPressed(GLFW_KEY_S))
	{
		moveDirection |= BACKWARD;
	}
	if (input.IsKeyPressed(GLFW_KEY_A))
	{
		moveDirection |= LEFTWARD;
	}
	if (input.IsKeyPressed(GLFW_KEY_D))
	{
		moveDirection |= RIGHTWARD;
	}
	if (input.IsKeyPressed(GLFW_KEY_E))
	{
		moveDirection |= UPWARD;
	}
	if (input.IsKeyPressed(GLFW_KEY_Q))
	{
		moveDirection |= DOWNWARD;
	}

	MovePositionOfCamera(speed, moveDirection);

	if (input.IsMouseButtonTriggered(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec2 mousePos = input.GetMousePosition();
		oldX = mousePos.x;
		oldY = mousePos.y;
	}
	else if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec2 mousePos = input.GetMousePosition();
		MoveViewOfCamera(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
	}
}

void Scene1::UpdateGUIShader()
{
	switch (currentShader)
	{
	case 0:
		mainObjMesh->SetShader(phongShading);
		break;
	default:
		break;
	}
}

void Scene1::InitLights()
{
	static const glm::vec3 initTranslations[16] = {
		glm::vec3(3.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 3.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(3.f, 0.f, 0.f),
		glm::vec3(0.f, -3.f / sqrt(2.f), -3.f / sqrt(2.f)),
		glm::vec3(0.f, 0.f, 3.f),
		glm::vec3(3.f / sqrt(2.f), 3.f / sqrt(2.f), 0.f),
		glm::vec3(0.f, -3.f, 0.f),

		glm::vec3(-3.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, -3.f),
		glm::vec3(0.f, -3.f, 0.f),
		glm::vec3(-3.f, 0.f, 0.f),
		glm::vec3(0.f, 3.f / sqrt(2.f), 3.f / sqrt(2.f)),
		glm::vec3(0.f, 0.f, -3.f),
		glm::vec3(-3.f / sqrt(2.f), -3.f / sqrt(2.f), 0.f),
		glm::vec3(0.f, 3.f, 0.f),
	};
	static const glm::vec3 initRotationAxis[16] = {
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 1.f),
		glm::vec3(0.f, 1.f, -1.f),
		glm::vec3(1.f, 1.f, 0.f),
		glm::vec3(1.f, -1.f, 0.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 1.f),
		glm::vec3(0.f, 1.f, -1.f),
		glm::vec3(1.f, 1.f, 0.f),
		glm::vec3(1.f, -1.f, 0.f),
		glm::vec3(1.f, 0.f, 1.f),

	};
	for (int i = 0; i < LightManager::MAX_LIGHT; i++)
	{
		Light& light = lightManager.GetLightReference(i);
		light.SetLightTranslation(initTranslations[i]);
		light.SetLightRotationAxis(initRotationAxis[i]);
		light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
		light.SetLightSpotFalloff(1.f);
	}
}

void Scene1::UpdateLights()
{
	const int lightSize = *lightManager.GetCurrentLightSizeReference();
	for (int i = 0; i < lightSize; i++)
	{
		Light& light = lightManager.GetLightReference(i);
		DrawSpheresAndOrbit(light.GetLightTranslation(), light.GetLightRotationAxis(), *light.GetLightRotationScalerReference(), *light.GetInitLightRotationReference(), *reinterpret_cast<glm::vec3*>(light.GetDiffusePointer()), light.GetMatrix());
		light.UpdateLightPosition(angleOfRotate);
		light.SetLightDirection(glm::vec3(0.f) - light.GetLightPosition());
	}
}

void Scene1::ReloadShader()
{
	if (reloadShader)
	{
		reloadShader = false;
		
		LoadAllShaders();
	}
}

void Scene1::ReloadMesh()
{
	if (shouldReload)
	{
		shouldReload = false;

		//centralMesh->initData();

		//myReader->ReadObjFile(buf, centralMesh, flip, uvImportType);
		model->LoadModel(buf);

		//mainObjMesh->Init(centralMesh->getVertexCount(), centralMesh->getVertexBuffer(), centralMesh->getVertexNormals(), centralMesh->getVertexUVs(),
		//	centralMesh->getIndexBufferSize(), centralMesh->getIndexBuffer());

		normalMesh->Init(model->GetVertexNormalCount(), model->GetVertexNormalsForDisplay());
		faceNormalMesh->Init(model->GetFaceNormalCount(), model->GetFaceNormalsForDisplay());
	}
}

void Scene1::HybridRendering()
{
	RenderDeferredObjects();

	CopyDepthInfo();

	RenderDebugObjects();
}

void Scene1::RenderDeferredObjects()
{
	// a. Implement the Deferred Shading pipeline for rendering the OBJ files using FBO's (let's call this FBO, FBO1.)
	// b. First pass: Generate the G-buffer using multiple render targets (Refer CS300 Assignment on Dynamic Reflection & Refraction)
	// c. The render targets will hold the information to implement Phong Shading in the second (Lighting) pass.
	// d. Second Pass: Use the render targets from Pass 1 as input textures to the Lighting Pass.
	//								The Vertex Shader will be a straight pass - through to render a Full Screen Quad (FSQ).
	//								The Fragment Shader will implement the Phong Shading, 
	//									but reading the input values for material / environment properties from the textures.

	Draw1stPass();

	


	// disable depthwriting for temporarily
	glDisable(GL_DEPTH_TEST);

	Draw2ndPass();

	glEnable(GL_DEPTH_TEST);
}

void Scene1::CopyDepthInfo()
{
	if (depthWriteFlag == true)
	{
		frameBuffer.CopyDepthInfo();
	}
}

void Scene1::RenderDebugObjects()
{
	// a. Now draw the face normal and vertex normal using the Forward Rendering method.
	//		Essentially, your code to render these objects should be unchanged from CS300.
	// b. Do not apply Phong Shading to debug draw objects.
	// c. In future assignments, we will expand our repertoire of "debug" objects to include bounding volumes, trees, and rays.

	UpdateLights();

	if (vertexNormalFlag == true)
	{
		DrawVertexNormals();
	}

	if (faceNormalFlag == true)
	{
		DrawFaceNormals();
	}

	if (gbufferRenderTargetFlag == true)
	{
		DrawGBufferRenderTargets();
	}
}

void Scene1::DrawGBufferRenderTargets()
{
	floorObjMesh->SetShader(fboCheckShader);
	floorObjMesh->PrepareDrawing();
	textureManager.ActivateTexture(fboCheckShader, "positionBuffer", "tex");
	floorObjMesh->SendUniformFloatMatrix4("trans", &gbufferRenderTargetsMatrix[0][0][0]);
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());


	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloatMatrix4("trans", &gbufferRenderTargetsMatrix[1][0][0]);
	textureManager.ActivateTexture(fboCheckShader, "normalBuffer", "tex");
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());


	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloatMatrix4("trans", &gbufferRenderTargetsMatrix[2][0][0]);
	textureManager.ActivateTexture(fboCheckShader, "diffuseBuffer", "tex");
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());

	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloatMatrix4("trans", &gbufferRenderTargetsMatrix[3][0][0]);
	textureManager.ActivateTexture(fboCheckShader, "specularBuffer", "tex");
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());
}
