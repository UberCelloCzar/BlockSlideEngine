// Trevor Walden, IGME 209 Section 2, 17 March 2016
// This file contains the declarations for the ShaderManager class, which handles shader loading and use

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

class ShaderManager
{
public:
	ShaderManager(void);

	GLuint getProgram(void) const; // Getter for program
	bool loadShaders(const char* vertexFile, const char* fragmentFile); // Loads shaders

	~ShaderManager(void);

private:
	GLuint program; // Index of loaded shader program
	GLuint loadShader(const char* file, GLenum shaderType); // Helps load shader by reading and compiling; returns index of stored shader
};

