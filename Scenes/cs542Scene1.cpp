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
#include <../Common/Meshes/MyMeshes/ComputeShaderDispatcher.h>

Scene1::Scene1(int width, int height)
	:Scene(width, height), vertexAttribute(0), normalAttribute(1), numOfFloatVertex(3),
	angleOfRotate(0), vertexNormalFlag(false), faceNormalFlag(false),
	oldX(0.f), oldY(0.f), cameraMovementOffset(0.004f), shouldReload(false), buf("../Common/Meshes/models/bunny.obj"), flip(false), uvImportType(Mesh::UVType::CUBE_MAPPED_UV),
	calculateUVonCPU(true), reloadShader(false), gbufferRenderTargetFlag(false), depthWriteFlag(true), shadowBufferSize(1024), blurStrength(0), bias(0.001f),
	cubePath("../Common/Meshes/models/cube.obj"), exposure(1.f), contrast(1.f), roughness(0.001f), backgroundImageWidth(2048), backgroundImageHeight(1024),
	irradianceMapWidth(512), irradianceMapHeight(256), localLightFlag(false), debugObjectsOpacity(0.5f)
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
	floorObjMesh = new ObjectMesh();

	sphericalViewPoint = Point(1.f, 0.f, 3.14f);
	cartesianViewVector = Vector(0.f, 0.f, -1.f);
	cartesianRightVector = Vector(1.f);
	cartesianUpVector = Vector(0.f, 1.f, 0.f);


	eyePoint = Point(0.f, 0.f, 5.f);
	targetPoint = Point(0.f, 0.f, 0.f);
	fov = static_cast<float>(M_PI_2);
	aspect = static_cast<float>(width) / height;
	nearDistance = 0.01f;
	farDistance = 200.f;
	relativeUp = Vector(0.f, 1.f, 0.f);

	ambient = glm::vec3(0.1f);
	diffuse = glm::vec3(0.5f);
	specular = glm::vec3(0.3f);
	ns = 1.f;
	intensityEmissive = glm::vec3(0.04f);

	// Lights
	lightManager.SetLightSize(2);

	intensityFog = glm::vec3(0.8f);
	attenuationConstants = glm::vec3(1.f, 0.5f, 0.25f);

	currentShader = 0;

	myReader = new MyObjReader();

	sphereEnvironmentalSize = 10;

	cubeMesh = new Mesh();
	cubeObjMesh = new ObjectMesh();
}

Scene1::~Scene1()
{
}

int Scene1::Init()
{
#ifndef _DEBUG
#endif
	MeshGenerator::GenerateSphereMesh(*sphereMesh, 1.f, 15);
	MeshGenerator::GenerateOrbitMesh(*orbitMesh, 1.f, 32);

	myReader->ReadObjFile("../Common/Meshes/models/bunny.obj", centralMesh, false, Mesh::UVType::CUBE_MAPPED_UV);
	myReader->ReadObjFile("../Common/Meshes/models/quad.obj", floorMesh, false, Mesh::UVType::CUBE_MAPPED_UV);
	myReader->ReadObjFile(cubePath, cubeMesh, false, Mesh::UVType::CUBE_MAPPED_UV);

	model = new Model("../Common/Meshes/models/Armadillo.ply");

	InitGraphics();
	skyboxFB.InitializeCubemap(&textureManager, "SkyCubeMap", 512, 512);


	AddMembersToGUI();

	LoadAllShaders();

	RecordSkyImage();
	RecordSkybox();
	DispatchIrradianceMapMaker();
	glViewport(0, 0, _windowWidth, _windowHeight);

	InitLights();

	SetupCamera();

	std::vector<std::string> textureNames{ "positionBuffer" , "normalBuffer" , "diffuseBuffer", "specularBuffer" };

	frameBuffer.InitializeCustomBuffer(&textureManager, textureNames);

	return Scene::Init();
}

void Scene1::LoadAllShaders()
{
	programID = LoadShaders("../Common/shaders/As1DiffuseShader.vert",
		"../Common/shaders/As1DiffuseShader.frag");

	opacityShader = LoadShaders("../Common/542shaders/OpacityShader.vert",
		"../Common/542shaders/OpacityShader.frag");

	phongShading = LoadShaders("../Common/shaders/As2PhongShading.vert",
		"../Common/shaders/As2PhongShading.frag");

	normalDisplayProgramID = LoadShaders("../Common/shaders/normalDisplayShader.vert",
		"../Common/shaders/normalDisplayShader.frag");

	fboCheckShader = LoadShaders("../Common/shaders/CheckRenderedTextureByFBO.vert",
		"../Common/shaders/CheckRenderedTextureByFBO.frag");

	hybridFirstPass = LoadShaders("../Common/542Shaders/As1FirstPassShader.vert",
		"../Common/542Shaders/As1FirstPassShader.frag");
	assimpHybridFirstPass = new AssimpShader(hybridFirstPass);

	hybridTextureFirstPass = LoadShaders("../Common/542Shaders/As1FirstPassTextureShader.vert",
		"../Common/542Shaders/As1FirstPassTextureShader.frag");

	hybridPhong = LoadShaders("../Common/542Shaders/As1HybridPhong.vert",
		"../Common/542Shaders/As1HybridPhong.frag");

	hybridLocalLights = LoadShaders("../Common/542Shaders/As1LocalLightsPass.vert",
		"../Common/542Shaders/As1LocalLightsPass.frag");

	shadowPass = LoadShaders("../Common/542Shaders/As2ShadowPass.vert",
		"../Common/542Shaders/As2ShadowPass.frag");
	assimpShadowPass = new AssimpShader(shadowPass);

	horizontalFilter = new ComputeShaderDispatcher("../Common/542Shaders/As2SATHorizontal.comp");
	verticalFilter = new ComputeShaderDispatcher("../Common/542Shaders/As2SATVertical.comp");
	blurFilterShader = new ComputeShaderDispatcher("../Common/542Shaders/As2SATBlurFilter.comp");

	skydomeShader = LoadShaders("../Common/542Shaders/As3Skydome.vert",
		"../Common/542Shaders/As3Skydome.frag");

	skyboxRecorderShader = LoadShaders("../Common/542shaders/As3SkyboxRecorder.vert",
		"../Common/542shaders/As3SkyboxRecorder.frag");

	projectionInputImageCalculator00 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker00.comp");
	projectionInputImageCalculator10 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker10.comp");
	projectionInputImageCalculator11 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker11.comp");
	projectionInputImageCalculator1_1 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker1_1.comp");
	projectionInputImageCalculator20 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker20.comp");
	projectionInputImageCalculator21 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker21.comp");
	projectionInputImageCalculator2_1 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker2_1.comp");
	projectionInputImageCalculator22 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker22.comp");
	projectionInputImageCalculator2_2 = new ComputeShaderDispatcher("../Common/542Shaders/As3IrradianceMaker2_2.comp");
	irr = new ComputeShaderDispatcher("../Common/542Shaders/As3Irr.comp");

	recorderShader = LoadShaders("../Common/542shaders/As3Recorder.vert",
		"../Common/542shaders/As3Recorder.frag");
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
		// glm::rotate(180.f * displacementToPi, glm::vec3(0.f, 1.f, 0.f)) *
		glm::scale(scaleVector) * model->CalcAdjustBoundingBoxMatrix();
	floorMatrix = glm::translate(glm::vec3(0.f, -1.f, 0.f)) * glm::rotate(glm::half_pi<float>(), glm::vec3(-1.f, 0.f, 0.f)) * glm::scale(glm::vec3(10.f, 10.f, 1.f)) * floorMesh->calcAdjustBoundingBoxMatrix();

	for (int i = 0; i < 4; i++)
	{
		gbufferRenderTargetsMatrix[i] = glm::translate(glm::vec3(-1.f + (0.5f * i), -1.f, 0.f)) * glm::scale(glm::vec3(0.5f));
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

	Matrix light = CameraToNDC(lightCamera) * WorldToCamera(lightCamera);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lightViewProjectionMatrix[i][j] = light[i][j];
		}
	}
	lightViewProjectionMatrix = glm::transpose(lightViewProjectionMatrix);

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
	glDeleteProgram(opacityShader);
	glDeleteProgram(phongShading);
	glDeleteProgram(normalDisplayProgramID);
	glDeleteProgram(fboCheckShader);
	glDeleteProgram(hybridFirstPass);
	glDeleteProgram(hybridTextureFirstPass);
	glDeleteProgram(hybridPhong);
	glDeleteProgram(hybridLocalLights);
	glDeleteProgram(shadowPass);
	glDeleteProgram(skydomeShader);

	glDeleteProgram(skyboxRecorderShader);
	glDeleteProgram(recorderShader);


	MyImGUI::ClearImGUI();

	textureManager.Clear();
	frameBuffer.Clear();

	delete horizontalFilter;
	delete verticalFilter;
	delete blurFilterShader;
	delete projectionInputImageCalculator00;
	delete projectionInputImageCalculator10;
	delete projectionInputImageCalculator11;
	delete projectionInputImageCalculator1_1;
	delete projectionInputImageCalculator20;
	delete projectionInputImageCalculator21;
	delete projectionInputImageCalculator2_1;
	delete projectionInputImageCalculator22;
	delete projectionInputImageCalculator2_2;
	delete irr;

	delete sphereMesh;
	delete centralMesh;
	delete normalMesh;
	delete faceNormalMesh;
	delete mainObjMesh;
	delete spheres;
	delete orbitMesh;
	delete floorMesh;
	delete floorObjMesh;

	delete cubeMesh;
	delete cubeObjMesh;

	delete myReader;

	delete assimpHybridFirstPass;
	delete assimpShadowPass;
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
		"../Common/ppms/MonValley_Lookout/MonValley_A_LookoutPoint_2k.hdr",
		"skydomeImage",
		Texture::TextureType::HDR
	);
	textureManager.AddTexture(irradianceMapWidth, irradianceMapHeight, "customIrr");
	glm::ivec2 skySize = textureManager.GetTextureSize("skydomeImage");
	backgroundImageWidth = skySize.x;
	backgroundImageHeight = skySize.y;

	imageConverterFB.Initialize(&textureManager, "newSky", backgroundImageWidth, backgroundImageHeight);
	textureManager.AddTexture(backgroundImageWidth, backgroundImageHeight, "irradianceImageProjection");
	textureManager.AddTexture(backgroundImageWidth, backgroundImageHeight, "irradianceImageProjectionSpare");

	shadowFB.Initialize(&textureManager, "shadowBuffer", shadowBufferSize, shadowBufferSize);
	textureManager.AddTexture(shadowBufferSize, shadowBufferSize, "shadowSAT");
	textureManager.AddTexture(shadowBufferSize, shadowBufferSize, "shadowSATSpare");
	textureManager.AddTexture(shadowBufferSize, shadowBufferSize, "shadowBlurred");



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

	cubeObjMesh->SetShader(skydomeShader);
	cubeObjMesh->Init(cubeMesh->getVertexCount(), cubeMesh->getVertexBuffer(), cubeMesh->getVertexNormals(), cubeMesh->getVertexUVs(),
		cubeMesh->getIndexBufferSize(), cubeMesh->getIndexBuffer());

	// normal inits
	normalMesh->SetShader(normalDisplayProgramID);
	normalMesh->Init(model->GetVertexNormalCount(), model->GetVertexNormalsForDisplay());

	// Face Inits
	faceNormalMesh->SetShader(normalDisplayProgramID);
	faceNormalMesh->Init(model->GetFaceNormalCount(), model->GetFaceNormalsForDisplay());


	spheres->SetShader(programID);
	spheres->Init(sphereMesh->getVertexCount(), sphereMesh->getVertexBuffer(), sphereMesh->getVertexNormals(), sphereMesh->getVertexUVs(),
		sphereMesh->getIndexBufferSize(), sphereMesh->getIndexBuffer());


	sphereEnvironmentalMatrix.resize(sphereEnvironmentalSize);
	sphereEnvironmentalMatrix[0] = glm::translate(glm::vec3(-7.f, 1.f, 7.f)) * glm::scale(glm::vec3(1.f));
	sphereEnvironmentalMatrix[1] = glm::translate(glm::vec3(-4.f, 0.5f, 3.f)) * glm::scale(glm::vec3(0.5f));
	sphereEnvironmentalMatrix[2] = glm::translate(glm::vec3(-5.f, 2.f, -0.5f)) * glm::scale(glm::vec3(2.f));
	sphereEnvironmentalMatrix[3] = glm::translate(glm::vec3(3.2f, 0.25f, 0.f)) * glm::scale(glm::vec3(0.25f));
	sphereEnvironmentalMatrix[4] = glm::translate(glm::vec3(3.8f, 1.8f, 5.5f)) * glm::scale(glm::vec3(1.6f));
	sphereEnvironmentalMatrix[5] = glm::translate(glm::vec3(4.5f, 1.f, -4.8f)) * glm::scale(glm::vec3(1.f));
	sphereEnvironmentalMatrix[6] = glm::translate(glm::vec3(2.f, 0.85f, 2.5f)) * glm::scale(glm::vec3(0.85f));
	sphereEnvironmentalMatrix[7] = glm::translate(glm::vec3(-2.f, 1.2f, 8.5f)) * glm::scale(glm::vec3(1.2f));
	sphereEnvironmentalMatrix[8] = glm::translate(glm::vec3(-6.f, 2.f, -7.f)) * glm::scale(glm::vec3(2.f));
	sphereEnvironmentalMatrix[9] = glm::translate(glm::vec3(0.f, 0.9f, -2.5f)) * glm::scale(glm::vec3(0.9f));

	sphereDiffuseColor.resize(sphereEnvironmentalSize);
	sphereDiffuseColor[0] = glm::vec3(1.f, 0.f, 0.f);
	sphereDiffuseColor[1] = glm::vec3(1.f, 0.f, 0.f);
	sphereDiffuseColor[2] = glm::vec3(0.f, 1.f, 0.f);
	sphereDiffuseColor[3] = glm::vec3(0.f, 1.f, 0.f);
	sphereDiffuseColor[4] = glm::vec3(0.f, 0.f, 1.f);
	sphereDiffuseColor[5] = glm::vec3(0.f, 0.f, 1.f);
	sphereDiffuseColor[6] = glm::vec3(1.f, 1.f, 0.f);
	sphereDiffuseColor[7] = glm::vec3(1.f, 1.f, 0.f);
	sphereDiffuseColor[8] = glm::vec3(1.f, 1.f, 1.f);
	sphereDiffuseColor[9] = glm::vec3(1.f, 1.f, 1.f);

	sphereSpecularColor.resize(sphereEnvironmentalSize);
	sphereSpecularColor[0] = glm::vec3(1.f, 0.f, 0.f);
	sphereSpecularColor[1] = glm::vec3(1.f, 0.f, 0.f);
	sphereSpecularColor[2] = glm::vec3(0.f, 1.f, 0.f);
	sphereSpecularColor[3] = glm::vec3(0.f, 1.f, 0.f);
	sphereSpecularColor[4] = glm::vec3(0.f, 0.f, 1.f);
	sphereSpecularColor[5] = glm::vec3(0.f, 0.f, 1.f);
	sphereSpecularColor[6] = glm::vec3(1.f, 1.f, 0.f);
	sphereSpecularColor[7] = glm::vec3(1.f, 1.f, 0.f);
	sphereSpecularColor[8] = glm::vec3(1.f, 1.f, 1.f);
	sphereSpecularColor[9] = glm::vec3(1.f, 1.f, 1.f);

	sphereRoughness.resize(sphereEnvironmentalSize);
	sphereRoughness[0] = 1.f;
	sphereRoughness[1] = 0.8f;
	sphereRoughness[2] = 0.4f;
	sphereRoughness[3] = 0.3f;
	sphereRoughness[4] = 0.2f;
	sphereRoughness[5] = 0.1f;
	sphereRoughness[6] = 0.05f;
	sphereRoughness[7] = 0.01f;
	sphereRoughness[8] = 0.005f;
	sphereRoughness[9] = 0.001f;
}

void Scene1::AddMembersToGUI()
{
	MyImGUI::SetNormalDisplayReferences(&vertexNormalFlag, &faceNormalFlag);
	MyImGUI::SetMaterialReferences(&ambient.x, &diffuse.x, &specular.x, reinterpret_cast<float*>(&intensityEmissive));
	MyImGUI::SetLightReferences(&lightManager);
	MyImGUI::SetEnvironmentReferences(reinterpret_cast<float*>(&intensityFog), reinterpret_cast<float*>(&attenuationConstants));
	MyImGUI::SetShaderReferences(&currentShader, &reloadShader);
	MyImGUI::SetCentralMesh(centralMesh, mainObjMesh, &shouldReload, buf, &flip, &uvImportType, &calculateUVonCPU);
	MyImGUI::SetHybridDebugging(&gbufferRenderTargetFlag, &depthWriteFlag, &isDrawDebugObjects, &localLightFlag, &debugObjectsOpacity);
	MyImGUI::SetShadowReferences(&blurStrength, &bias, &nearDepth, &farDepth);
	MyImGUI::SetBRDFReferences(&exposure, &contrast, &h.hammersley[1], &h.hammersley[2], &roughness, &useIrradianceMap);
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
	if (SATToggle)
	{
		textureManager.ActivateTexture(floorObjMesh->GetShader(), "shadowSATSpare", "shadowBufferSAT");
	}
	else
	{
		textureManager.ActivateTexture(floorObjMesh->GetShader(), "shadowSAT", "shadowBufferSAT");
	}
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "shadowBuffer", "shadowBufferMap");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "SkyCubeMap");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "customIrr", "irradianceImageProjection");
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "skydomeImage", "equirectangularMap");

	glm::mat4 diffuseObjToWorld = glm::translate(glm::vec3(-1.f, -1.f, 0.f)) * glm::scale(glm::vec3(2.f));
	floorObjMesh->SendUniformFloatMatrix4("objToWorld", &diffuseObjToWorld[0][0]);
	glm::mat4 shadowMatrix = glm::translate(glm::vec3(1.f / 2.f)) * glm::scale(glm::vec3(1.f / 2.f)) * lightViewProjectionMatrix;
	floorObjMesh->SendUniformFloatMatrix4("objToShadow", &shadowMatrix[0][0]);

	Point c = camera.Eye();

	floorObjMesh->SendUniformFloat3("cameraPosition", &c.x);

	floorObjMesh->SendUniformFloat3("ambient", &ambient.x);
	floorObjMesh->SendUniformFloat("ns", ns);
	floorObjMesh->SendUniformFloat("zNear", nearDistance);
	floorObjMesh->SendUniformFloat("zFar", farDistance);
	floorObjMesh->SendUniformFloat3("intensityEmissive", &intensityEmissive.x);
	floorObjMesh->SendUniformFloat3("intensityFog", &intensityFog.x);
	floorObjMesh->SendUniformFloat3("attenuationConstants", &attenuationConstants.x);
	floorObjMesh->SendUniformInt("UseIrradianceMap", useIrradianceMap);

	floorObjMesh->SendUniformFloat("nearDepth", nearDepth);
	floorObjMesh->SendUniformFloat("farDepth", farDepth);
	floorObjMesh->SendUniformFloat("bias", bias);
	floorObjMesh->SendUniformInt("blurStrength", blurStrength);
	floorObjMesh->SendUniformInt("shadowMapSize", shadowBufferSize);
	floorObjMesh->SendUniformBlockFloats(h.GetBlockName(), h.GetBlockDataSize(), h.GetData());
	floorObjMesh->SendUniformFloat("exposure", exposure);
	floorObjMesh->SendUniformFloat("contrast", contrast);

	glm::mat4 tmp1 = glm::rotate(glm::pi<float>() / 4, glm::vec3(0.f, 1.f, 0.f));
	floorObjMesh->SendUniformFloatMatrix4("tmp1", &tmp1[0][0]);
	glm::mat4 tmp2 = glm::rotate(glm::pi<float>() / 4, glm::vec3(1.f, 0.f, 0.f));
	floorObjMesh->SendUniformFloatMatrix4("tmp2", &tmp2[0][0]);


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
	spheres->SetShader(programID);
	spheres->PrepareDrawing();

	sphereMatrix = matrix * glm::scale(glm::vec3(0.1f));

	spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	spheres->SendUniformFloatMatrix4("objToWorld", &sphereMatrix[0][0]);
	spheres->SendUniformFloat3("diffuseColor", &diffuseColor[0]);
	spheres->Draw(sphereMesh->getIndexBufferSize());
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

	floorObjMesh->SetShader(hybridTextureFirstPass);
	floorObjMesh->PrepareDrawing();

	floorObjMesh->SendUniformFloatMatrix4("objToWorld", &floorMatrix[0][0]);
	floorObjMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	textureManager.ActivateTexture(hybridTextureFirstPass, "diffuseTexture");
	textureManager.ActivateTexture(hybridTextureFirstPass, "specularTexture");
	floorObjMesh->SendUniformFloat("roughness", roughness);

	floorObjMesh->Draw(floorMesh->getIndexBufferSize());

	////////////////////////////////////////////////////////////////////////////////////// Draw ends

	assimpHybridFirstPass->Use();
	assimpHybridFirstPass->SendUniformFloatMatrix4("objToWorld", &modelMatrix[0][0]);
	assimpHybridFirstPass->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	assimpHybridFirstPass->SendUniformFloat3("diffuse", &diffuse[0]);
	assimpHybridFirstPass->SendUniformFloat3("specular", &specular[0]);
	assimpHybridFirstPass->SendUniformFloat("roughness", roughness);
	model->Draw(hybridFirstPass);

	DrawEnvironmentalObjects();

	frameBuffer.RestoreDefaultFrameBuffer();
}

void Scene1::DrawLocalLightsPass()
{



	spheres->SetShader(hybridLocalLights);
	spheres->PrepareDrawing();



	textureManager.ActivateTexture(spheres->GetShader(), "diffuseBuffer");
	textureManager.ActivateTexture(spheres->GetShader(), "specularBuffer");
	textureManager.ActivateTexture(spheres->GetShader(), "positionBuffer");
	textureManager.ActivateTexture(spheres->GetShader(), "normalBuffer");
	textureManager.ActivateTexture(spheres->GetShader(), "customIrr", "irradianceMap");


	Point c = camera.Eye();
	spheres->SendUniformFloat3("cameraPosition", &c.x);

	// uniform block data
	// Uniform Block Update Starts Here
	spheres->SendUniformBlockVector3s("Block", lightManager.GetLightUniformDataSize(),
		lightManager.GetLightUniformBlockNames(), lightManager.GetLightUniformBlockData());

	sphereMatrix = glm::mat4(1.f);

	spheres->SendUniformInt("width", _windowWidth);
	spheres->SendUniformInt("height", _windowHeight);
	spheres->SendUniformFloat("ns", ns);
	spheres->SendUniformFloat3("attenuationConstants", &attenuationConstants.x);

	spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	spheres->SendUniformFloatMatrix4("objToWorld", &sphereMatrix[0][0]);
	int lightSize = *lightManager.GetCurrentLightSizeReference();
	spheres->DrawInstanced(sphereMesh->getIndexBufferSize(), lightSize);
}

void Scene1::DrawLocalLightsDebugInfo()
{
	const int lightSize = *lightManager.GetCurrentLightSizeReference();
	for (int i = 0; i < lightSize; i++)
	{
		Light& light = lightManager.GetLightReference(i);

		spheres->SetShader(opacityShader);
		spheres->PrepareDrawing();

		sphereMatrix = glm::translate(light.GetLightPosition()) * glm::scale(glm::vec3(10.f));
		spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
		spheres->SendUniformFloatMatrix4("objToWorld", &sphereMatrix[0][0]);
		spheres->SendUniformFloat3("diffuseColor", light.GetDiffusePointer());
		spheres->SendUniformFloat("opacity", debugObjectsOpacity);
		spheres->Draw(sphereMesh->getIndexBufferSize());

		// light.UpdateLightPosition(angleOfRotate);
		// light.SetLightDirection(glm::vec3(0.f) - light.GetLightPosition());
	}
}

void Scene1::SetupCamera()
{
	camera = Camera(eyePoint, targetPoint - eyePoint, relativeUp, fov, aspect, nearDistance, farDistance);
	glm::vec3* lightDirection = reinterpret_cast<glm::vec3*>(lightManager.GetDirectionalLightDirection());
	Point lightEye(-lightDirection->x * 20, -lightDirection->y * 20, -lightDirection->z * 20);
	Vector lightCameraDirection(lightDirection->x, lightDirection->y, lightDirection->z);
	lightCamera = Camera(lightEye, lightCameraDirection, relativeUp, fov, 1.f, nearDistance, farDistance);

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
	static const glm::vec3 initTranslations[LightManager::MAX_LIGHT] = {
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
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
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
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),

		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
		glm::vec3(0.f, 3.f, 0.f),
			/*glm::vec3(0.f, 3.f, 0.f),*/
	};
	static const glm::vec3 initRotationAxis[LightManager::MAX_LIGHT] = {
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
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
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
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),

		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 0.f, 1.f),
			/*glm::vec3(1.f, 0.f, 1.f),*/

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
		if (i >= 16)
		{
			spheres->SetShader(programID);
			spheres->PrepareDrawing();

			sphereMatrix = glm::translate(light.GetLightPosition()) * glm::scale(glm::vec3(0.1f));
			spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
			spheres->SendUniformFloatMatrix4("objToWorld", &sphereMatrix[0][0]);
			spheres->SendUniformFloat3("diffuseColor", light.GetDiffusePointer());
			spheres->Draw(sphereMesh->getIndexBufferSize());

		}
		else
		{
			DrawSpheresAndOrbit(light.GetLightTranslation(), light.GetLightRotationAxis(), *light.GetLightRotationScalerReference(), *light.GetInitLightRotationReference(), *reinterpret_cast<glm::vec3*>(light.GetDiffusePointer()), light.GetMatrix());
		}
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

	RenderSkydome();

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

	glCullFace(GL_FRONT);
	DrawShadowPass();
	glCullFace(GL_BACK);
	DispatchBlurFilter();

	Draw1stPass();



	// disable depthwriting for temporarily
	glDisable(GL_DEPTH_TEST);

	Draw2ndPass();


	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	if (localLightFlag)
	{
		DrawLocalLightsPass();
	}
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);

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
	if (localLightFlag == true)
	{
		UpdateLights();
	}

	if (isDrawDebugObjects == true)
	{
		glCullFace(GL_FRONT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawLocalLightsDebugInfo();
		glDisable(GL_BLEND);
	}

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

	floorObjMesh->SetShader(fboCheckShader);
	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloatMatrix4("trans", &gbufferRenderTargetsMatrix[3][0][0]);
	textureManager.ActivateTexture(fboCheckShader, "specularBuffer", "tex");
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());
}

void Scene1::DrawEnvironmentalObjects()
{
	spheres->SetShader(hybridFirstPass);

	for (size_t i = 0; i < sphereEnvironmentalSize; i++)
	{
		spheres->PrepareDrawing();
		spheres->SendUniformFloatMatrix4("lightViewProjectionMatrix", &lightViewProjectionMatrix[0][0]);
		spheres->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
		spheres->SendUniformFloatMatrix4("objToWorld", &sphereEnvironmentalMatrix[i][0][0]);
		spheres->SendUniformFloat3("diffuse", &sphereDiffuseColor[i][0]);
		spheres->SendUniformFloat3("specular", &sphereSpecularColor[i][0]);
		spheres->SendUniformFloat("roughness", sphereRoughness[i]);
		spheres->Draw(sphereMesh->getIndexBufferSize());
	}
}

void Scene1::DrawShadowPass()
{
	shadowFB.ApplyFBO();

	glm::mat4 localFloorMatrix = glm::translate(glm::vec3(0.f, -1.f, 0.f)) * glm::rotate(glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f)) * glm::scale(glm::vec3(12.f, 12.f, 1.f)) * floorMesh->calcAdjustBoundingBoxMatrix();
	floorObjMesh->SetShader(shadowPass);
	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloat("nearDepth", nearDepth);
	floorObjMesh->SendUniformFloat("farDepth", farDepth);
	floorObjMesh->SendUniformFloatMatrix4("objToWorld", &localFloorMatrix[0][0]);
	floorObjMesh->SendUniformFloatMatrix4("worldToNDC", &lightViewProjectionMatrix[0][0]);
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());


	assimpShadowPass->Use();
	assimpShadowPass->SendUniformFloat("nearDepth", nearDepth);
	assimpShadowPass->SendUniformFloat("farDepth", farDepth);
	assimpShadowPass->SendUniformFloatMatrix4("objToWorld", &modelMatrix[0][0]);
	assimpShadowPass->SendUniformFloatMatrix4("worldToNDC", &lightViewProjectionMatrix[0][0]);
	model->Draw(*assimpShadowPass);

	spheres->SetShader(shadowPass);

	for (size_t i = 0; i < sphereEnvironmentalSize; i++)
	{
		spheres->PrepareDrawing();
		spheres->SendUniformFloat("nearDepth", nearDepth);
		spheres->SendUniformFloat("farDepth", farDepth);
		spheres->SendUniformFloatMatrix4("worldToNDC", &lightViewProjectionMatrix[0][0]);
		spheres->SendUniformFloatMatrix4("objToWorld", &sphereEnvironmentalMatrix[i][0][0]);
		spheres->Draw(sphereMesh->getIndexBufferSize());
	}


	shadowFB.RestoreDefaultFrameBuffer();
}


void Scene1::DispatchBlurFilter()
{
	SATToggle = true;
	// Vertical pass
	verticalFilter->PrepareDrawing();
	textureManager.ActivateImage(verticalFilter->GetShader(), "shadowBuffer", "src", GL_READ_ONLY);
	textureManager.ActivateImage(verticalFilter->GetShader(), "shadowSAT", "dst", GL_WRITE_ONLY);
	int delta = 1;
	while (delta < shadowBufferSize)
	{
		verticalFilter->SendUniformInt("delta", delta);
		verticalFilter->Dispatch(shadowBufferSize / 16, shadowBufferSize / 16, 1);
		glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
		delta *= 2;
		ActivateAppropriateSATImage(verticalFilter->GetShader());
	}

	horizontalFilter->PrepareDrawing();
	SATToggle = !SATToggle;

	// Horizontal pass
	delta = 1;
	while (delta < shadowBufferSize)
	{
		horizontalFilter->SendUniformInt("delta", delta);
		ActivateAppropriateSATImage(horizontalFilter->GetShader());
		horizontalFilter->Dispatch(shadowBufferSize / 16, shadowBufferSize / 16, 1);
		delta *= 2;
	}


	blurFilterShader->PrepareDrawing();
	ActivateAppropriateSATImage(blurFilterShader->GetShader());
	textureManager.ActivateImage(blurFilterShader->GetShader(), "shadowBlurred", "dst", GL_WRITE_ONLY);
	blurFilterShader->SendUniformInt("blurStrength", blurStrength);
	blurFilterShader->Dispatch(shadowBufferSize / 16, shadowBufferSize / 16, 1);
}

void Scene1::ActivateAppropriateSATImage(GLuint shader)
{
	if (SATToggle)
	{
		textureManager.ActivateImage(shader, "shadowSAT", "src", GL_READ_ONLY);
		textureManager.ActivateImage(shader, "shadowSATSpare", "dst", GL_WRITE_ONLY);
	}
	else
	{
		textureManager.ActivateImage(shader, "shadowSATSpare", "src", GL_READ_ONLY);
		textureManager.ActivateImage(shader, "shadowSAT", "dst", GL_WRITE_ONLY);
	}
	SATToggle = !SATToggle;
}

void Scene1::RenderSkydome()
{
	cubeObjMesh->SetShader(skydomeShader);
	cubeObjMesh->PrepareDrawing();
	glm::mat4 mappingMatrix = glm::scale(glm::vec3(100.f)) * glm::translate(glm::vec3(-1.f));
	glm::mat4 objToWorld = glm::translate(glm::vec3(camera.Eye().x, camera.Eye().y, camera.Eye().z));
	cubeObjMesh->SendUniformFloatMatrix4("mappingMatrix", &mappingMatrix[0][0]);
	cubeObjMesh->SendUniformFloatMatrix4("objToWorld", &objToWorld[0][0]);
	cubeObjMesh->SendUniformFloatMatrix4("worldToNDC", &worldToNDC[0][0]);
	textureManager.ActivateTexture(cubeObjMesh->GetShader(), "SkyCubeMap");
	cubeObjMesh->SendUniformFloat("exposure", exposure);
	cubeObjMesh->SendUniformFloat("contrast", contrast);
	cubeObjMesh->Draw(cubeMesh->getIndexBufferSize());
}

void Scene1::RecordSkybox()
{


	glm::mat4 mappingMatrix = glm::scale(glm::vec3(100.f)) * glm::translate(glm::vec3(-1.f));
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};


	for (size_t i = 0; i < 6; i++)
	{
		skyboxFB.ApplyCubeFBO("SkyCubeMap", static_cast<int>(i));


		cubeObjMesh->SetShader(skyboxRecorderShader);
		cubeObjMesh->PrepareDrawing();
		cubeObjMesh->SendUniformFloatMatrix4("view", &captureViews[i][0][0]);
		cubeObjMesh->SendUniformFloatMatrix4("mapping", &mappingMatrix[0][0]);
		cubeObjMesh->SendUniformFloatMatrix4("projection", &captureProjection[0][0]);
		textureManager.ActivateTexture(cubeObjMesh->GetShader(), "skydomeImage");
		cubeObjMesh->Draw(cubeMesh->getIndexBufferSize());
	}

	textureManager.ActivateTexture(cubeObjMesh->GetShader(), "SkyCubeMap");
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 10);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	skyboxFB.RestoreDefaultFrameBuffer();
}

void Scene1::DispatchIrradianceMapMaker()
{
	imageProjections.resize(9);
	imageProjections[0] = CalculateImageProjection(projectionInputImageCalculator00, "dst00");
	imageProjections[1] = CalculateImageProjection(projectionInputImageCalculator1_1, "dst1_1");
	imageProjections[2] = CalculateImageProjection(projectionInputImageCalculator10, "dst10");
	imageProjections[3] = CalculateImageProjection(projectionInputImageCalculator11, "dst11");
	imageProjections[4] = CalculateImageProjection(projectionInputImageCalculator2_2, "dst2_2");
	imageProjections[5] = CalculateImageProjection(projectionInputImageCalculator2_1, "dst2_1");
	imageProjections[6] = CalculateImageProjection(projectionInputImageCalculator20, "dst20");
	imageProjections[7] = CalculateImageProjection(projectionInputImageCalculator21, "dst21");
	imageProjections[8] = CalculateImageProjection(projectionInputImageCalculator22, "dst22");


	irr->PrepareDrawing();
	textureManager.ActivateImage(irr->GetShader(), "customIrr", "dst", GL_WRITE_ONLY);
	GLint loc0 = glGetUniformLocation(irr->GetShader(), "e00");
	glUniform4fv(loc0, 1, &imageProjections[0].x);
	GLint loc2 = glGetUniformLocation(irr->GetShader(), "e1_1");
	glUniform4fv(loc2, 1, &imageProjections[1].x);
	GLint loc1 = glGetUniformLocation(irr->GetShader(), "e10");
	glUniform4fv(loc1, 1, &imageProjections[2].x);
	GLint loc3 = glGetUniformLocation(irr->GetShader(), "e11");
	glUniform4fv(loc3, 1, &imageProjections[3].x);
	GLint loc8 = glGetUniformLocation(irr->GetShader(), "e2_2");
	glUniform4fv(loc8, 1, &imageProjections[4].x);
	GLint loc7 = glGetUniformLocation(irr->GetShader(), "e2_1");
	glUniform4fv(loc7, 1, &imageProjections[5].x);
	GLint loc4 = glGetUniformLocation(irr->GetShader(), "e20");
	glUniform4fv(loc4, 1, &imageProjections[6].x);
	GLint loc5 = glGetUniformLocation(irr->GetShader(), "e21");
	glUniform4fv(loc5, 1, &imageProjections[7].x);
	GLint loc6 = glGetUniformLocation(irr->GetShader(), "e22");
	glUniform4fv(loc6, 1, &imageProjections[8].x);
	irr->SendUniformInt("imageWidth", irradianceMapWidth);
	irr->SendUniformInt("imageHeight", irradianceMapHeight);
	irr->Dispatch(irradianceMapWidth / 16, irradianceMapHeight / 16, 1);
}

glm::vec4 Scene1::CalculateImageProjection(ComputeShaderDispatcher* computeShader, const char* activatedDestinationTextureName)
{
	computeShader->PrepareDrawing();
	// Activate image for src, and dst
	textureManager.ActivateImage(computeShader->GetShader(), "newSky", "src", GL_READ_ONLY);
	textureManager.ActivateImage(computeShader->GetShader(), "irradianceImageProjection", activatedDestinationTextureName, GL_WRITE_ONLY);
	computeShader->SendUniformInt("imageWidth", backgroundImageWidth);
	computeShader->SendUniformInt("imageHeight", backgroundImageHeight);
	computeShader->Dispatch(backgroundImageWidth / 16, backgroundImageHeight / 16, 1);

	SumIrradianceImageProjection("irradianceImageProjection");

	return textureManager.ReadPixelData("irradianceImageProjection", backgroundImageWidth - 1, backgroundImageHeight - 1);
}

void Scene1::ActivateAppropriateIrradianceImage(GLuint shader, const char* textureName)
{
	if (irradianceToggle)
	{
		textureManager.ActivateImage(shader, textureName, "src", GL_READ_ONLY);
		textureManager.ActivateImage(shader, (textureName + std::string("Spare")).c_str(), "dst", GL_WRITE_ONLY);
	}
	else
	{
		textureManager.ActivateImage(shader, (textureName + std::string("Spare")).c_str(), "src", GL_READ_ONLY);
		textureManager.ActivateImage(shader, textureName, "dst", GL_WRITE_ONLY);
	}
	irradianceToggle = !irradianceToggle;
}

void Scene1::SumIrradianceImageProjection(const char* textureName)
{
	// Sum them up all.
	irradianceToggle = true;
	// Vertical pass
	verticalFilter->PrepareDrawing();
	int delta = 1;
	while (delta < backgroundImageWidth)
	{
		verticalFilter->SendUniformInt("delta", delta);
		ActivateAppropriateIrradianceImage(verticalFilter->GetShader(), textureName);
		verticalFilter->Dispatch(backgroundImageWidth / 16, backgroundImageHeight / 16, 1);
		glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT);
		delta *= 2;
	}

	horizontalFilter->PrepareDrawing();

	// Horizontal pass
	delta = 1;
	while (delta < backgroundImageWidth)
	{
		horizontalFilter->SendUniformInt("delta", delta);
		ActivateAppropriateIrradianceImage(horizontalFilter->GetShader(), textureName);
		horizontalFilter->Dispatch(backgroundImageWidth / 16, backgroundImageHeight / 16, 1);
		delta *= 2;
	}
}

void Scene1::RecordSkyImage()
{

	glm::ivec2 skySize = textureManager.GetTextureSize("skydomeImage");
	//imageConverterFB.Initialize(&textureManager, "newSky", skySize.x, skySize.y);
	glViewport(0, 0, skySize.x, skySize.y);
	glm::mat4 mappingMatrix = glm::translate(glm::vec3(-1.f, -1.f, -1.f)) * glm::scale(glm::vec3(2.f));
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
	glm::mat4 captureView =
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	imageConverterFB.ApplyFBO();
	floorObjMesh->SetShader(recorderShader);
	floorObjMesh->PrepareDrawing();
	floorObjMesh->SendUniformFloatMatrix4("view", &captureView[0][0]);
	floorObjMesh->SendUniformFloatMatrix4("mapping", &mappingMatrix[0][0]);
	floorObjMesh->SendUniformFloatMatrix4("projection", &captureProjection[0][0]);
	textureManager.ActivateTexture(floorObjMesh->GetShader(), "skydomeImage", "image");
	floorObjMesh->Draw(floorMesh->getIndexBufferSize());

	imageConverterFB.RestoreDefaultFrameBuffer();
	glViewport(0, 0, _windowWidth, _windowHeight);
}
