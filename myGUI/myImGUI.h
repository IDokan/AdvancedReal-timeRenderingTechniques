/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: myImGUI.cpp
Purpose: myGUI implementation!
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 10/20/2021
End Header --------------------------------------------------------*/
#include <../Common/Meshes/Mesh.h>
class LightManager;
class Mesh;
class ObjectMesh;

namespace MyImGUI
{

	void InitImGUI(GLFWwindow* window);
	void UpdateImGUI();
	void ClearImGUI();

    void SetNormalDisplayReferences(bool* vertexNormalFlag, bool* faceNormalFlag);
    void SetMaterialReferences(float* ambient, float* diffuse, float* specular, float* intensityEmissive);
    void SetLightReferences(LightManager* lightManager);
    void SetEnvironmentReferences(float* intensityFog, float* attenuationConstants);
    void SetShaderReferences(int* currentShader, bool* reloadShader);
    void SetCentralMesh(Mesh* mesh, ObjectMesh* mainObjMesh, bool* shouldReload, char* buf, bool* flip, Mesh::UVType* uvType, bool* calculateUVonCPU);
    void SetHybridDebugging(bool* ptrToGBufferRenderTargetFlag, bool* ptrToDepthWriteFlag, bool* ptrToisDrawDebugObjects, bool* ptrToLocalLightFlag, float* ptrToOpacity);
    void SetShadowReferences(int* _blurStrength, float* _bias, float* _nearDepth, float* _farDepth);
    void SetBRDFReferences(float* _exposure, float* _contrast, float* u, float* v, float* roughness, bool* useIrradianceMap);
    void SetAOReferences(int* _sampledPointsForAO, float* _influenceRangeForAO, float* _aoScaler, float* _aoContrast, int* _aoBlurWidth, float* _aoVariance, bool* _aoBlurFlag);
    void SetNoiseReferences(double* _frequency, int* octaves, bool* generateNoise, float* scaler);
    void SetTextureReferences(float* _earlyColor, float* _lateColor, bool* generateWood, bool* generateWood2);
}
