
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
					"Block.item[50].lightPosition",							// vec3
					"Block.item[50].intensityAmbient",					// vec3
					"Block.item[50].intensityDiffuse",					// vec3
					"Block.item[50].intensitySpecular",					// vec3
					"Block.item[50].lightType",
					"Block.item[50].lightDirection",
					"Block.item[50].anglesInnerOuter",
					"Block.item[50].spotFalloff",
					"Block.item[51].lightPosition",							// vec3
					"Block.item[51].intensityAmbient",					// vec3
					"Block.item[51].intensityDiffuse",					// vec3
					"Block.item[51].intensitySpecular",					// vec3
					"Block.item[51].lightType",
					"Block.item[51].lightDirection",
					"Block.item[51].anglesInnerOuter",
					"Block.item[51].spotFalloff",
					"Block.item[52].lightPosition",							// vec3
					"Block.item[52].intensityAmbient",					// vec3
					"Block.item[52].intensityDiffuse",					// vec3
					"Block.item[52].intensitySpecular",					// vec3
					"Block.item[52].lightType",
					"Block.item[52].lightDirection",
					"Block.item[52].anglesInnerOuter",
					"Block.item[52].spotFalloff",
					"Block.item[53].lightPosition",							// vec3
					"Block.item[53].intensityAmbient",					// vec3
					"Block.item[53].intensityDiffuse",					// vec3
					"Block.item[53].intensitySpecular",					// vec3
					"Block.item[53].lightType",
					"Block.item[53].lightDirection",
					"Block.item[53].anglesInnerOuter",
					"Block.item[53].spotFalloff",
					"Block.item[54].lightPosition",							// vec3
					"Block.item[54].intensityAmbient",					// vec3
					"Block.item[54].intensityDiffuse",					// vec3
					"Block.item[54].intensitySpecular",					// vec3
					"Block.item[54].lightType",
					"Block.item[54].lightDirection",
					"Block.item[54].anglesInnerOuter",
					"Block.item[54].spotFalloff",
					"Block.item[55].lightPosition",							// vec3
					"Block.item[55].intensityAmbient",					// vec3
					"Block.item[55].intensityDiffuse",					// vec3
					"Block.item[55].intensitySpecular",					// vec3
					"Block.item[55].lightType",
					"Block.item[55].lightDirection",
					"Block.item[55].anglesInnerOuter",
					"Block.item[55].spotFalloff",
					"Block.item[56].lightPosition",							// vec3
					"Block.item[56].intensityAmbient",					// vec3
					"Block.item[56].intensityDiffuse",					// vec3
					"Block.item[56].intensitySpecular",					// vec3
					"Block.item[56].lightType",
					"Block.item[56].lightDirection",
					"Block.item[56].anglesInnerOuter",
					"Block.item[56].spotFalloff",
					"Block.item[57].lightPosition",							// vec3
					"Block.item[57].intensityAmbient",					// vec3
					"Block.item[57].intensityDiffuse",					// vec3
					"Block.item[57].intensitySpecular",					// vec3
					"Block.item[57].lightType",
					"Block.item[57].lightDirection",
					"Block.item[57].anglesInnerOuter",
					"Block.item[57].spotFalloff",
					"Block.item[58].lightPosition",							// vec3
					"Block.item[58].intensityAmbient",					// vec3
					"Block.item[58].intensityDiffuse",					// vec3
					"Block.item[58].intensitySpecular",					// vec3
					"Block.item[58].lightType",
					"Block.item[58].lightDirection",
					"Block.item[58].anglesInnerOuter",
					"Block.item[58].spotFalloff",
					"Block.item[59].lightPosition",							// vec3
					"Block.item[59].intensityAmbient",					// vec3
					"Block.item[59].intensityDiffuse",					// vec3
					"Block.item[59].intensitySpecular",					// vec3
					"Block.item[59].lightType",
					"Block.item[59].lightDirection",
					"Block.item[59].anglesInnerOuter",
					"Block.item[59].spotFalloff",
					"Block.item[60].lightPosition",							// vec3
					"Block.item[60].intensityAmbient",					// vec3
					"Block.item[60].intensityDiffuse",					// vec3
					"Block.item[60].intensitySpecular",					// vec3
					"Block.item[60].lightType",
					"Block.item[60].lightDirection",
					"Block.item[60].anglesInnerOuter",
					"Block.item[60].spotFalloff",
					"Block.item[61].lightPosition",							// vec3
					"Block.item[61].intensityAmbient",					// vec3
					"Block.item[61].intensityDiffuse",					// vec3
					"Block.item[61].intensitySpecular",					// vec3
					"Block.item[61].lightType",
					"Block.item[61].lightDirection",
					"Block.item[61].anglesInnerOuter",
					"Block.item[61].spotFalloff",
					"Block.item[62].lightPosition",							// vec3
					"Block.item[62].intensityAmbient",					// vec3
					"Block.item[62].intensityDiffuse",					// vec3
					"Block.item[62].intensitySpecular",					// vec3
					"Block.item[62].lightType",
					"Block.item[62].lightDirection",
					"Block.item[62].anglesInnerOuter",
					"Block.item[62].spotFalloff",
					"Block.item[63].lightPosition",							// vec3
					"Block.item[63].intensityAmbient",					// vec3
					"Block.item[63].intensityDiffuse",					// vec3
					"Block.item[63].intensitySpecular",					// vec3
					"Block.item[63].lightType",
					"Block.item[63].lightDirection",
					"Block.item[63].anglesInnerOuter",
					"Block.item[63].spotFalloff",
					"Block.item[64].lightPosition",							// vec3
					"Block.item[64].intensityAmbient",					// vec3
					"Block.item[64].intensityDiffuse",					// vec3
					"Block.item[64].intensitySpecular",					// vec3
					"Block.item[64].lightType",
					"Block.item[64].lightDirection",
					"Block.item[64].anglesInnerOuter",
					"Block.item[64].spotFalloff",
					"Block.item[65].lightPosition",							// vec3
					"Block.item[65].intensityAmbient",					// vec3
					"Block.item[65].intensityDiffuse",					// vec3
					"Block.item[65].intensitySpecular",					// vec3
					"Block.item[65].lightType",
					"Block.item[65].lightDirection",
					"Block.item[65].anglesInnerOuter",
					"Block.item[65].spotFalloff",
					"Block.item[66].lightPosition",							// vec3
					"Block.item[66].intensityAmbient",					// vec3
					"Block.item[66].intensityDiffuse",					// vec3
					"Block.item[66].intensitySpecular",					// vec3
					"Block.item[66].lightType",
					"Block.item[66].lightDirection",
					"Block.item[66].anglesInnerOuter",
					"Block.item[66].spotFalloff",
					"Block.item[67].lightPosition",							// vec3
					"Block.item[67].intensityAmbient",					// vec3
					"Block.item[67].intensityDiffuse",					// vec3
					"Block.item[67].intensitySpecular",					// vec3
					"Block.item[67].lightType",
					"Block.item[67].lightDirection",
					"Block.item[67].anglesInnerOuter",
					"Block.item[67].spotFalloff",
					"Block.item[68].lightPosition",							// vec3
					"Block.item[68].intensityAmbient",					// vec3
					"Block.item[68].intensityDiffuse",					// vec3
					"Block.item[68].intensitySpecular",					// vec3
					"Block.item[68].lightType",
					"Block.item[68].lightDirection",
					"Block.item[68].anglesInnerOuter",
					"Block.item[68].spotFalloff",
					"Block.item[69].lightPosition",							// vec3
					"Block.item[69].intensityAmbient",					// vec3
					"Block.item[69].intensityDiffuse",					// vec3
					"Block.item[69].intensitySpecular",					// vec3
					"Block.item[69].lightType",
					"Block.item[69].lightDirection",
					"Block.item[69].anglesInnerOuter",
					"Block.item[69].spotFalloff",
					"Block.item[70].lightPosition",							// vec3
					"Block.item[70].intensityAmbient",					// vec3
					"Block.item[70].intensityDiffuse",					// vec3
					"Block.item[70].intensitySpecular",					// vec3
					"Block.item[70].lightType",
					"Block.item[70].lightDirection",
					"Block.item[70].anglesInnerOuter",
					"Block.item[70].spotFalloff",
					"Block.item[71].lightPosition",							// vec3
					"Block.item[71].intensityAmbient",					// vec3
					"Block.item[71].intensityDiffuse",					// vec3
					"Block.item[71].intensitySpecular",					// vec3
					"Block.item[71].lightType",
					"Block.item[71].lightDirection",
					"Block.item[71].anglesInnerOuter",
					"Block.item[71].spotFalloff",
					"Block.item[72].lightPosition",							// vec3
					"Block.item[72].intensityAmbient",					// vec3
					"Block.item[72].intensityDiffuse",					// vec3
					"Block.item[72].intensitySpecular",					// vec3
					"Block.item[72].lightType",
					"Block.item[72].lightDirection",
					"Block.item[72].anglesInnerOuter",
					"Block.item[72].spotFalloff",
					"Block.item[73].lightPosition",							// vec3
					"Block.item[73].intensityAmbient",					// vec3
					"Block.item[73].intensityDiffuse",					// vec3
					"Block.item[73].intensitySpecular",					// vec3
					"Block.item[73].lightType",
					"Block.item[73].lightDirection",
					"Block.item[73].anglesInnerOuter",
					"Block.item[73].spotFalloff",
					"Block.item[74].lightPosition",							// vec3
					"Block.item[74].intensityAmbient",					// vec3
					"Block.item[74].intensityDiffuse",					// vec3
					"Block.item[74].intensitySpecular",					// vec3
					"Block.item[74].lightType",
					"Block.item[74].lightDirection",
					"Block.item[74].anglesInnerOuter",
					"Block.item[74].spotFalloff",
					"Block.item[75].lightPosition",							// vec3
					"Block.item[75].intensityAmbient",					// vec3
					"Block.item[75].intensityDiffuse",					// vec3
					"Block.item[75].intensitySpecular",					// vec3
					"Block.item[75].lightType",
					"Block.item[75].lightDirection",
					"Block.item[75].anglesInnerOuter",
					"Block.item[75].spotFalloff",
					"Block.item[76].lightPosition",							// vec3
					"Block.item[76].intensityAmbient",					// vec3
					"Block.item[76].intensityDiffuse",					// vec3
					"Block.item[76].intensitySpecular",					// vec3
					"Block.item[76].lightType",
					"Block.item[76].lightDirection",
					"Block.item[76].anglesInnerOuter",
					"Block.item[76].spotFalloff",
					"Block.item[77].lightPosition",							// vec3
					"Block.item[77].intensityAmbient",					// vec3
					"Block.item[77].intensityDiffuse",					// vec3
					"Block.item[77].intensitySpecular",					// vec3
					"Block.item[77].lightType",
					"Block.item[77].lightDirection",
					"Block.item[77].anglesInnerOuter",
					"Block.item[77].spotFalloff",
					"Block.item[78].lightPosition",							// vec3
					"Block.item[78].intensityAmbient",					// vec3
					"Block.item[78].intensityDiffuse",					// vec3
					"Block.item[78].intensitySpecular",					// vec3
					"Block.item[78].lightType",
					"Block.item[78].lightDirection",
					"Block.item[78].anglesInnerOuter",
					"Block.item[78].spotFalloff",
					"Block.item[79].lightPosition",							// vec3
					"Block.item[79].intensityAmbient",					// vec3
					"Block.item[79].intensityDiffuse",					// vec3
					"Block.item[79].intensitySpecular",					// vec3
					"Block.item[79].lightType",
					"Block.item[79].lightDirection",
					"Block.item[79].anglesInnerOuter",
					"Block.item[79].spotFalloff",
					"Block.item[80].lightPosition",							// vec3
					"Block.item[80].intensityAmbient",					// vec3
					"Block.item[80].intensityDiffuse",					// vec3
					"Block.item[80].intensitySpecular",					// vec3
					"Block.item[80].lightType",
					"Block.item[80].lightDirection",
					"Block.item[80].anglesInnerOuter",
					"Block.item[80].spotFalloff",
					"Block.item[81].lightPosition",							// vec3
					"Block.item[81].intensityAmbient",					// vec3
					"Block.item[81].intensityDiffuse",					// vec3
					"Block.item[81].intensitySpecular",					// vec3
					"Block.item[81].lightType",
					"Block.item[81].lightDirection",
					"Block.item[81].anglesInnerOuter",
					"Block.item[81].spotFalloff",
					"Block.item[82].lightPosition",							// vec3
					"Block.item[82].intensityAmbient",					// vec3
					"Block.item[82].intensityDiffuse",					// vec3
					"Block.item[82].intensitySpecular",					// vec3
					"Block.item[82].lightType",
					"Block.item[82].lightDirection",
					"Block.item[82].anglesInnerOuter",
					"Block.item[82].spotFalloff",
					"Block.item[83].lightPosition",							// vec3
					"Block.item[83].intensityAmbient",					// vec3
					"Block.item[83].intensityDiffuse",					// vec3
					"Block.item[83].intensitySpecular",					// vec3
					"Block.item[83].lightType",
					"Block.item[83].lightDirection",
					"Block.item[83].anglesInnerOuter",
					"Block.item[83].spotFalloff",
					"Block.item[84].lightPosition",							// vec3
					"Block.item[84].intensityAmbient",					// vec3
					"Block.item[84].intensityDiffuse",					// vec3
					"Block.item[84].intensitySpecular",					// vec3
					"Block.item[84].lightType",
					"Block.item[84].lightDirection",
					"Block.item[84].anglesInnerOuter",
					"Block.item[84].spotFalloff",
					"Block.item[85].lightPosition",							// vec3
					"Block.item[85].intensityAmbient",					// vec3
					"Block.item[85].intensityDiffuse",					// vec3
					"Block.item[85].intensitySpecular",					// vec3
					"Block.item[85].lightType",
					"Block.item[85].lightDirection",
					"Block.item[85].anglesInnerOuter",
					"Block.item[85].spotFalloff",
					"Block.item[86].lightPosition",							// vec3
					"Block.item[86].intensityAmbient",					// vec3
					"Block.item[86].intensityDiffuse",					// vec3
					"Block.item[86].intensitySpecular",					// vec3
					"Block.item[86].lightType",
					"Block.item[86].lightDirection",
					"Block.item[86].anglesInnerOuter",
					"Block.item[86].spotFalloff",
					"Block.item[87].lightPosition",							// vec3
					"Block.item[87].intensityAmbient",					// vec3
					"Block.item[87].intensityDiffuse",					// vec3
					"Block.item[87].intensitySpecular",					// vec3
					"Block.item[87].lightType",
					"Block.item[87].lightDirection",
					"Block.item[87].anglesInnerOuter",
					"Block.item[87].spotFalloff",
					"Block.item[88].lightPosition",							// vec3
					"Block.item[88].intensityAmbient",					// vec3
					"Block.item[88].intensityDiffuse",					// vec3
					"Block.item[88].intensitySpecular",					// vec3
					"Block.item[88].lightType",
					"Block.item[88].lightDirection",
					"Block.item[88].anglesInnerOuter",
					"Block.item[88].spotFalloff",
					"Block.item[89].lightPosition",							// vec3
					"Block.item[89].intensityAmbient",					// vec3
					"Block.item[89].intensityDiffuse",					// vec3
					"Block.item[89].intensitySpecular",					// vec3
					"Block.item[89].lightType",
					"Block.item[89].lightDirection",
					"Block.item[89].anglesInnerOuter",
					"Block.item[89].spotFalloff",
					"Block.item[90].lightPosition",							// vec3
					"Block.item[90].intensityAmbient",					// vec3
					"Block.item[90].intensityDiffuse",					// vec3
					"Block.item[90].intensitySpecular",					// vec3
					"Block.item[90].lightType",
					"Block.item[90].lightDirection",
					"Block.item[90].anglesInnerOuter",
					"Block.item[90].spotFalloff",
					"Block.item[91].lightPosition",							// vec3
					"Block.item[91].intensityAmbient",					// vec3
					"Block.item[91].intensityDiffuse",					// vec3
					"Block.item[91].intensitySpecular",					// vec3
					"Block.item[91].lightType",
					"Block.item[91].lightDirection",
					"Block.item[91].anglesInnerOuter",
					"Block.item[91].spotFalloff",
					"Block.item[92].lightPosition",							// vec3
					"Block.item[92].intensityAmbient",					// vec3
					"Block.item[92].intensityDiffuse",					// vec3
					"Block.item[92].intensitySpecular",					// vec3
					"Block.item[92].lightType",
					"Block.item[92].lightDirection",
					"Block.item[92].anglesInnerOuter",
					"Block.item[92].spotFalloff",
					"Block.item[93].lightPosition",							// vec3
					"Block.item[93].intensityAmbient",					// vec3
					"Block.item[93].intensityDiffuse",					// vec3
					"Block.item[93].intensitySpecular",					// vec3
					"Block.item[93].lightType",
					"Block.item[93].lightDirection",
					"Block.item[93].anglesInnerOuter",
					"Block.item[93].spotFalloff",
					"Block.item[94].lightPosition",							// vec3
					"Block.item[94].intensityAmbient",					// vec3
					"Block.item[94].intensityDiffuse",					// vec3
					"Block.item[94].intensitySpecular",					// vec3
					"Block.item[94].lightType",
					"Block.item[94].lightDirection",
					"Block.item[94].anglesInnerOuter",
					"Block.item[94].spotFalloff",
					"Block.item[95].lightPosition",							// vec3
					"Block.item[95].intensityAmbient",					// vec3
					"Block.item[95].intensityDiffuse",					// vec3
					"Block.item[95].intensitySpecular",					// vec3
					"Block.item[95].lightType",
					"Block.item[95].lightDirection",
					"Block.item[95].anglesInnerOuter",
					"Block.item[95].spotFalloff",
					"Block.item[96].lightPosition",							// vec3
					"Block.item[96].intensityAmbient",					// vec3
					"Block.item[96].intensityDiffuse",					// vec3
					"Block.item[96].intensitySpecular",					// vec3
					"Block.item[96].lightType",
					"Block.item[96].lightDirection",
					"Block.item[96].anglesInnerOuter",
					"Block.item[96].spotFalloff",
					//"Block.item[97].lightPosition",							// vec3
					//"Block.item[97].intensityAmbient",					// vec3
					//"Block.item[97].intensityDiffuse",					// vec3
					//"Block.item[97].intensitySpecular",					// vec3
					//"Block.item[97].lightType",
					//"Block.item[97].lightDirection",
					//"Block.item[97].anglesInnerOuter",
					//"Block.item[97].spotFalloff",
					//"Block.item[98].lightPosition",							// vec3
					//"Block.item[98].intensityAmbient",					// vec3
					//"Block.item[98].intensityDiffuse",					// vec3
					//"Block.item[98].intensitySpecular",					// vec3
					//"Block.item[98].lightType",
					//"Block.item[98].lightDirection",
					//"Block.item[98].anglesInnerOuter",
					//"Block.item[98].spotFalloff",
					//"Block.item[99].lightPosition",							// vec3
					//"Block.item[99].intensityAmbient",					// vec3
					//"Block.item[99].intensityDiffuse",					// vec3
					//"Block.item[99].intensitySpecular",					// vec3
					//"Block.item[99].lightType",
					//"Block.item[99].lightDirection",
					//"Block.item[99].anglesInnerOuter",
					//"Block.item[99].spotFalloff",
						//"Block.item[100].lightPosition",							// vec3
						//"Block.item[100].intensityAmbient",					// vec3
						//"Block.item[100].intensityDiffuse",					// vec3
						//"Block.item[100].intensitySpecular",					// vec3
						//"Block.item[100].lightType",
						//"Block.item[100].lightDirection",
						//"Block.item[100].anglesInnerOuter",
						//"Block.item[100].spotFalloff",
						//"Block.item[101].lightPosition",							// vec3
						//"Block.item[101].intensityAmbient",					// vec3
						//"Block.item[101].intensityDiffuse",					// vec3
						//"Block.item[101].intensitySpecular",					// vec3
						//"Block.item[101].lightType",
						//"Block.item[101].lightDirection",
						//"Block.item[101].anglesInnerOuter",
						//"Block.item[101].spotFalloff",
						//"Block.item[102].lightPosition",							// vec3
						//"Block.item[102].intensityAmbient",					// vec3
						//"Block.item[102].intensityDiffuse",					// vec3
						//"Block.item[102].intensitySpecular",					// vec3
						//"Block.item[102].lightType",
						//"Block.item[102].lightDirection",
						//"Block.item[102].anglesInnerOuter",
						//"Block.item[102].spotFalloff",
						//"Block.item[103].lightPosition",							// vec3
						//"Block.item[103].intensityAmbient",					// vec3
						//"Block.item[103].intensityDiffuse",					// vec3
						//"Block.item[103].intensitySpecular",					// vec3
						//"Block.item[103].lightType",
						//"Block.item[103].lightDirection",
						//"Block.item[103].anglesInnerOuter",
						//"Block.item[103].spotFalloff",
						//"Block.item[104].lightPosition",							// vec3
						//"Block.item[104].intensityAmbient",					// vec3
						//"Block.item[104].intensityDiffuse",					// vec3
						//"Block.item[104].intensitySpecular",					// vec3
						//"Block.item[104].lightType",
						//"Block.item[104].lightDirection",
						//"Block.item[104].anglesInnerOuter",
						//"Block.item[104].spotFalloff",
						//"Block.item[105].lightPosition",							// vec3
						//"Block.item[105].intensityAmbient",					// vec3
						//"Block.item[105].intensityDiffuse",					// vec3
						//"Block.item[105].intensitySpecular",					// vec3
						//"Block.item[105].lightType",
						//"Block.item[105].lightDirection",
						//"Block.item[105].anglesInnerOuter",
						//"Block.item[105].spotFalloff",
						//"Block.item[106].lightPosition",							// vec3
						//"Block.item[106].intensityAmbient",					// vec3
						//"Block.item[106].intensityDiffuse",					// vec3
						//"Block.item[106].intensitySpecular",					// vec3
						//"Block.item[106].lightType",
						//"Block.item[106].lightDirection",
						//"Block.item[106].anglesInnerOuter",
						//"Block.item[106].spotFalloff",
						//"Block.item[107].lightPosition",							// vec3
						//"Block.item[107].intensityAmbient",					// vec3
						//"Block.item[107].intensityDiffuse",					// vec3
						//"Block.item[107].intensitySpecular",					// vec3
						//"Block.item[107].lightType",
						//"Block.item[107].lightDirection",
						//"Block.item[107].anglesInnerOuter",
						//"Block.item[107].spotFalloff",
						//"Block.item[108].lightPosition",							// vec3
						//"Block.item[108].intensityAmbient",					// vec3
						//"Block.item[108].intensityDiffuse",					// vec3
						//"Block.item[108].intensitySpecular",					// vec3
						//"Block.item[108].lightType",
						//"Block.item[108].lightDirection",
						//"Block.item[108].anglesInnerOuter",
						//"Block.item[108].spotFalloff",
						//"Block.item[109].lightPosition",							// vec3
						//"Block.item[109].intensityAmbient",					// vec3
						//"Block.item[109].intensityDiffuse",					// vec3
						//"Block.item[109].intensitySpecular",					// vec3
						//"Block.item[109].lightType",
						//"Block.item[109].lightDirection",
						//"Block.item[109].anglesInnerOuter",
						//"Block.item[109].spotFalloff",
						//"Block.item[110].lightPosition",							// vec3
						//"Block.item[110].intensityAmbient",					// vec3
						//"Block.item[110].intensityDiffuse",					// vec3
						//"Block.item[110].intensitySpecular",					// vec3
						//"Block.item[110].lightType",
						//"Block.item[110].lightDirection",
						//"Block.item[110].anglesInnerOuter",
						//"Block.item[110].spotFalloff",
						//"Block.item[111].lightPosition",							// vec3
						//"Block.item[111].intensityAmbient",					// vec3
						//"Block.item[111].intensityDiffuse",					// vec3
						//"Block.item[111].intensitySpecular",					// vec3
						//"Block.item[111].lightType",
						//"Block.item[111].lightDirection",
						//"Block.item[111].anglesInnerOuter",
						//"Block.item[111].spotFalloff",
						//"Block.item[112].lightPosition",							// vec3
						//"Block.item[112].intensityAmbient",					// vec3
						//"Block.item[112].intensityDiffuse",					// vec3
						//"Block.item[112].intensitySpecular",					// vec3
						//"Block.item[112].lightType",
						//"Block.item[112].lightDirection",
						//"Block.item[112].anglesInnerOuter",
						//"Block.item[112].spotFalloff",
						//"Block.item[113].lightPosition",							// vec3
						//"Block.item[113].intensityAmbient",					// vec3
						//"Block.item[113].intensityDiffuse",					// vec3
						//"Block.item[113].intensitySpecular",					// vec3
						//"Block.item[113].lightType",
						//"Block.item[113].lightDirection",
						//"Block.item[113].anglesInnerOuter",
						//"Block.item[113].spotFalloff",
						//"Block.item[114].lightPosition",							// vec3
						//"Block.item[114].intensityAmbient",					// vec3
						//"Block.item[114].intensityDiffuse",					// vec3
						//"Block.item[114].intensitySpecular",					// vec3
						//"Block.item[114].lightType",
						//"Block.item[114].lightDirection",
						//"Block.item[114].anglesInnerOuter",
						//"Block.item[114].spotFalloff",
						//"Block.item[115].lightPosition",							// vec3
						//"Block.item[115].intensityAmbient",					// vec3
						//"Block.item[115].intensityDiffuse",					// vec3
						//"Block.item[115].intensitySpecular",					// vec3
						//"Block.item[115].lightType",
						//"Block.item[115].lightDirection",
						//"Block.item[115].anglesInnerOuter",
						//"Block.item[115].spotFalloff",
						//"Block.item[116].lightPosition",							// vec3
						//"Block.item[116].intensityAmbient",					// vec3
						//"Block.item[116].intensityDiffuse",					// vec3
						//"Block.item[116].intensitySpecular",					// vec3
						//"Block.item[116].lightType",
						//"Block.item[116].lightDirection",
						//"Block.item[116].anglesInnerOuter",
						//"Block.item[116].spotFalloff",
						//"Block.item[117].lightPosition",							// vec3
						//"Block.item[117].intensityAmbient",					// vec3
						//"Block.item[117].intensityDiffuse",					// vec3
						//"Block.item[117].intensitySpecular",					// vec3
						//"Block.item[117].lightType",
						//"Block.item[117].lightDirection",
						//"Block.item[117].anglesInnerOuter",
						//"Block.item[117].spotFalloff",
						//"Block.item[118].lightPosition",							// vec3
						//"Block.item[118].intensityAmbient",					// vec3
						//"Block.item[118].intensityDiffuse",					// vec3
						//"Block.item[118].intensitySpecular",					// vec3
						//"Block.item[118].lightType",
						//"Block.item[118].lightDirection",
						//"Block.item[118].anglesInnerOuter",
						//"Block.item[118].spotFalloff",
						//"Block.item[119].lightPosition",							// vec3
						//"Block.item[119].intensityAmbient",					// vec3
						//"Block.item[119].intensityDiffuse",					// vec3
						//"Block.item[119].intensitySpecular",					// vec3
						//"Block.item[119].lightType",
						//"Block.item[119].lightDirection",
						//"Block.item[119].anglesInnerOuter",
						//"Block.item[119].spotFalloff",
						//"Block.item[120].lightPosition",							// vec3
						//"Block.item[120].intensityAmbient",					// vec3
						//"Block.item[120].intensityDiffuse",					// vec3
						//"Block.item[120].intensitySpecular",					// vec3
						//"Block.item[120].lightType",
						//"Block.item[120].lightDirection",
						//"Block.item[120].anglesInnerOuter",
						//"Block.item[120].spotFalloff",
						//"Block.item[121].lightPosition",							// vec3
						//"Block.item[121].intensityAmbient",					// vec3
						//"Block.item[121].intensityDiffuse",					// vec3
						//"Block.item[121].intensitySpecular",					// vec3
						//"Block.item[121].lightType",
						//"Block.item[121].lightDirection",
						//"Block.item[121].anglesInnerOuter",
						//"Block.item[121].spotFalloff",
						//"Block.item[122].lightPosition",							// vec3
						//"Block.item[122].intensityAmbient",					// vec3
						//"Block.item[122].intensityDiffuse",					// vec3
						//"Block.item[122].intensitySpecular",					// vec3
						//"Block.item[122].lightType",
						//"Block.item[122].lightDirection",
						//"Block.item[122].anglesInnerOuter",
						//"Block.item[122].spotFalloff",
						//"Block.item[123].lightPosition",							// vec3
						//"Block.item[123].intensityAmbient",					// vec3
						//"Block.item[123].intensityDiffuse",					// vec3
						//"Block.item[123].intensitySpecular",					// vec3
						//"Block.item[123].lightType",
						//"Block.item[123].lightDirection",
						//"Block.item[123].anglesInnerOuter",
						//"Block.item[123].spotFalloff",
						//"Block.item[124].lightPosition",							// vec3
						//"Block.item[124].intensityAmbient",					// vec3
						//"Block.item[124].intensityDiffuse",					// vec3
						//"Block.item[124].intensitySpecular",					// vec3
						//"Block.item[124].lightType",
						//"Block.item[124].lightDirection",
						//"Block.item[124].anglesInnerOuter",
						//"Block.item[124].spotFalloff",
						//"Block.item[125].lightPosition",							// vec3
						//"Block.item[125].intensityAmbient",					// vec3
						//"Block.item[125].intensityDiffuse",					// vec3
						//"Block.item[125].intensitySpecular",					// vec3
						//"Block.item[125].lightType",
						//"Block.item[125].lightDirection",
						//"Block.item[125].anglesInnerOuter",
						//"Block.item[125].spotFalloff",
						//"Block.item[126].lightPosition",							// vec3
						//"Block.item[126].intensityAmbient",					// vec3
						//"Block.item[126].intensityDiffuse",					// vec3
						//"Block.item[126].intensitySpecular",					// vec3
						//"Block.item[126].lightType",
						//"Block.item[126].lightDirection",
						//"Block.item[126].anglesInnerOuter",
						//"Block.item[126].spotFalloff",
						//"Block.item[127].lightPosition",							// vec3
						//"Block.item[127].intensityAmbient",					// vec3
						//"Block.item[127].intensityDiffuse",					// vec3
						//"Block.item[127].intensitySpecular",					// vec3
						//"Block.item[127].lightType",
						//"Block.item[127].lightDirection",
						//"Block.item[127].anglesInnerOuter",
						//"Block.item[127].spotFalloff",
						//"Block.item[128].lightPosition",							// vec3
						//"Block.item[128].intensityAmbient",					// vec3
						//"Block.item[128].intensityDiffuse",					// vec3
						//"Block.item[128].intensitySpecular",					// vec3
						//"Block.item[128].lightType",
						//"Block.item[128].lightDirection",
						//"Block.item[128].anglesInnerOuter",
						//"Block.item[128].spotFalloff",
						//"Block.item[129].lightPosition",							// vec3
						//"Block.item[129].intensityAmbient",					// vec3
						//"Block.item[129].intensityDiffuse",					// vec3
						//"Block.item[129].intensitySpecular",					// vec3
						//"Block.item[129].lightType",
						//"Block.item[129].lightDirection",
						//"Block.item[129].anglesInnerOuter",
						//"Block.item[129].spotFalloff",
						//"Block.item[130].lightPosition",							// vec3
						//"Block.item[130].intensityAmbient",					// vec3
						//"Block.item[130].intensityDiffuse",					// vec3
						//"Block.item[130].intensitySpecular",					// vec3
						//"Block.item[130].lightType",
						//"Block.item[130].lightDirection",
						//"Block.item[130].anglesInnerOuter",
						//"Block.item[130].spotFalloff",
						//"Block.item[131].lightPosition",							// vec3
						//"Block.item[131].intensityAmbient",					// vec3
						//"Block.item[131].intensityDiffuse",					// vec3
						//"Block.item[131].intensitySpecular",					// vec3
						//"Block.item[131].lightType",
						//"Block.item[131].lightDirection",
						//"Block.item[131].anglesInnerOuter",
						//"Block.item[131].spotFalloff",
						//"Block.item[132].lightPosition",							// vec3
						//"Block.item[132].intensityAmbient",					// vec3
						//"Block.item[132].intensityDiffuse",					// vec3
						//"Block.item[132].intensitySpecular",					// vec3
						//"Block.item[132].lightType",
						//"Block.item[132].lightDirection",
						//"Block.item[132].anglesInnerOuter",
						//"Block.item[132].spotFalloff",
						//"Block.item[133].lightPosition",							// vec3
						//"Block.item[133].intensityAmbient",					// vec3
						//"Block.item[133].intensityDiffuse",					// vec3
						//"Block.item[133].intensitySpecular",					// vec3
						//"Block.item[133].lightType",
						//"Block.item[133].lightDirection",
						//"Block.item[133].anglesInnerOuter",
						//"Block.item[133].spotFalloff",
						//"Block.item[134].lightPosition",							// vec3
						//"Block.item[134].intensityAmbient",					// vec3
						//"Block.item[134].intensityDiffuse",					// vec3
						//"Block.item[134].intensitySpecular",					// vec3
						//"Block.item[134].lightType",
						//"Block.item[134].lightDirection",
						//"Block.item[134].anglesInnerOuter",
						//"Block.item[134].spotFalloff",
						//"Block.item[135].lightPosition",							// vec3
						//"Block.item[135].intensityAmbient",					// vec3
						//"Block.item[135].intensityDiffuse",					// vec3
						//"Block.item[135].intensitySpecular",					// vec3
						//"Block.item[135].lightType",
						//"Block.item[135].lightDirection",
						//"Block.item[135].anglesInnerOuter",
						//"Block.item[135].spotFalloff",
						//"Block.item[136].lightPosition",							// vec3
						//"Block.item[136].intensityAmbient",					// vec3
						//"Block.item[136].intensityDiffuse",					// vec3
						//"Block.item[136].intensitySpecular",					// vec3
						//"Block.item[136].lightType",
						//"Block.item[136].lightDirection",
						//"Block.item[136].anglesInnerOuter",
						//"Block.item[136].spotFalloff",
						//"Block.item[137].lightPosition",							// vec3
						//"Block.item[137].intensityAmbient",					// vec3
						//"Block.item[137].intensityDiffuse",					// vec3
						//"Block.item[137].intensitySpecular",					// vec3
						//"Block.item[137].lightType",
						//"Block.item[137].lightDirection",
						//"Block.item[137].anglesInnerOuter",
						//"Block.item[137].spotFalloff",
						//"Block.item[138].lightPosition",							// vec3
						//"Block.item[138].intensityAmbient",					// vec3
						//"Block.item[138].intensityDiffuse",					// vec3
						//"Block.item[138].intensitySpecular",					// vec3
						//"Block.item[138].lightType",
						//"Block.item[138].lightDirection",
						//"Block.item[138].anglesInnerOuter",
						//"Block.item[138].spotFalloff",
						//"Block.item[139].lightPosition",							// vec3
						//"Block.item[139].intensityAmbient",					// vec3
						//"Block.item[139].intensityDiffuse",					// vec3
						//"Block.item[139].intensitySpecular",					// vec3
						//"Block.item[139].lightType",
						//"Block.item[139].lightDirection",
						//"Block.item[139].anglesInnerOuter",
						//"Block.item[139].spotFalloff",
						//"Block.item[140].lightPosition",							// vec3
						//"Block.item[140].intensityAmbient",					// vec3
						//"Block.item[140].intensityDiffuse",					// vec3
						//"Block.item[140].intensitySpecular",					// vec3
						//"Block.item[140].lightType",
						//"Block.item[140].lightDirection",
						//"Block.item[140].anglesInnerOuter",
						//"Block.item[140].spotFalloff",
						//"Block.item[141].lightPosition",							// vec3
						//"Block.item[141].intensityAmbient",					// vec3
						//"Block.item[141].intensityDiffuse",					// vec3
						//"Block.item[141].intensitySpecular",					// vec3
						//"Block.item[141].lightType",
						//"Block.item[141].lightDirection",
						//"Block.item[141].anglesInnerOuter",
						//"Block.item[141].spotFalloff",
						//"Block.item[142].lightPosition",							// vec3
						//"Block.item[142].intensityAmbient",					// vec3
						//"Block.item[142].intensityDiffuse",					// vec3
						//"Block.item[142].intensitySpecular",					// vec3
						//"Block.item[142].lightType",
						//"Block.item[142].lightDirection",
						//"Block.item[142].anglesInnerOuter",
						//"Block.item[142].spotFalloff",
						//"Block.item[143].lightPosition",							// vec3
						//"Block.item[143].intensityAmbient",					// vec3
						//"Block.item[143].intensityDiffuse",					// vec3
						//"Block.item[143].intensitySpecular",					// vec3
						//"Block.item[143].lightType",
						//"Block.item[143].lightDirection",
						//"Block.item[143].anglesInnerOuter",
						//"Block.item[143].spotFalloff",
						//"Block.item[144].lightPosition",							// vec3
						//"Block.item[144].intensityAmbient",					// vec3
						//"Block.item[144].intensityDiffuse",					// vec3
						//"Block.item[144].intensitySpecular",					// vec3
						//"Block.item[144].lightType",
						//"Block.item[144].lightDirection",
						//"Block.item[144].anglesInnerOuter",
						//"Block.item[144].spotFalloff",
						//"Block.item[145].lightPosition",							// vec3
						//"Block.item[145].intensityAmbient",					// vec3
						//"Block.item[145].intensityDiffuse",					// vec3
						//"Block.item[145].intensitySpecular",					// vec3
						//"Block.item[145].lightType",
						//"Block.item[145].lightDirection",
						//"Block.item[145].anglesInnerOuter",
						//"Block.item[145].spotFalloff",
						//"Block.item[146].lightPosition",							// vec3
						//"Block.item[146].intensityAmbient",					// vec3
						//"Block.item[146].intensityDiffuse",					// vec3
						//"Block.item[146].intensitySpecular",					// vec3
						//"Block.item[146].lightType",
						//"Block.item[146].lightDirection",
						//"Block.item[146].anglesInnerOuter",
						//"Block.item[146].spotFalloff",
						//"Block.item[147].lightPosition",							// vec3
						//"Block.item[147].intensityAmbient",					// vec3
						//"Block.item[147].intensityDiffuse",					// vec3
						//"Block.item[147].intensitySpecular",					// vec3
						//"Block.item[147].lightType",
						//"Block.item[147].lightDirection",
						//"Block.item[147].anglesInnerOuter",
						//"Block.item[147].spotFalloff",
						//"Block.item[148].lightPosition",							// vec3
						//"Block.item[148].intensityAmbient",					// vec3
						//"Block.item[148].intensityDiffuse",					// vec3
						//"Block.item[148].intensitySpecular",					// vec3
						//"Block.item[148].lightType",
						//"Block.item[148].lightDirection",
						//"Block.item[148].anglesInnerOuter",
						//"Block.item[148].spotFalloff",
						//"Block.item[149].lightPosition",							// vec3
						//"Block.item[149].intensityAmbient",					// vec3
						//"Block.item[149].intensityDiffuse",					// vec3
						//"Block.item[149].intensitySpecular",					// vec3
						//"Block.item[149].lightType",
						//"Block.item[149].lightDirection",
						//"Block.item[149].anglesInnerOuter",
						//"Block.item[149].spotFalloff",
						//"Block.item[150].lightPosition",							// vec3
						//"Block.item[150].intensityAmbient",					// vec3
						//"Block.item[150].intensityDiffuse",					// vec3
						//"Block.item[150].intensitySpecular",					// vec3
						//"Block.item[150].lightType",
						//"Block.item[150].lightDirection",
						//"Block.item[150].anglesInnerOuter",
						//"Block.item[150].spotFalloff",
						//"Block.item[151].lightPosition",							// vec3
						//"Block.item[151].intensityAmbient",					// vec3
						//"Block.item[151].intensityDiffuse",					// vec3
						//"Block.item[151].intensitySpecular",					// vec3
						//"Block.item[151].lightType",
						//"Block.item[151].lightDirection",
						//"Block.item[151].anglesInnerOuter",
						//"Block.item[151].spotFalloff",
						//"Block.item[152].lightPosition",							// vec3
						//"Block.item[152].intensityAmbient",					// vec3
						//"Block.item[152].intensityDiffuse",					// vec3
						//"Block.item[152].intensitySpecular",					// vec3
						//"Block.item[152].lightType",
						//"Block.item[152].lightDirection",
						//"Block.item[152].anglesInnerOuter",
						//"Block.item[152].spotFalloff",
						//"Block.item[153].lightPosition",							// vec3
						//"Block.item[153].intensityAmbient",					// vec3
						//"Block.item[153].intensityDiffuse",					// vec3
						//"Block.item[153].intensitySpecular",					// vec3
						//"Block.item[153].lightType",
						//"Block.item[153].lightDirection",
						//"Block.item[153].anglesInnerOuter",
						//"Block.item[153].spotFalloff",
						//"Block.item[154].lightPosition",							// vec3
						//"Block.item[154].intensityAmbient",					// vec3
						//"Block.item[154].intensityDiffuse",					// vec3
						//"Block.item[154].intensitySpecular",					// vec3
						//"Block.item[154].lightType",
						//"Block.item[154].lightDirection",
						//"Block.item[154].anglesInnerOuter",
						//"Block.item[154].spotFalloff",
						//"Block.item[155].lightPosition",							// vec3
						//"Block.item[155].intensityAmbient",					// vec3
						//"Block.item[155].intensityDiffuse",					// vec3
						//"Block.item[155].intensitySpecular",					// vec3
						//"Block.item[155].lightType",
						//"Block.item[155].lightDirection",
						//"Block.item[155].anglesInnerOuter",
						//"Block.item[155].spotFalloff",
						//"Block.item[156].lightPosition",							// vec3
						//"Block.item[156].intensityAmbient",					// vec3
						//"Block.item[156].intensityDiffuse",					// vec3
						//"Block.item[156].intensitySpecular",					// vec3
						//"Block.item[156].lightType",
						//"Block.item[156].lightDirection",
						//"Block.item[156].anglesInnerOuter",
						//"Block.item[156].spotFalloff",
						//"Block.item[157].lightPosition",							// vec3
						//"Block.item[157].intensityAmbient",					// vec3
						//"Block.item[157].intensityDiffuse",					// vec3
						//"Block.item[157].intensitySpecular",					// vec3
						//"Block.item[157].lightType",
						//"Block.item[157].lightDirection",
						//"Block.item[157].anglesInnerOuter",
						//"Block.item[157].spotFalloff",
						//"Block.item[158].lightPosition",							// vec3
						//"Block.item[158].intensityAmbient",					// vec3
						//"Block.item[158].intensityDiffuse",					// vec3
						//"Block.item[158].intensitySpecular",					// vec3
						//"Block.item[158].lightType",
						//"Block.item[158].lightDirection",
						//"Block.item[158].anglesInnerOuter",
						//"Block.item[158].spotFalloff",
						//"Block.item[159].lightPosition",							// vec3
						//"Block.item[159].intensityAmbient",					// vec3
						//"Block.item[159].intensityDiffuse",					// vec3
						//"Block.item[159].intensitySpecular",					// vec3
						//"Block.item[159].lightType",
						//"Block.item[159].lightDirection",
						//"Block.item[159].anglesInnerOuter",
						//"Block.item[159].spotFalloff",
						//"Block.item[160].lightPosition",							// vec3
						//"Block.item[160].intensityAmbient",					// vec3
						//"Block.item[160].intensityDiffuse",					// vec3
						//"Block.item[160].intensitySpecular",					// vec3
						//"Block.item[160].lightType",
						//"Block.item[160].lightDirection",
						//"Block.item[160].anglesInnerOuter",
						//"Block.item[160].spotFalloff",
						//"Block.item[161].lightPosition",							// vec3
						//"Block.item[161].intensityAmbient",					// vec3
						//"Block.item[161].intensityDiffuse",					// vec3
						//"Block.item[161].intensitySpecular",					// vec3
						//"Block.item[161].lightType",
						//"Block.item[161].lightDirection",
						//"Block.item[161].anglesInnerOuter",
						//"Block.item[161].spotFalloff",
						//"Block.item[162].lightPosition",							// vec3
						//"Block.item[162].intensityAmbient",					// vec3
						//"Block.item[162].intensityDiffuse",					// vec3
						//"Block.item[162].intensitySpecular",					// vec3
						//"Block.item[162].lightType",
						//"Block.item[162].lightDirection",
						//"Block.item[162].anglesInnerOuter",
						//"Block.item[162].spotFalloff",
						//"Block.item[163].lightPosition",							// vec3
						//"Block.item[163].intensityAmbient",					// vec3
						//"Block.item[163].intensityDiffuse",					// vec3
						//"Block.item[163].intensitySpecular",					// vec3
						//"Block.item[163].lightType",
						//"Block.item[163].lightDirection",
						//"Block.item[163].anglesInnerOuter",
						//"Block.item[163].spotFalloff",
						//"Block.item[164].lightPosition",							// vec3
						//"Block.item[164].intensityAmbient",					// vec3
						//"Block.item[164].intensityDiffuse",					// vec3
						//"Block.item[164].intensitySpecular",					// vec3
						//"Block.item[164].lightType",
						//"Block.item[164].lightDirection",
						//"Block.item[164].anglesInnerOuter",
						//"Block.item[164].spotFalloff",
						//"Block.item[165].lightPosition",							// vec3
						//"Block.item[165].intensityAmbient",					// vec3
						//"Block.item[165].intensityDiffuse",					// vec3
						//"Block.item[165].intensitySpecular",					// vec3
						//"Block.item[165].lightType",
						//"Block.item[165].lightDirection",
						//"Block.item[165].anglesInnerOuter",
						//"Block.item[165].spotFalloff",
						//"Block.item[166].lightPosition",							// vec3
						//"Block.item[166].intensityAmbient",					// vec3
						//"Block.item[166].intensityDiffuse",					// vec3
						//"Block.item[166].intensitySpecular",					// vec3
						//"Block.item[166].lightType",
						//"Block.item[166].lightDirection",
						//"Block.item[166].anglesInnerOuter",
						//"Block.item[166].spotFalloff",
						//"Block.item[167].lightPosition",							// vec3
						//"Block.item[167].intensityAmbient",					// vec3
						//"Block.item[167].intensityDiffuse",					// vec3
						//"Block.item[167].intensitySpecular",					// vec3
						//"Block.item[167].lightType",
						//"Block.item[167].lightDirection",
						//"Block.item[167].anglesInnerOuter",
						//"Block.item[167].spotFalloff",
						//"Block.item[168].lightPosition",							// vec3
						//"Block.item[168].intensityAmbient",					// vec3
						//"Block.item[168].intensityDiffuse",					// vec3
						//"Block.item[168].intensitySpecular",					// vec3
						//"Block.item[168].lightType",
						//"Block.item[168].lightDirection",
						//"Block.item[168].anglesInnerOuter",
						//"Block.item[168].spotFalloff",
						//"Block.item[169].lightPosition",							// vec3
						//"Block.item[169].intensityAmbient",					// vec3
						//"Block.item[169].intensityDiffuse",					// vec3
						//"Block.item[169].intensitySpecular",					// vec3
						//"Block.item[169].lightType",
						//"Block.item[169].lightDirection",
						//"Block.item[169].anglesInnerOuter",
						//"Block.item[169].spotFalloff",
						//"Block.item[170].lightPosition",							// vec3
						//"Block.item[170].intensityAmbient",					// vec3
						//"Block.item[170].intensityDiffuse",					// vec3
						//"Block.item[170].intensitySpecular",					// vec3
						//"Block.item[170].lightType",
						//"Block.item[170].lightDirection",
						//"Block.item[170].anglesInnerOuter",
						//"Block.item[170].spotFalloff",
						//"Block.item[171].lightPosition",							// vec3
						//"Block.item[171].intensityAmbient",					// vec3
						//"Block.item[171].intensityDiffuse",					// vec3
						//"Block.item[171].intensitySpecular",					// vec3
						//"Block.item[171].lightType",
						//"Block.item[171].lightDirection",
						//"Block.item[171].anglesInnerOuter",
						//"Block.item[171].spotFalloff",
						//"Block.item[172].lightPosition",							// vec3
						//"Block.item[172].intensityAmbient",					// vec3
						//"Block.item[172].intensityDiffuse",					// vec3
						//"Block.item[172].intensitySpecular",					// vec3
						//"Block.item[172].lightType",
						//"Block.item[172].lightDirection",
						//"Block.item[172].anglesInnerOuter",
						//"Block.item[172].spotFalloff",
						//"Block.item[173].lightPosition",							// vec3
						//"Block.item[173].intensityAmbient",					// vec3
						//"Block.item[173].intensityDiffuse",					// vec3
						//"Block.item[173].intensitySpecular",					// vec3
						//"Block.item[173].lightType",
						//"Block.item[173].lightDirection",
						//"Block.item[173].anglesInnerOuter",
						//"Block.item[173].spotFalloff",
						//"Block.item[174].lightPosition",							// vec3
						//"Block.item[174].intensityAmbient",					// vec3
						//"Block.item[174].intensityDiffuse",					// vec3
						//"Block.item[174].intensitySpecular",					// vec3
						//"Block.item[174].lightType",
						//"Block.item[174].lightDirection",
						//"Block.item[174].anglesInnerOuter",
						//"Block.item[174].spotFalloff",
						//"Block.item[175].lightPosition",							// vec3
						//"Block.item[175].intensityAmbient",					// vec3
						//"Block.item[175].intensityDiffuse",					// vec3
						//"Block.item[175].intensitySpecular",					// vec3
						//"Block.item[175].lightType",
						//"Block.item[175].lightDirection",
						//"Block.item[175].anglesInnerOuter",
						//"Block.item[175].spotFalloff",
						//"Block.item[176].lightPosition",							// vec3
						//"Block.item[176].intensityAmbient",					// vec3
						//"Block.item[176].intensityDiffuse",					// vec3
						//"Block.item[176].intensitySpecular",					// vec3
						//"Block.item[176].lightType",
						//"Block.item[176].lightDirection",
						//"Block.item[176].anglesInnerOuter",
						//"Block.item[176].spotFalloff",
						//"Block.item[177].lightPosition",							// vec3
						//"Block.item[177].intensityAmbient",					// vec3
						//"Block.item[177].intensityDiffuse",					// vec3
						//"Block.item[177].intensitySpecular",					// vec3
						//"Block.item[177].lightType",
						//"Block.item[177].lightDirection",
						//"Block.item[177].anglesInnerOuter",
						//"Block.item[177].spotFalloff",
						//"Block.item[178].lightPosition",							// vec3
						//"Block.item[178].intensityAmbient",					// vec3
						//"Block.item[178].intensityDiffuse",					// vec3
						//"Block.item[178].intensitySpecular",					// vec3
						//"Block.item[178].lightType",
						//"Block.item[178].lightDirection",
						//"Block.item[178].anglesInnerOuter",
						//"Block.item[178].spotFalloff",
						//"Block.item[179].lightPosition",							// vec3
						//"Block.item[179].intensityAmbient",					// vec3
						//"Block.item[179].intensityDiffuse",					// vec3
						//"Block.item[179].intensitySpecular",					// vec3
						//"Block.item[179].lightType",
						//"Block.item[179].lightDirection",
						//"Block.item[179].anglesInnerOuter",
						//"Block.item[179].spotFalloff",
						//"Block.item[180].lightPosition",							// vec3
						//"Block.item[180].intensityAmbient",					// vec3
						//"Block.item[180].intensityDiffuse",					// vec3
						//"Block.item[180].intensitySpecular",					// vec3
						//"Block.item[180].lightType",
						//"Block.item[180].lightDirection",
						//"Block.item[180].anglesInnerOuter",
						//"Block.item[180].spotFalloff",
						//"Block.item[181].lightPosition",							// vec3
						//"Block.item[181].intensityAmbient",					// vec3
						//"Block.item[181].intensityDiffuse",					// vec3
						//"Block.item[181].intensitySpecular",					// vec3
						//"Block.item[181].lightType",
						//"Block.item[181].lightDirection",
						//"Block.item[181].anglesInnerOuter",
						//"Block.item[181].spotFalloff",
						//"Block.item[182].lightPosition",							// vec3
						//"Block.item[182].intensityAmbient",					// vec3
						//"Block.item[182].intensityDiffuse",					// vec3
						//"Block.item[182].intensitySpecular",					// vec3
						//"Block.item[182].lightType",
						//"Block.item[182].lightDirection",
						//"Block.item[182].anglesInnerOuter",
						//"Block.item[182].spotFalloff",
						//"Block.item[183].lightPosition",							// vec3
						//"Block.item[183].intensityAmbient",					// vec3
						//"Block.item[183].intensityDiffuse",					// vec3
						//"Block.item[183].intensitySpecular",					// vec3
						//"Block.item[183].lightType",
						//"Block.item[183].lightDirection",
						//"Block.item[183].anglesInnerOuter",
						//"Block.item[183].spotFalloff",
						//"Block.item[184].lightPosition",							// vec3
						//"Block.item[184].intensityAmbient",					// vec3
						//"Block.item[184].intensityDiffuse",					// vec3
						//"Block.item[184].intensitySpecular",					// vec3
						//"Block.item[184].lightType",
						//"Block.item[184].lightDirection",
						//"Block.item[184].anglesInnerOuter",
						//"Block.item[184].spotFalloff",
						//"Block.item[185].lightPosition",							// vec3
						//"Block.item[185].intensityAmbient",					// vec3
						//"Block.item[185].intensityDiffuse",					// vec3
						//"Block.item[185].intensitySpecular",					// vec3
						//"Block.item[185].lightType",
						//"Block.item[185].lightDirection",
						//"Block.item[185].anglesInnerOuter",
						//"Block.item[185].spotFalloff",
						//"Block.item[186].lightPosition",							// vec3
						//"Block.item[186].intensityAmbient",					// vec3
						//"Block.item[186].intensityDiffuse",					// vec3
						//"Block.item[186].intensitySpecular",					// vec3
						//"Block.item[186].lightType",
						//"Block.item[186].lightDirection",
						//"Block.item[186].anglesInnerOuter",
						//"Block.item[186].spotFalloff",
						//"Block.item[187].lightPosition",							// vec3
						//"Block.item[187].intensityAmbient",					// vec3
						//"Block.item[187].intensityDiffuse",					// vec3
						//"Block.item[187].intensitySpecular",					// vec3
						//"Block.item[187].lightType",
						//"Block.item[187].lightDirection",
						//"Block.item[187].anglesInnerOuter",
						//"Block.item[187].spotFalloff",
						//"Block.item[188].lightPosition",							// vec3
						//"Block.item[188].intensityAmbient",					// vec3
						//"Block.item[188].intensityDiffuse",					// vec3
						//"Block.item[188].intensitySpecular",					// vec3
						//"Block.item[188].lightType",
						//"Block.item[188].lightDirection",
						//"Block.item[188].anglesInnerOuter",
						//"Block.item[188].spotFalloff",
						//"Block.item[189].lightPosition",							// vec3
						//"Block.item[189].intensityAmbient",					// vec3
						//"Block.item[189].intensityDiffuse",					// vec3
						//"Block.item[189].intensitySpecular",					// vec3
						//"Block.item[189].lightType",
						//"Block.item[189].lightDirection",
						//"Block.item[189].anglesInnerOuter",
						//"Block.item[189].spotFalloff",
						//"Block.item[190].lightPosition",							// vec3
						//"Block.item[190].intensityAmbient",					// vec3
						//"Block.item[190].intensityDiffuse",					// vec3
						//"Block.item[190].intensitySpecular",					// vec3
						//"Block.item[190].lightType",
						//"Block.item[190].lightDirection",
						//"Block.item[190].anglesInnerOuter",
						//"Block.item[190].spotFalloff",
						//"Block.item[191].lightPosition",							// vec3
						//"Block.item[191].intensityAmbient",					// vec3
						//"Block.item[191].intensityDiffuse",					// vec3
						//"Block.item[191].intensitySpecular",					// vec3
						//"Block.item[191].lightType",
						//"Block.item[191].lightDirection",
						//"Block.item[191].anglesInnerOuter",
						//"Block.item[191].spotFalloff",
						//"Block.item[192].lightPosition",							// vec3
						//"Block.item[192].intensityAmbient",					// vec3
						//"Block.item[192].intensityDiffuse",					// vec3
						//"Block.item[192].intensitySpecular",					// vec3
						//"Block.item[192].lightType",
						//"Block.item[192].lightDirection",
						//"Block.item[192].anglesInnerOuter",
						//"Block.item[192].spotFalloff",
						//"Block.item[193].lightPosition",							// vec3
						//"Block.item[193].intensityAmbient",					// vec3
						//"Block.item[193].intensityDiffuse",					// vec3
						//"Block.item[193].intensitySpecular",					// vec3
						//"Block.item[193].lightType",
						//"Block.item[193].lightDirection",
						//"Block.item[193].anglesInnerOuter",
						//"Block.item[193].spotFalloff",
						//"Block.item[194].lightPosition",							// vec3
						//"Block.item[194].intensityAmbient",					// vec3
						//"Block.item[194].intensityDiffuse",					// vec3
						//"Block.item[194].intensitySpecular",					// vec3
						//"Block.item[194].lightType",
						//"Block.item[194].lightDirection",
						//"Block.item[194].anglesInnerOuter",
						//"Block.item[194].spotFalloff",
						//"Block.item[195].lightPosition",							// vec3
						//"Block.item[195].intensityAmbient",					// vec3
						//"Block.item[195].intensityDiffuse",					// vec3
						//"Block.item[195].intensitySpecular",					// vec3
						//"Block.item[195].lightType",
						//"Block.item[195].lightDirection",
						//"Block.item[195].anglesInnerOuter",
						//"Block.item[195].spotFalloff",
						//"Block.item[196].lightPosition",							// vec3
						//"Block.item[196].intensityAmbient",					// vec3
						//"Block.item[196].intensityDiffuse",					// vec3
						//"Block.item[196].intensitySpecular",					// vec3
						//"Block.item[196].lightType",
						//"Block.item[196].lightDirection",
						//"Block.item[196].anglesInnerOuter",
						//"Block.item[196].spotFalloff",
						//"Block.item[197].lightPosition",							// vec3
						//"Block.item[197].intensityAmbient",					// vec3
						//"Block.item[197].intensityDiffuse",					// vec3
						//"Block.item[197].intensitySpecular",					// vec3
						//"Block.item[197].lightType",
						//"Block.item[197].lightDirection",
						//"Block.item[197].anglesInnerOuter",
						//"Block.item[197].spotFalloff",
						//"Block.item[198].lightPosition",							// vec3
						//"Block.item[198].intensityAmbient",					// vec3
						//"Block.item[198].intensityDiffuse",					// vec3
						//"Block.item[198].intensitySpecular",					// vec3
						//"Block.item[198].lightType",
						//"Block.item[198].lightDirection",
						//"Block.item[198].anglesInnerOuter",
						//"Block.item[198].spotFalloff",
						//"Block.item[199].lightPosition",							// vec3
						//"Block.item[199].intensityAmbient",					// vec3
						//"Block.item[199].intensityDiffuse",					// vec3
						//"Block.item[199].intensitySpecular",					// vec3
						//"Block.item[199].lightType",
						//"Block.item[199].lightDirection",
						//"Block.item[199].anglesInnerOuter",
						//"Block.item[199].spotFalloff",
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
