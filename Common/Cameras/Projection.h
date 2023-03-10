/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Projection.h
Purpose: Projection calculate codes
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/30/2021
End Header --------------------------------------------------------*/

#ifndef CS250_PROJECTION_H
#define CS250_PROJECTION_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat4x4.hpp>

struct Affine;
struct Matrix;
class Camera;

Affine CameraToWorld(const Camera& cam);
Affine WorldToCamera(const Camera& cam);
Matrix CameraToNDC(const Camera& cam);
//Hcoord deviceToWorld(SDL_Window* window, Hcoord clickedPoint, Camera camera);

glm::mat4 GetWorldToNDC(const Camera& cam);
#endif

