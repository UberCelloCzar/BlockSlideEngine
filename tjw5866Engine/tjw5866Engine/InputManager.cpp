// Trevor Walden, IGME 209 Section 2, 29 April 2016
// This file contains the definitions for the input handling functions

#include "InputManager.h"
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <map>

std::map<int, bool> KeyIsDown; // Int is key, bool is condition
std::map<int, bool> KeyWasDown;

void UpdateInput(void) // Update input
{
	KeyWasDown = KeyIsDown;
}

bool keyWasDown(int key) // Return last state of key
{
	return KeyWasDown[key];
}

bool keyIsDown(int key) // Return current state of key
{
	return KeyIsDown[key];
}

void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods) // Do action on click
{
	KeyIsDown[button] = (bool)action; // What is this for?
}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) // Do action on button press
{
	KeyIsDown[key] = (bool)action; // Still don't know what this does
}