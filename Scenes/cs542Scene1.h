/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: as1Scene.h
Purpose: main scene header code of this assignment
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/18/2021
End Header --------------------------------------------------------*/

#ifndef CS542_SCENE1_H
#define CS542_SCENE1_H

#include <../Common/Scene.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <../Common/Cameras/Camera.h>
#include <../Common/Lights/LightManager.h>
#include <../Common/Textures/TextureManager.h>
#include <../Common/Meshes/Mesh.h>

#include <../Common/FBO/FBO.h>

#include <../Common/Structs.h>

class Mesh;
class LineMesh;
class ObjectMesh;
class Model;
class AssimpShader;
class ComputeShaderDispatcher;

class Scene1 : public Scene
{
public:
	Scene1() = default;
	Scene1(int width, int height);
	virtual ~Scene1();

public:
	int Init();
	void LoadAllShaders();
	int preRender();
	int Render();
	int postRender();
	void CleanUp();
	void SetupNanoGUI(GLFWwindow* window);
	void UpdateGUI();

private:
	void InitGraphics();

	void AddMembersToGUI();

	// Directional Light(Global Light) Pass
	void Draw2ndPass();
	void DrawVertexNormals();
	void DrawFaceNormals();
	void DrawSpheresAndOrbit(glm::vec3 position, glm::vec3 rotateAxis, float rotateScale = 1.f, float InitRotate = 0.f, glm::vec3 diffuseColor = glm::vec3(0.f, 0.f, 0.f), glm::mat4 matrix = glm::mat4());
	// G-buffer pass
	void Draw1stPass();

	// 3rd Pass
	void DrawLocalLightsPass();

	void SetupCamera();
	void MoveViewOfCamera(int x, int y);
	void MovePositionOfCamera(float speed, short moveDirection);
	void UpdateCamera();

	void UpdateGUIShader();

	void InitLights();
	void UpdateLights();

	void ReloadShader();
	void ReloadMesh();

	void HybridRendering();
	void RenderDeferredObjects();
	void CopyDepthInfo();
	void RenderDebugObjects();
	bool isDrawDebugObjects = false;

	void DrawGBufferRenderTargets();

	void DrawEnvironmentalObjects();
	int sphereEnvironmentalSize;
	std::vector<glm::mat4> sphereEnvironmentalMatrix;
	std::vector<glm::vec3> sphereDiffuseColor;
	std::vector<glm::vec3> sphereSpecularColor;
	std::vector<float> sphereRoughness;

private:
	const GLuint vertexAttribute;
	const GLuint normalAttribute;
	const int numOfFloatVertex;
private:
	Mesh* sphereMesh;
	Mesh* centralMesh;
	Mesh* orbitMesh;
	Mesh* floorMesh;

	Model* model;

	// Shaders
	GLuint programID;
	GLuint phongShading;
	ObjectMesh* mainObjMesh;
	ObjectMesh* spheres;
	ObjectMesh* floorObjMesh;
	// Normal Drawing
	GLuint normalDisplayProgramID;
	LineMesh* normalMesh;
	// Face Normals
	LineMesh* faceNormalMesh;
	LineMesh* sphereOrbit;

	// Hybrid rendering
	GLuint fboCheckShader;
	GLuint hybridFirstPass;
	GLuint hybridTextureFirstPass;
	AssimpShader* assimpHybridFirstPass;
	GLuint hybridPhong;

	GLuint hybridLocalLights;


	float angleOfRotate;
	glm::mat4 centralMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 sphereMatrix;
	glm::mat4 floorMatrix;
	glm::mat4 gbufferRenderTargetsMatrix[4];

	// GUI members
	GLFWwindow* displayWindow;
	GLFWwindow* guiWindow;

	bool vertexNormalFlag;
	bool faceNormalFlag;
	bool gbufferRenderTargetFlag;
	bool depthWriteFlag;

	glm::vec2 handlerDisplacement;

	Camera camera;
	glm::mat4 worldToNDC;
	Point sphericalViewPoint;
	float oldX;
	float oldY;
	Vector cartesianViewVector;
	float cameraMovementOffset;
	Vector cartesianRightVector;
	Vector cartesianUpVector;
	Point eyePoint;
	Point targetPoint;
	float fov;
	float aspect;
	float nearDistance;
	float farDistance;
	Vector relativeUp;
	enum
	{
		FORWARD = 0b1,
		BACKWARD = 0b10,
		RIGHTWARD = 0b100,
		LEFTWARD = 0b1000,
		UPWARD = 0b10000,
		DOWNWARD = 0b100000,
	};

	// material variables
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float ns;
	glm::vec3 intensityEmissive;
	// light variables
	LightManager lightManager;
	// environment variables
	glm::vec3 intensityFog;
	glm::vec3 attenuationConstants;

	TextureManager textureManager;


	// 0 - Phong Lighting
	// 1 - Phong Shading
	// 2 - Blinn Shading
	int currentShader;

	bool shouldReload;
	char buf[64];
	bool flip;
	Mesh::UVType uvImportType;
	bool calculateUVonCPU;

	bool reloadShader;

	MyObjReader* myReader;

	FBO frameBuffer;

	void DrawShadowPass();
	FBO shadowFB;
	Camera lightCamera;
	glm::mat4 lightViewProjectionMatrix;
	GLuint shadowPass;
	AssimpShader* assimpShadowPass;

	void DispatchBlurFilter();
	ComputeShaderDispatcher* horizontalFilter;
	ComputeShaderDispatcher* verticalFilter;
	const int shadowBufferSize;
	int blurStrength;
	ComputeShaderDispatcher* blurFilterShader;
	void ActivateAppropriateSATImage(GLuint shader);
	bool SATToggle = true;

	float nearDepth = 15.f;
	float farDepth = 40.f;
	float bias;

	char cubePath[64];
	GLuint skydomeShader;
	void RenderSkydome();
	Mesh* cubeMesh;
	ObjectMesh* cubeObjMesh;

	struct Hammersley h;

	float exposure;
	float contrast;

	FBO skyboxFB;
	void RecordSkybox();
	GLuint skyboxRecorderShader;
	float roughness;

	ComputeShaderDispatcher* projectionInputImageCalculator00;
	ComputeShaderDispatcher* projectionInputImageCalculator10;
	ComputeShaderDispatcher* projectionInputImageCalculator11;
	ComputeShaderDispatcher* projectionInputImageCalculator1_1;
	ComputeShaderDispatcher* projectionInputImageCalculator20;
	ComputeShaderDispatcher* projectionInputImageCalculator21;
	ComputeShaderDispatcher* projectionInputImageCalculator2_1;
	ComputeShaderDispatcher* projectionInputImageCalculator22;
	ComputeShaderDispatcher* projectionInputImageCalculator2_2;
	void DispatchIrradianceMapMaker();
	int backgroundImageWidth;
	int backgroundImageHeight;
	bool irradianceToggle = true;
	glm::vec4 CalculateImageProjection(ComputeShaderDispatcher* computeShader, const char* activatedDestinationTextureName);
	void ActivateAppropriateIrradianceImage(GLuint shader, const char* textureName);
	void SumIrradianceImageProjection(const char* textureName);
	FBO imageConverterFB;
	void RecordSkyImage();
	GLuint recorderShader;
	std::vector<glm::vec4> imageProjections;
	bool useIrradianceMap = false;
	ComputeShaderDispatcher* irr;
	int irradianceMapWidth;
	int irradianceMapHeight;
};

#endif // CS542_SCENE1_H