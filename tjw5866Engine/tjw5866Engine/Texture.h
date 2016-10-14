// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the declarations for the functions and variables need for a texture

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture(void);

	bool Texture::loadTexture(char* fileName); // LoadTexture method and filename:texid map
	GLuint id;

	~Texture(void);
};

