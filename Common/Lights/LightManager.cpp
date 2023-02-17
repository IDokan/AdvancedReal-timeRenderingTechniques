
/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LightManager.cpp
Purpose: LightManager source code
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 11/2/2021
End Header --------------------------------------------------------*/

#include <GL/glew.h>
#include "LightManager.h"

LightManager::LightManager()
{
	currentLightSize = 1;
	lights.resize(MAX_LIGHT);

	intensity = glm::vec3(1.f, 1.f, 1.f);
	lightDirection = glm::vec3(0.4f, -1.f, -0.5f);
}

void LightManager::SetLightSize(unsigned size)
{
	if (size > MAX_LIGHT)
	{
		currentLightSize = MAX_LIGHT;
		return;
	}
	else if (size < 0)
	{
		currentLightSize = 0;
		return;
	}

	currentLightSize = size;
}

const GLchar** LightManager::GetLightUniformBlockNames()
{
	constexpr int blockNameSize = propertySize * MAX_LIGHT;
	static const GLchar* blockNames[blockNameSize] = {
	   "Block.item[0].lightPosition",							// vec3
	   "Block.item[0].intensityAmbient",					// vec3
	   "Block.item[0].intensityDiffuse",					// vec3
	   "Block.item[0].intensitySpecular",					// vec3
		"Block.item[0].lightType",
		"Block.item[0].lightDirection",
		"Block.item[0].anglesInnerOuter",
		"Block.item[0].spotFalloff",

	   "Block.item[1].lightPosition",							// vec3
	   "Block.item[1].intensityAmbient",					// vec3
	   "Block.item[1].intensityDiffuse",					// vec3
	   "Block.item[1].intensitySpecular",					// vec3
		"Block.item[1].lightType",
		"Block.item[1].lightDirection",
		"Block.item[1].anglesInnerOuter",
		"Block.item[1].spotFalloff",

	   "Block.item[2].lightPosition",							// vec3
	   "Block.item[2].intensityAmbient",					// vec3
	   "Block.item[2].intensityDiffuse",					// vec3
	   "Block.item[2].intensitySpecular",					// vec3
		"Block.item[2].lightType",
		"Block.item[2].lightDirection",
		"Block.item[2].anglesInnerOuter",
		"Block.item[2].spotFalloff",

	   "Block.item[3].lightPosition",							// vec3
	   "Block.item[3].intensityAmbient",					// vec3
	   "Block.item[3].intensityDiffuse",					// vec3
	   "Block.item[3].intensitySpecular",					// vec3
		"Block.item[3].lightType",
		"Block.item[3].lightDirection",
		"Block.item[3].anglesInnerOuter",
		"Block.item[3].spotFalloff",

	   "Block.item[4].lightPosition",							// vec3
	   "Block.item[4].intensityAmbient",					// vec3
	   "Block.item[4].intensityDiffuse",					// vec3
	   "Block.item[4].intensitySpecular",					// vec3
		"Block.item[4].lightType",
		"Block.item[4].lightDirection",
		"Block.item[4].anglesInnerOuter",
		"Block.item[4].spotFalloff",

	   "Block.item[5].lightPosition",							// vec3
	   "Block.item[5].intensityAmbient",					// vec3
	   "Block.item[5].intensityDiffuse",					// vec3
	   "Block.item[5].intensitySpecular",					// vec3
		"Block.item[5].lightType",
		"Block.item[5].lightDirection",
		"Block.item[5].anglesInnerOuter",
		"Block.item[5].spotFalloff",

	   "Block.item[6].lightPosition",							// vec3
	   "Block.item[6].intensityAmbient",					// vec3
	   "Block.item[6].intensityDiffuse",					// vec3
	   "Block.item[6].intensitySpecular",					// vec3
		"Block.item[6].lightType",
		"Block.item[6].lightDirection",
		"Block.item[6].anglesInnerOuter",
		"Block.item[6].spotFalloff",

	   "Block.item[7].lightPosition",							// vec3
	   "Block.item[7].intensityAmbient",					// vec3
	   "Block.item[7].intensityDiffuse",					// vec3
	   "Block.item[7].intensitySpecular",					// vec3
		"Block.item[7].lightType",
		"Block.item[7].lightDirection",
		"Block.item[7].anglesInnerOuter",
		"Block.item[7].spotFalloff",

	   "Block.item[8].lightPosition",							// vec3
	   "Block.item[8].intensityAmbient",					// vec3
	   "Block.item[8].intensityDiffuse",					// vec3
	   "Block.item[8].intensitySpecular",					// vec3
		"Block.item[8].lightType",
		"Block.item[8].lightDirection",
		"Block.item[8].anglesInnerOuter",
		"Block.item[8].spotFalloff",

	   "Block.item[9].lightPosition",							// vec3
	   "Block.item[9].intensityAmbient",					// vec3
	   "Block.item[9].intensityDiffuse",					// vec3
	   "Block.item[9].intensitySpecular",					// vec3
		"Block.item[9].lightType",
		"Block.item[9].lightDirection",
		"Block.item[9].anglesInnerOuter",
		"Block.item[9].spotFalloff",

	   "Block.item[10].lightPosition",							// vec3
	   "Block.item[10].intensityAmbient",					// vec3
	   "Block.item[10].intensityDiffuse",					// vec3
	   "Block.item[10].intensitySpecular",					// vec3
		"Block.item[10].lightType",
		"Block.item[10].lightDirection",
		"Block.item[10].anglesInnerOuter",
		"Block.item[10].spotFalloff",

	   "Block.item[11].lightPosition",							// vec3
	   "Block.item[11].intensityAmbient",					// vec3
	   "Block.item[11].intensityDiffuse",					// vec3
	   "Block.item[11].intensitySpecular",					// vec3
		   "Block.item[11].lightType",
		   "Block.item[11].lightDirection",
		   "Block.item[11].anglesInnerOuter",
		   "Block.item[11].spotFalloff",

	   "Block.item[12].lightPosition",							// vec3
	   "Block.item[12].intensityAmbient",					// vec3
	   "Block.item[12].intensityDiffuse",					// vec3
	   "Block.item[12].intensitySpecular",					// vec3
		   "Block.item[12].lightType",
		   "Block.item[12].lightDirection",
		   "Block.item[12].anglesInnerOuter",
		   "Block.item[12].spotFalloff",

	   "Block.item[13].lightPosition",							// vec3
	   "Block.item[13].intensityAmbient",					// vec3
	   "Block.item[13].intensityDiffuse",					// vec3
	   "Block.item[13].intensitySpecular",					// vec3
		   "Block.item[13].lightType",
		   "Block.item[13].lightDirection",
		   "Block.item[13].anglesInnerOuter",
		   "Block.item[13].spotFalloff",

	   "Block.item[14].lightPosition",							// vec3
	   "Block.item[14].intensityAmbient",					// vec3
	   "Block.item[14].intensityDiffuse",					// vec3
	   "Block.item[14].intensitySpecular",					// vec3
		   "Block.item[14].lightType",
		   "Block.item[14].lightDirection",
		   "Block.item[14].anglesInnerOuter",
		   "Block.item[14].spotFalloff",

	   "Block.item[15].lightPosition",							// vec3
	   "Block.item[15].intensityAmbient",					// vec3
	   "Block.item[15].intensityDiffuse",					// vec3
	   "Block.item[15].intensitySpecular",					// vec3
		   "Block.item[15].lightType",
		   "Block.item[15].lightDirection",
		   "Block.item[15].anglesInnerOuter",
		   "Block.item[15].spotFalloff",

			"Block.item[16].lightPosition",							// vec3
			"Block.item[16].intensityAmbient",					// vec3
			"Block.item[16].intensityDiffuse",					// vec3
			"Block.item[16].intensitySpecular",					// vec3
			"Block.item[16].lightType",
			"Block.item[16].lightDirection",
			"Block.item[16].anglesInnerOuter",
			"Block.item[16].spotFalloff",

			"Block.item[17].lightPosition",							// vec3
			"Block.item[17].intensityAmbient",					// vec3
			"Block.item[17].intensityDiffuse",					// vec3
			"Block.item[17].intensitySpecular",					// vec3
			"Block.item[17].lightType",
			"Block.item[17].lightDirection",
			"Block.item[17].anglesInnerOuter",
			"Block.item[17].spotFalloff",

			"Block.item[18].lightPosition",							// vec3
			"Block.item[18].intensityAmbient",					// vec3
			"Block.item[18].intensityDiffuse",					// vec3
			"Block.item[18].intensitySpecular",					// vec3
			"Block.item[18].lightType",
			"Block.item[18].lightDirection",
			"Block.item[18].anglesInnerOuter",
			"Block.item[18].spotFalloff",

			"Block.item[19].lightPosition",							// vec3
			"Block.item[19].intensityAmbient",					// vec3
			"Block.item[19].intensityDiffuse",					// vec3
			"Block.item[19].intensitySpecular",					// vec3
			"Block.item[19].lightType",
			"Block.item[19].lightDirection",
			"Block.item[19].anglesInnerOuter",
			"Block.item[19].spotFalloff",

			"Block.item[20].lightPosition",							// vec3
			"Block.item[20].intensityAmbient",					// vec3
			"Block.item[20].intensityDiffuse",					// vec3
			"Block.item[20].intensitySpecular",					// vec3
			"Block.item[20].lightType",
			"Block.item[20].lightDirection",
			"Block.item[20].anglesInnerOuter",
			"Block.item[20].spotFalloff",

			"Block.item[21].lightPosition",							// vec3
			"Block.item[21].intensityAmbient",					// vec3
			"Block.item[21].intensityDiffuse",					// vec3
			"Block.item[21].intensitySpecular",					// vec3
			"Block.item[21].lightType",
			"Block.item[21].lightDirection",
			"Block.item[21].anglesInnerOuter",
			"Block.item[21].spotFalloff",

			"Block.item[22].lightPosition",							// vec3
			"Block.item[22].intensityAmbient",					// vec3
			"Block.item[22].intensityDiffuse",					// vec3
			"Block.item[22].intensitySpecular",					// vec3
			"Block.item[22].lightType",
			"Block.item[22].lightDirection",
			"Block.item[22].anglesInnerOuter",
			"Block.item[22].spotFalloff",

			"Block.item[23].lightPosition",							// vec3
			"Block.item[23].intensityAmbient",					// vec3
			"Block.item[23].intensityDiffuse",					// vec3
			"Block.item[23].intensitySpecular",					// vec3
			"Block.item[23].lightType",
			"Block.item[23].lightDirection",
			"Block.item[23].anglesInnerOuter",
			"Block.item[23].spotFalloff",

			"Block.item[24].lightPosition",							// vec3
			"Block.item[24].intensityAmbient",					// vec3
			"Block.item[24].intensityDiffuse",					// vec3
			"Block.item[24].intensitySpecular",					// vec3
			"Block.item[24].lightType",
			"Block.item[24].lightDirection",
			"Block.item[24].anglesInnerOuter",
			"Block.item[24].spotFalloff",

			"Block.item[25].lightPosition",							// vec3
			"Block.item[25].intensityAmbient",					// vec3
			"Block.item[25].intensityDiffuse",					// vec3
			"Block.item[25].intensitySpecular",					// vec3
			"Block.item[25].lightType",
			"Block.item[25].lightDirection",
			"Block.item[25].anglesInnerOuter",
			"Block.item[25].spotFalloff",

			"Block.item[26].lightPosition",							// vec3
			"Block.item[26].intensityAmbient",					// vec3
			"Block.item[26].intensityDiffuse",					// vec3
			"Block.item[26].intensitySpecular",					// vec3
			"Block.item[26].lightType",
			"Block.item[26].lightDirection",
			"Block.item[26].anglesInnerOuter",
			"Block.item[26].spotFalloff",

			"Block.item[27].lightPosition",							// vec3
			"Block.item[27].intensityAmbient",					// vec3
			"Block.item[27].intensityDiffuse",					// vec3
			"Block.item[27].intensitySpecular",					// vec3
			"Block.item[27].lightType",
			"Block.item[27].lightDirection",
			"Block.item[27].anglesInnerOuter",
			"Block.item[27].spotFalloff",

			"Block.item[28].lightPosition",							// vec3
			"Block.item[28].intensityAmbient",					// vec3
			"Block.item[28].intensityDiffuse",					// vec3
			"Block.item[28].intensitySpecular",					// vec3
			"Block.item[28].lightType",
			"Block.item[28].lightDirection",
			"Block.item[28].anglesInnerOuter",
			"Block.item[28].spotFalloff",

			"Block.item[29].lightPosition",							// vec3
			"Block.item[29].intensityAmbient",					// vec3
			"Block.item[29].intensityDiffuse",					// vec3
			"Block.item[29].intensitySpecular",					// vec3
			"Block.item[29].lightType",
			"Block.item[29].lightDirection",
			"Block.item[29].anglesInnerOuter",
			"Block.item[29].spotFalloff",

				"Block.item[30].lightPosition",							// vec3
				"Block.item[30].intensityAmbient",					// vec3
				"Block.item[30].intensityDiffuse",					// vec3
				"Block.item[30].intensitySpecular",					// vec3
				"Block.item[30].lightType",
				"Block.item[30].lightDirection",
				"Block.item[30].anglesInnerOuter",
				"Block.item[30].spotFalloff",

				"Block.item[31].lightPosition",							// vec3
				"Block.item[31].intensityAmbient",					// vec3
				"Block.item[31].intensityDiffuse",					// vec3
				"Block.item[31].intensitySpecular",					// vec3
				"Block.item[31].lightType",
				"Block.item[31].lightDirection",
				"Block.item[31].anglesInnerOuter",
				"Block.item[31].spotFalloff",

				"Block.item[32].lightPosition",							// vec3
				"Block.item[32].intensityAmbient",					// vec3
				"Block.item[32].intensityDiffuse",					// vec3
				"Block.item[32].intensitySpecular",					// vec3
				"Block.item[32].lightType",
				"Block.item[32].lightDirection",
				"Block.item[32].anglesInnerOuter",
				"Block.item[32].spotFalloff",

				"Block.item[33].lightPosition",							// vec3
				"Block.item[33].intensityAmbient",					// vec3
				"Block.item[33].intensityDiffuse",					// vec3
				"Block.item[33].intensitySpecular",					// vec3
				"Block.item[33].lightType",
				"Block.item[33].lightDirection",
				"Block.item[33].anglesInnerOuter",
				"Block.item[33].spotFalloff",

				"Block.item[34].lightPosition",							// vec3
				"Block.item[34].intensityAmbient",					// vec3
				"Block.item[34].intensityDiffuse",					// vec3
				"Block.item[34].intensitySpecular",					// vec3
				"Block.item[34].lightType",
				"Block.item[34].lightDirection",
				"Block.item[34].anglesInnerOuter",
				"Block.item[34].spotFalloff",

				"Block.item[35].lightPosition",							// vec3
				"Block.item[35].intensityAmbient",					// vec3
				"Block.item[35].intensityDiffuse",					// vec3
				"Block.item[35].intensitySpecular",					// vec3
				"Block.item[35].lightType",
				"Block.item[35].lightDirection",
				"Block.item[35].anglesInnerOuter",
				"Block.item[35].spotFalloff",

				"Block.item[36].lightPosition",							// vec3
				"Block.item[36].intensityAmbient",					// vec3
				"Block.item[36].intensityDiffuse",					// vec3
				"Block.item[36].intensitySpecular",					// vec3
				"Block.item[36].lightType",
				"Block.item[36].lightDirection",
				"Block.item[36].anglesInnerOuter",
				"Block.item[36].spotFalloff",

				"Block.item[37].lightPosition",							// vec3
				"Block.item[37].intensityAmbient",					// vec3
				"Block.item[37].intensityDiffuse",					// vec3
				"Block.item[37].intensitySpecular",					// vec3
				"Block.item[37].lightType",
				"Block.item[37].lightDirection",
				"Block.item[37].anglesInnerOuter",
				"Block.item[37].spotFalloff",

				"Block.item[38].lightPosition",							// vec3
				"Block.item[38].intensityAmbient",					// vec3
				"Block.item[38].intensityDiffuse",					// vec3
				"Block.item[38].intensitySpecular",					// vec3
				"Block.item[38].lightType",
				"Block.item[38].lightDirection",
				"Block.item[38].anglesInnerOuter",
				"Block.item[38].spotFalloff",

				"Block.item[39].lightPosition",							// vec3
				"Block.item[39].intensityAmbient",					// vec3
				"Block.item[39].intensityDiffuse",					// vec3
				"Block.item[39].intensitySpecular",					// vec3
				"Block.item[39].lightType",
				"Block.item[39].lightDirection",
				"Block.item[39].anglesInnerOuter",
				"Block.item[39].spotFalloff",

				"Block.item[40].lightPosition",							// vec3
				"Block.item[40].intensityAmbient",					// vec3
				"Block.item[40].intensityDiffuse",					// vec3
				"Block.item[40].intensitySpecular",					// vec3
				"Block.item[40].lightType",
				"Block.item[40].lightDirection",
				"Block.item[40].anglesInnerOuter",
				"Block.item[40].spotFalloff",

				"Block.item[41].lightPosition",							// vec3
				"Block.item[41].intensityAmbient",					// vec3
				"Block.item[41].intensityDiffuse",					// vec3
				"Block.item[41].intensitySpecular",					// vec3
				"Block.item[41].lightType",
				"Block.item[41].lightDirection",
				"Block.item[41].anglesInnerOuter",
				"Block.item[41].spotFalloff",

				"Block.item[42].lightPosition",							// vec3
				"Block.item[42].intensityAmbient",					// vec3
				"Block.item[42].intensityDiffuse",					// vec3
				"Block.item[42].intensitySpecular",					// vec3
				"Block.item[42].lightType",
				"Block.item[42].lightDirection",
				"Block.item[42].anglesInnerOuter",
				"Block.item[42].spotFalloff",

				"Block.item[43].lightPosition",							// vec3
				"Block.item[43].intensityAmbient",					// vec3
				"Block.item[43].intensityDiffuse",					// vec3
				"Block.item[43].intensitySpecular",					// vec3
				"Block.item[43].lightType",
				"Block.item[43].lightDirection",
				"Block.item[43].anglesInnerOuter",
				"Block.item[43].spotFalloff",

				"Block.item[44].lightPosition",							// vec3
				"Block.item[44].intensityAmbient",					// vec3
				"Block.item[44].intensityDiffuse",					// vec3
				"Block.item[44].intensitySpecular",					// vec3
				"Block.item[44].lightType",
				"Block.item[44].lightDirection",
				"Block.item[44].anglesInnerOuter",
				"Block.item[44].spotFalloff",

				"Block.item[45].lightPosition",							// vec3
				"Block.item[45].intensityAmbient",					// vec3
				"Block.item[45].intensityDiffuse",					// vec3
				"Block.item[45].intensitySpecular",					// vec3
				"Block.item[45].lightType",
				"Block.item[45].lightDirection",
				"Block.item[45].anglesInnerOuter",
				"Block.item[45].spotFalloff",

				"Block.item[46].lightPosition",							// vec3
				"Block.item[46].intensityAmbient",					// vec3
				"Block.item[46].intensityDiffuse",					// vec3
				"Block.item[46].intensitySpecular",					// vec3
				"Block.item[46].lightType",
				"Block.item[46].lightDirection",
				"Block.item[46].anglesInnerOuter",
				"Block.item[46].spotFalloff",

				"Block.item[47].lightPosition",							// vec3
				"Block.item[47].intensityAmbient",					// vec3
				"Block.item[47].intensityDiffuse",					// vec3
				"Block.item[47].intensitySpecular",					// vec3
				"Block.item[47].lightType",
				"Block.item[47].lightDirection",
				"Block.item[47].anglesInnerOuter",
				"Block.item[47].spotFalloff",

				"Block.item[48].lightPosition",							// vec3
				"Block.item[48].intensityAmbient",					// vec3
				"Block.item[48].intensityDiffuse",					// vec3
				"Block.item[48].intensitySpecular",					// vec3
				"Block.item[48].lightType",
				"Block.item[48].lightDirection",
				"Block.item[48].anglesInnerOuter",
				"Block.item[48].spotFalloff",

				"Block.item[49].lightPosition",							// vec3
				"Block.item[49].intensityAmbient",					// vec3
				"Block.item[49].intensityDiffuse",					// vec3
				"Block.item[49].intensitySpecular",					// vec3
				"Block.item[49].lightType",
				"Block.item[49].lightDirection",
				"Block.item[49].anglesInnerOuter",
				"Block.item[49].spotFalloff",
	};
	return blockNames;
}

const float** LightManager::GetLightUniformBlockData()
{
	static float* data[lightUniformDataSize] = {};
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		data[i * propertySize] = lights[i].GetPositionPointer();
		data[i * propertySize + 1] = lights[i].GetAmbientPointer();
		data[i * propertySize + 2] = lights[i].GetDiffusePointer();
		data[i * propertySize + 3] = lights[i].GetSpecularPointer();
		data[i * propertySize + 4] = lights[i].GetTypePointer();		// type
		data[i * propertySize + 5] = lights[i].GetDirectionPointer();			// direction
		data[i * propertySize + 6] = lights[i].GetAnglesPointer();		// angles
		data[i * propertySize + 7] = lights[i].GetSpotFalloff();		// spot falloff
	}
	return const_cast<const float**>(reinterpret_cast<float**>(data));
}

const int LightManager::GetLightUniformDataSize()
{
	return lightUniformDataSize;
}

int* LightManager::GetCurrentLightSizeReference()
{
	return &currentLightSize;
}

Light& LightManager::GetLightReference(unsigned index)
{
	return lights.at(index);
}

float* LightManager::GetDirectionalLightIntensity()
{
	return &intensity.x;
}

float* LightManager::GetDirectionalLightDirection()
{
	return &lightDirection.x;
}
