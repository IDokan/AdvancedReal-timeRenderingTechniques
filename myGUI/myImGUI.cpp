/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: myImGUI.cpp
Purpose: myImGUI implementation!
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 10/20/2021
End Header --------------------------------------------------------*/

#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../myGUI/imgui.h>
#include <../myGUI/imgui_impl_opengl3.h>
#include <../myGUI/imgui_impl_glfw.h>

#include <../myGUI/myImGUI.h>
#include <../Common/Lights/LightManager.h>

#include <string>
#include <../Common/Meshes/MyObjReader.h>
#include <../Common/Meshes/Mesh.h>
#include <../Common/Meshes/MyMeshes/ObjectMesh.h>
#include <glm/gtx/transform.hpp>

namespace MyImGUI {
    bool* vertexNormalFlag;
    bool* faceNormalFlag;

    float* ambient;
    float* diffuse;
    float* specular;
    float* intensityEmissive;
    float* intensityFog;
    float* attenuationConstants;

    int* currentShader;

    int* lightSize;
    glm::vec3* lightProperties[LightManager::lightUniformDataSize];
    LightManager* lightm;

    Mesh* centralMesh;
    ObjectMesh* mainObjMesh;
    MyObjReader* myReader;
    bool* shouldReload;
    char* buf;
    bool* flip;
    Mesh::UVType* uv;
    bool* calculateUVonCPU;

    bool* reloadShader;

    int currentUVType = 2;

    bool* gBufferRenderTargetFlag;
    bool* depthWriteFlag;
    bool* isDrawDebugObjects;

    float* lightDepthOffset;
    int* blurStrength;

    namespace Helper
    {
        void Normals();

        void LightScenarios();
        void LightScene1();
        void LightScene2();
        void LightScene3();

        void Lights();

        // Lights
        void MaterialSetup();
        void LightSetup();
        void EnvironmentSetup();
        void LightRepresenter(std::string identifier, int index);
        int GetCurrentLightType(int index);
        void SetCurrentLightType(int index, int t);

        void Shaders();

        // Shaders
        void ShaderList();

        // Models
        void Models();

        void HybridRenders();

        void Shadows();


        // Helper to display a little (?) mark which shows a tooltip when hovered.
        // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
        void HelpMarker(const char* desc);
    }
}



void MyImGUI::Helper::HelpMarker(const char* desc)
{
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


void MyImGUI::InitImGUI(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    myReader = new MyObjReader();
}

void MyImGUI::UpdateImGUI()
{
    glfwPollEvents();
    glClearColor(0.45f, 0.55f, 0.6f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Demo window");

    Helper::Normals();
    Helper::Models();
    Helper::Lights();
    Helper::LightScenarios();
    Helper::HybridRenders();
    Helper::Shadows();

    // Shader selection is not disabled
    //Helper::Shaders();



    ImGui::End();
    //ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyImGUI::ClearImGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete myReader;
}

void MyImGUI::SetNormalDisplayReferences(bool* _vertexNormalFlag, bool* _faceNormalFlag)
{
    vertexNormalFlag = _vertexNormalFlag;
    faceNormalFlag = _faceNormalFlag;
}

void MyImGUI::SetMaterialReferences(float* _ambient, float* _diffuse, float* _specular, float* _intensityEmissive)
{
    ambient = _ambient;
    diffuse = _diffuse;
    specular = _specular;
    intensityEmissive = _intensityEmissive;
}

void MyImGUI::SetLightReferences(LightManager* lightManager)
{
    lightSize = lightManager->GetCurrentLightSizeReference();
    lightm = lightManager;
    const int ps = LightManager::propertySize;
    for (int i = 0; i < LightManager::MAX_LIGHT; i++)
    {
        lightProperties[i * ps + 0] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetPositionPointer());
        lightProperties[i * ps + 1] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetAmbientPointer());
        lightProperties[i * ps + 2] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetDiffusePointer());
        lightProperties[i * ps + 3] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetSpecularPointer());
        lightProperties[i * ps + 4] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetTypePointer());
        lightProperties[i * ps + 5] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetDirectionPointer());
        lightProperties[i * ps + 6] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetAnglesPointer());
        lightProperties[i * ps + 7] = reinterpret_cast<glm::vec3*>(lightManager->GetLightReference(i).GetSpotFalloff());
    }

}

void MyImGUI::SetEnvironmentReferences(float* _intensityFog, float* _attenuationConstants )
{
    intensityFog = _intensityFog;
    attenuationConstants = _attenuationConstants;
}

void MyImGUI::SetCentralMesh(Mesh* mesh, ObjectMesh* _mainObjMesh, bool* _shouldReload, char* _buf, bool* _flip, Mesh::UVType* uvType, bool* _calculateUVonCPU)
{
    centralMesh = mesh;
    mainObjMesh = _mainObjMesh;
    shouldReload = _shouldReload;
    buf = _buf;
    flip = _flip;
    uv = uvType;
    calculateUVonCPU = _calculateUVonCPU;
}

void MyImGUI::SetHybridDebugging(bool* ptrToGBufferRenderTargetFlag, bool* ptrToDepthWriteFlag, bool* ptrToisDrawDebugObjects)
{
    gBufferRenderTargetFlag = ptrToGBufferRenderTargetFlag;
    depthWriteFlag = ptrToDepthWriteFlag;
    isDrawDebugObjects = ptrToisDrawDebugObjects;
}

void MyImGUI::SetShadowReferences(float* _lightDepthOffset, int* _blurStrength)
{
    lightDepthOffset = _lightDepthOffset;
    blurStrength = _blurStrength;
}

void MyImGUI::Helper::MaterialSetup()
{
    ImGui::ColorEdit3("material ambient", ambient);
    ImGui::ColorEdit3("intensityEmissive", intensityEmissive);
}
void MyImGUI::Helper::LightSetup()
{
    ImGui::ColorEdit3("Directional Light Intensity", lightm->GetDirectionalLightIntensity());
    ImGui::SliderFloat3("Directional Light Direction", lightm->GetDirectionalLightDirection(), -1.f, 1.f);
    ImGui::Separator();

    ImGui::InputInt("input int", lightSize);
    if (*lightSize <= 0)
    {
        *lightSize = 0;
    }
    else if (*lightSize > LightManager::MAX_LIGHT)
    {
        *lightSize = LightManager::MAX_LIGHT;
    }

    ImGui::Separator();

    for (int i = 0; i < *lightSize; i++)
    {

        char tmp[10] = ".";
        _itoa_s(i, tmp, 10);
        LightRepresenter(std::string("Light ") + std::string(tmp) + std::string(": "), i);
        ImGui::Separator();
    }
    int ps = LightManager::propertySize;
}
void MyImGUI::Helper::EnvironmentSetup()
{
    ImGui::ColorEdit3("light fog", intensityFog);
    ImGui::SliderFloat3("attenuation constants", attenuationConstants, 0.f, 1.f);
}

void MyImGUI::Helper::Lights()
{
    if (ImGui::CollapsingHeader("Lights"))
    {
        Helper::MaterialSetup();
        Helper::EnvironmentSetup();
        ImGui::Separator();
        Helper::LightSetup();
    }
}

void MyImGUI::Helper::Shaders()
{
    if (ImGui::CollapsingHeader("Shaders"))
    {
        ShaderList();
    }
}

void MyImGUI::Helper::ShaderList()
{
    const char* items[] = { "Phong Lighting", "Phong Shading", "Blinn Shading" };
    ImGui::Combo("Select Shader", currentShader, items, IM_ARRAYSIZE(items));
    if (ImGui::Button("Reload Shaders"))
    {
        *reloadShader = true;
    }
}
void MyImGUI::SetShaderReferences(int* _currentShader, bool* _reloadShader)
{
    currentShader = _currentShader;
    reloadShader = _reloadShader;
}


void MyImGUI::Helper::LightRepresenter(std::string identifier, int index)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(identifier.c_str());
    ImGui::SameLine();

    static int type = 0;
    type = MyImGUI::Helper::GetCurrentLightType(index);

    const char* lightTypes[] = { "Point", "Spot" };
    ImGui::Combo(std::string(std::string("LightType") + identifier).c_str(), &type, lightTypes, IM_ARRAYSIZE(lightTypes));
    static bool stopLight = false;
    stopLight = lightm->GetLightReference(index).GetShouldStopLight();
    if (ImGui::Checkbox(std::string(std::string("Stop light") + identifier).c_str(), &stopLight))
    {
        lightm->GetLightReference(index).SetShouldStopLight(stopLight);
    }

    ImGui::SliderFloat3(std::string(std::string("Light Position") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 0]), -10.f, 10.f);
    ImGui::SliderFloat(std::string(std::string("Light Speed ") + identifier).c_str(), lightm->GetLightReference(index).GetLightRotationScalerReference(), 0.f, 6.28f);
    ImGui::SliderFloat(std::string(std::string("Starting Light Point") + identifier).c_str(), lightm->GetLightReference(index).GetInitLightRotationReference(), 0.f, 6.28f);
    ImGui::ColorEdit3(std::string(std::string("Ambient Intensity") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 1]));
    ImGui::ColorEdit3(std::string(std::string("Diffuse Intensity") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 2]));
    ImGui::ColorEdit3(std::string(std::string("Specular Intensity") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 3]));
    ImGui::SliderFloat2(std::string(std::string("Spotlight Angles") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 6]), 0.f, 6.28f);
    ImGui::SliderFloat(std::string(std::string("Spotlight Falloff value") + identifier).c_str(), reinterpret_cast<float*>(lightProperties[index * LightManager::propertySize + 7]), 0.f, 10.f);

    MyImGUI::Helper::SetCurrentLightType(index, type);
}


int MyImGUI::Helper::GetCurrentLightType(int index)
{
    int type = 0;
    float lightTypeFloat = lightProperties[index * LightManager::propertySize + 4]->x;
    if (lightTypeFloat >= -5.f && lightTypeFloat <= 5.f)
    {
        type = 0;
    }
    else if (lightTypeFloat >= 5.f && lightTypeFloat <= 15.f)
    {
        type = 1;
    }
    else if (lightTypeFloat >= 95.f && lightTypeFloat <= 105.f)
    {
        type = 2;
    }

    return type;
}
void MyImGUI::Helper::SetCurrentLightType(int index, int t)
{
    switch (t)
    {
    case 0:
        lightProperties[index * LightManager::propertySize + 4]->x = static_cast<float>(Light::LightType::Point);
        break;
    case 1:
        lightProperties[index * LightManager::propertySize + 4]->x = static_cast<float>(Light::LightType::Spot);
        break;
    default:
        break;
    }
}

void MyImGUI::Helper::Models()
{
    if (ImGui::CollapsingHeader("Models"))
    {
        ImGui::InputText("Model Path", buf, 64);

        ImGui::Checkbox("Flip Normal?", flip);

        // Add uv combo
        const char* uvItems[] = { "Shperical", "Cylindrical", "Planer (6-sided)" };
        ImGui::Combo("Select UV type", &currentUVType, uvItems, IM_ARRAYSIZE(uvItems));

        switch (currentUVType)
        {
        case 0:
            *uv = Mesh::UVType::SPHERICAL_UV;
            break;
        case 1:
            *uv = Mesh::UVType::CYLINDRICAL_UV;
            break;
        case 2:
            *uv = Mesh::UVType::CUBE_MAPPED_UV;
            break;
        default:
            break;
        }

        ImGui::Checkbox("Calculate UV on CPU", calculateUVonCPU);

        if (ImGui::Button("Load Model"))
        {
            *shouldReload = true;
        }

        ImGui::SliderFloat3("Model Diffuse", diffuse, 0.f, 1.f);
        ImGui::SliderFloat3("Model Specular", specular, 0.f, 1.f);
    }
}

void MyImGUI::Helper::HybridRenders()
{
    if (ImGui::CollapsingHeader("Hybrid Rendering Debuggings"))
    {
        ImGui::Checkbox("Show render targets in G-Buffer", gBufferRenderTargetFlag);
        ImGui::Checkbox("Copy depth information", depthWriteFlag);
        if (isDrawDebugObjects != nullptr)
        {
            ImGui::Checkbox("Draw Debug Objects", isDrawDebugObjects);
        }
    }
}

void MyImGUI::Helper::Shadows()
{
    if (ImGui::CollapsingHeader("Shadow configurations"))
    {
        ImGui::SliderFloat("Light Depth Offset", lightDepthOffset, -0.5f, 0.5f);
        ImGui::SliderInt("Blur Strength", blurStrength, 0, 5);
    }
}

void MyImGUI::Helper::LightScenarios()
{
    if (ImGui::CollapsingHeader("Light Scenarios"))
    {
        if (ImGui::Button("Scenario 1"))
        {
            MyImGUI::Helper::LightScene1();
        }
        if (ImGui::Button("Scenario 2"))
        {
            MyImGUI::Helper::LightScene2();
        }
        if (ImGui::Button("Scenario 3"))
        {
            MyImGUI::Helper::LightScene3();
        }
    }
}

void MyImGUI::Helper::LightScene1()
{
    static const glm::vec3 scene1Translations[LightManager::MAX_LIGHT] = {
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
    };
    static const glm::vec3 scene1RotationAxis[LightManager::MAX_LIGHT] = {
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
    };
    lightm->SetLightSize(LightManager::MAX_LIGHT);
    for (int i = 0; i < LightManager::MAX_LIGHT; i++)
    {
        Light& light = lightm->GetLightReference(i);
        light.SetLightTranslation(scene1Translations[i]);
        light.SetLightRotationAxis(scene1RotationAxis[i]);
        light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
        light.SetLightSpotFalloff(1.f);
        (*light.GetTypePointer()) = static_cast<float>(Light::LightType::Point);
        glm::vec3 ambient = glm::vec3(1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT);
        glm::vec3 diffuse = glm::vec3(1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT);
        glm::vec3 specular = glm::vec3(1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT, 1.f / LightManager::MAX_LIGHT);
        light.SetLightColor(ambient, diffuse, specular);
    }
}

void MyImGUI::Helper::LightScene2()
{
    static const glm::vec3 scene2Translations[LightManager::MAX_LIGHT] = {
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
    };
    static const glm::vec3 scene2RotationAxis[LightManager::MAX_LIGHT] = {
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
    };
    lightm->SetLightSize(LightManager::MAX_LIGHT);
    for (int i = 0; i < LightManager::MAX_LIGHT; i++)
    {
        if (i < LightManager::MAX_LIGHT / 2)
        {
            Light& light = lightm->GetLightReference(i);
            light.SetLightTranslation(scene2Translations[i]);
            light.SetLightRotationAxis(scene2RotationAxis[i]);
            light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
            light.SetLightSpotFalloff(1.f);
            (*light.GetTypePointer()) = static_cast<float>(Light::LightType::Point);
            glm::vec3 ambient = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            glm::vec3 diffuse = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            glm::vec3 specular = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            light.SetLightColor(ambient, diffuse, specular);
        }
        else
        {
            Light& light = lightm->GetLightReference(i);

            light.SetLightTranslation(scene2Translations[i]);
            light.SetLightRotationAxis(scene2RotationAxis[i]);
            light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
            light.SetLightSpotFalloff(1.f);
            (*light.GetTypePointer()) = static_cast<float>(Light::LightType::Spot);
            glm::vec3 ambient = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            glm::vec3 diffuse = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            glm::vec3 specular = glm::vec3(0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT, 0.5f * i / LightManager::MAX_LIGHT);
            light.SetLightColor(ambient, diffuse, specular);
        }
    }

    (*lightm->GetLightReference(5).GetTypePointer()) = static_cast<float>(Light::LightType::Point);
}

void MyImGUI::Helper::LightScene3()
{

    static const glm::vec3 scene3Translations[LightManager::MAX_LIGHT] = {
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
    };
    static glm::vec3 scene3RotationAxis[LightManager::MAX_LIGHT] = {
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
    };
    static glm::vec3 scene3Ambients[LightManager::MAX_LIGHT] = {
        glm::vec3(41, 44, 8),
        glm::vec3(50, 43, 14),
        glm::vec3(34, 11, 11),
        glm::vec3(3, 26, 23),
        glm::vec3(36, 36, 36),
        glm::vec3(53, 18, 18),
        glm::vec3(101, 98, 0),
        glm::vec3(0, 25, 0),
        glm::vec3(89, 0, 39),
        glm::vec3(42, 75, 0),
        glm::vec3(158, 0, 0),
        glm::vec3(0, 83, 255),
        glm::vec3(0, 135, 255),
        glm::vec3(225, 0, 255),
        glm::vec3(59, 0, 59),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),

        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),

        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),

        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
        glm::vec3(26, 35, 95),
    };
    static glm::vec3 scene3Diffuse[LightManager::MAX_LIGHT] = {
        glm::vec3(27, 31, 6),
        glm::vec3(80, 36, 36),
        glm::vec3(45, 12, 12),
        glm::vec3(55, 23, 23),
        glm::vec3(34, 34, 34),
        glm::vec3(31, 19, 19),
        glm::vec3(85, 80, 0),
        glm::vec3(4, 0, 68),
        glm::vec3(0, 43, 104),
        glm::vec3(41, 116, 0),
        glm::vec3(179, 4, 4),
        glm::vec3(0, 143, 255),
        glm::vec3(3, 255, 144),
        glm::vec3(89, 20, 255),
        glm::vec3(54, 0, 61),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),

        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),

        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),

        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
        glm::vec3(49, 16, 93),
    };
    static glm::vec3 scene3Specular[LightManager::MAX_LIGHT] = {
        glm::vec3(43, 44, 10),
        glm::vec3(15, 21, 43),
        glm::vec3(41, 21, 21),
        glm::vec3(64, 0, 0),
        glm::vec3(55, 55, 55),
        glm::vec3(18, 7, 7),
        glm::vec3(14, 39, 0),
        glm::vec3(248, 0, 255),
        glm::vec3(45, 4, 53),
        glm::vec3(3, 104, 0),
        glm::vec3(161, 0, 0),
        glm::vec3(0, 45, 255),
        glm::vec3(0, 255, 75),
        glm::vec3(180, 0, 255),
        glm::vec3(49, 0, 46),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),

        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
        glm::vec3(84, 12, 95),
    };
    if (scene3Ambients[0].x > 3.f)
    {
        for (int i = 0; i < LightManager::MAX_LIGHT; i++)
        {
            scene3Ambients[i].x /= 255;
            scene3Ambients[i].y /= 255;
            scene3Ambients[i].z /= 255;
            scene3Diffuse[i].x /= 255;
            scene3Diffuse[i].y /= 255;
            scene3Diffuse[i].z /= 255;
            scene3Specular[i].x /= 255;
            scene3Specular[i].y /= 255;
            scene3Specular[i].z /= 255;
        }
    }

    lightm->SetLightSize(LightManager::MAX_LIGHT);
    for (int i = 0; i < LightManager::MAX_LIGHT; i++)
    {
        if (i < LightManager::MAX_LIGHT / 2)
        {
            Light& light = lightm->GetLightReference(i);
            light.SetLightTranslation(scene3Translations[i]);
            light.SetLightRotationAxis(scene3RotationAxis[i]);
            light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
            light.SetLightSpotFalloff(1.f);
            (*light.GetTypePointer()) = static_cast<float>(Light::LightType::Point);
            light.SetLightColor(scene3Ambients[i], scene3Diffuse[i], scene3Specular[i]);
        }
        else
        {
            Light& light = lightm->GetLightReference(i);

            light.SetLightTranslation(scene3Translations[i]);
            light.SetLightRotationAxis(scene3RotationAxis[i]);
            light.SetLightAngles(glm::half_pi<float>() / 3, glm::quarter_pi<float>());
            light.SetLightSpotFalloff(1.f);
            (*light.GetTypePointer()) = static_cast<float>(Light::LightType::Spot);
            light.SetLightColor(scene3Ambients[i], scene3Diffuse[i], scene3Specular[i]);
        }
    }

    (*lightm->GetLightReference(5).GetTypePointer()) = static_cast<float>(Light::LightType::Point);
}

void MyImGUI::Helper::Normals()
{
    if (ImGui::CollapsingHeader("Normals"))
    {
        ImGui::Checkbox("Display Vertex Normal", vertexNormalFlag);
        ImGui::Checkbox("Display Face Normal", faceNormalFlag);
    }
}
