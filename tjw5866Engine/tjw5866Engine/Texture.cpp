// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the definitions for the functions and variables need for a texture

#include "Texture.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>


Texture::Texture(void)
{
}

bool Texture::loadTexture(char* fileName) // Load image file from file to video card
{
	glGenTextures(1, &id); // Generate texture space


	FIBITMAP* imageFile = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName); // Load image file
	if (imageFile == nullptr)
	{
		return false; // Break if loading fails
	}
	FIBITMAP* image = FreeImage_ConvertTo32Bits(imageFile); // Convert to 32 bit then unload original
	FreeImage_Unload(imageFile);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image)); // Load to the video card
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Switch from mipmap to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND); // Don't quite know how this works, but it lets the texture have transparency somehow
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FreeImage_Unload(image); // No longer need original
	return true;
}


Texture::~Texture(void)
{
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind
	glDeleteTextures(1, &id); // Delete textures
}
