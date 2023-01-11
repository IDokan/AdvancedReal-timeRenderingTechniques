/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: describe overview of my application
Language: English none of computer language is used in here.
Platform: Compiler version: Compiler does not need for README, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 12/3/2021
End Header --------------------------------------------------------*/


1. How to use my interface.
1-a. 
WASD to move camera.
During move, press left shift to move faster.
Press right mouse button to look around.
Press E to move up
Press Q to move down

In MyGUI window,
use Normals tab to toggle to display vertex, and face normals
use Hybrid Rendering Debuggings to see individual render targets on FSQ and test depth copying.


2. How to use the application
2-a. Do not run in DEBUG mode(Pushpak confirmed). Use it in Release mode only.

3. Completed assignment parts
3-a. Everything perfect. 
Objects loaded correctly from the files and displayed in proper position
G-buffer populated correctly
Lighting pass setup for FSQ
Implementation of lighting pass with Phong Shading
Rendering vertex/face normals using forward rendering 
Camera movement (WASD)
GUI option to toggle depth copying
Ability to visualize individual render targets on FSQ


4. Which part has NOT been completed
4-a. Everything has been done.

5. Where the relevant source codes.
5-a. Assignment1 folder contains the scene code of this assignment.
5-b. Common contains basic asset sources such as , 
5-b-3. shaders/HybridRendering/ folder contains shader files. In this assignment, I created shaders/HybridRendering/A
5-b-4. Neshes/models folder contains Assimp loading source files.
5-b-5. Textures folder contains texture source files
5-b-6. Lights folder contains light source files
5-b-7. FBO folder contains FBO source files.

6. Where my program is tested
6-a. OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015

7. How many I spend
