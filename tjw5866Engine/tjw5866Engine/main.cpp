// Trevor Walden, IGME 209 Section 2, 17 March 2016
// This is an exercise in learning OpenGL and shaders (GLSL)

#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include <iostream>
#include "Engine.h"

int main(void)
{
	Engine engine = Engine();

	if (!engine.init()) // Initialize and end if failure
	{
		return -1;
	}

	if (engine.useShaders())
	{
		engine.gameLoop(); // Draw to the screen
	}

	std::cin.get();
	glfwTerminate(); // Safely exit engine
	return 0;
}