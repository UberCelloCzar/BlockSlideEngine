// Trevor Walden, IGME 209 Section 2, 17 March 2016
// This file contains the declarations for the functions in the Engine class that handles openGL and screen drawing

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include "ShaderManager.h"
#include <map>
#include "Texture.h"
#include "GameObject.h"
#include "Camera.h"

class Engine
{
public:
	Engine(void);

	bool init(void); // Initialize GLFW and GLEW
	bool gameLoop(void); // Game loop, actually draws to the screen
	bool useShaders(void); // Calls for creation of shaders and uses them

	~Engine(void);

private:
	GLFWwindow* GLFWwindowPtr; // Store pointer for window

	ShaderManager shaders; // Will handle shader creation
	double currFrameT; // Variables to keep track of frame times
	double prevFrameT;
	double deltaT;
	glm::vec3 vForce; // Multipurpose force vector
	std::map<char*, Texture> textures; // Map of names to textures
	GameObject gameObjects[17]; // Objects in the scene
	Camera camera; // Camera that views the scene

};

