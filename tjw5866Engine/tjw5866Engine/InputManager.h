// Trevor Walden, IGME 209 Section 2, 29 April 2016
// This file contains the declarations for the input handling functions

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>

void UpdateInput(void); // Updates inputs

bool keyWasDown(int key); // Return requested button states
bool keyIsDown(int key);

void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods); // Do action on click
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods); // Do action on button press