// Trevor Walden, IGME 209 Section 2, 17 March 2016
// This file contains the code for the ShaderManager class, which handles shader loading and use

#include "ShaderManager.h"
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <fstream>
#include <iostream>
using namespace std;

ShaderManager::ShaderManager(void)
{
}

GLuint ShaderManager::getProgram(void) const // Getter for program
{
	return program;
}

GLuint ShaderManager::loadShader(const char* file, GLenum shaderType) // Helps load shader by reading and compiling; returns index of stored shader
{
	ifstream binaryReader(file, ios::binary); // Create input stream and open the file

	if (!binaryReader.is_open()) // If file opening not successful, return failure
	{
		return 0;
	}

	binaryReader.seekg(0, ios::end); // Find the size of the file
	int size = (int)binaryReader.tellg();

	binaryReader.seekg(0, ios::beg); // Move back to beginning of file

	char* shaderFile = new char[size + 1]; // Array to put the file in

	binaryReader.read(shaderFile, size); // Put file contents in array
	shaderFile[size] = 0; // Add null terminator

	binaryReader.close(); // Close the file

	GLuint shader = glCreateShader(shaderType); // Create new shader
	glShaderSource(shader, 1, &shaderFile, 0); // Set shader source: location, # of strings, char array pointer, array of string sizes (0 is null terminated)
	glCompileShader(shader); // Compile the shader
	delete[] shaderFile; // Delete the character array, it's not needed anymore

	GLint status; // Holds compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // Check if shader compiled correctly
	if (status != 0) // If compile didn't fail, return the shader
	{
		return shader;
	}
	else // Otherwise, find out why compile failed
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size); // Size is repurposed to hold the length of the info log
		GLchar* log = new GLchar[size]; // Create array to hold info log
		glGetShaderInfoLog(shader, size, 0, log); // Get the info log
		cout << log; // Print the log

		delete[] log; // Delete created data and return failure
		glDeleteShader(shader);
		return 0;
	}
}

bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile) // Loads shaders
{
	GLuint vShader = loadShader(vertexFile, GL_VERTEX_SHADER); // Compile the vertex shader
	if (vShader == 0) // Return failure if compile fails
	{
		return 0;
	}

	GLuint fShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER); // Compile the fragment shader
	if (fShader == 0) // Return failure if compile fails
	{
		return 0;
	}

	program = glCreateProgram(); // Create the shader program
	glAttachShader(program, vShader); // Add the shaders
	glAttachShader(program, fShader);
	glLinkProgram(program); // Link the program

	GLint status; // Holds status of program linking
	glGetProgramiv(program, GL_LINK_STATUS, &status); // Check if linked correctly
	if (status != 0) // If it linked correctly return success
	{
		return true;
	}
	else // Otherwise, find out why linking failed
	{
		int size;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size); // Find the length of the log
		GLchar* log = new GLchar[size]; // Create array to hold info log
		glGetProgramInfoLog(program, size, 0, log); // Get the info log
		cout << log; // Print the log

		delete[] log; // Delete created data and return failure
		glDeleteProgram(program);
		return false;
	}
}

ShaderManager::~ShaderManager(void)
{
}
