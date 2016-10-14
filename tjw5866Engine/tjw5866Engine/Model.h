// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the declarations for the functions and variables that make up a game object's model and texture

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Model
{
public:
	Model(void); // Unparameterized constructor
	Model(GLuint pTexture); // Parameterized constructor sets texture


	GLuint vertBuff;
	bool buffer(char* objFile); // Read in the model's data
	GLuint texture; // Index of the model's texture
	void Draw(void); // Draws the model to the screen

	~Model(void);
private:
	unsigned int vertCount; // Will hold number of vertices to be used (3 for each tri)
	GLuint vertArr; // Hold the numbers identifying the array for the background and model
};

