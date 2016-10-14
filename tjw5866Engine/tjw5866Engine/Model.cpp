// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the definitions for the functions and variables that make up a game object's model

#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

struct Vertex // Struct to hold vertex data
{
	glm::vec3 loc; // Vertex position
	glm::vec2 uv; // Vertex texture coords
	glm::vec3 norm; // Vertex normal coords
};

struct vertInd // Struct to hold vertex index data
{
	unsigned int locInd, uvInd, normInd;
};

Model::Model(void) // Unparameterized constructor
{
	texture = NULL; 
	vertArr = 0;
	vertCount = 0;
}

Model::Model(GLuint pTexture) // Parameterized constructior sets texture
{
	texture = pTexture;
	vertArr = 0;
	vertCount = 0;
}

bool Model::buffer(char* objFile) // Bring in the model
{
	// Create stuff to draw stuff
	std::vector<glm::vec3> locs;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;
	std::vector<vertInd> vertInds;

	/* Read the file */
	char* fileContents; // Holds the string of the file
	std::ifstream fileStream(objFile, std::ios::binary); // Create stream and open file
	if (fileStream.is_open())
	{
		fileStream.seekg(0, std::ios::end); // Count to the end
		int size = (int)fileStream.tellg(); // Give the count
		fileStream.seekg(0, std::ios::beg); // Go back to beginning

		fileContents = new char[size + 1]; // Holder for the file + terminator
		fileStream.read(fileContents, size); // Read in the file
		fileContents[size] = 0; // Add terminator
		fileStream.close(); // Close the file
	}
	else
	{
		return false;
	}
	
	/* Parse the file */
	glm::vec3 nums; // Holds numbers
	char chr; // Holds throwaway chars
	unsigned int locIndex, uvIndex, normIndex; // Hold values for indexes

	std::stringstream fileString(fileContents); // Turn the file string into a stream

	std::string line;
	std::string id; 
	while (std::getline(fileString, line)) // Get each line from the stream
	{
		std::stringstream ss(line);
		ss >> id;
		if (id == "vt") // UV
		{
			ss >> nums[0] >> nums[1]; // If it's a uv, get the values and sick 'em in
			uvs.push_back(glm::vec2(nums[0], nums[1]));
		}
		else if (id == "vn") // Normal
		{
			ss >> nums[0] >> nums[1] >> nums[2]; // If it's a normal, get the values and stick 'em in
			norms.push_back(nums);
		}
		else if (id == "v") // Vertex
		{
			ss >> nums[0] >> nums[1] >> nums[2]; // If it's a vertex, get the values and stick 'em in
			locs.push_back(nums);
		}
		else if (id == "f") // Indexes
		{
			for (int j = 0; j < 3; j++) // Put all 3 sets of 3 indexes into the data
			{
				ss >> locIndex >> chr >> uvIndex >> chr >> normIndex;
				--locIndex; // Start counts at 0
				--uvIndex;
				--normIndex;
				vertInds.push_back({ locIndex, uvIndex, normIndex }); // Put 'em in
				++vertCount; // Count the vertexes
			}
		}
	}
	delete[] fileContents;

	/* Upload the data */
	std::vector<Vertex> vertBufData(vertInds.size()); // Duplicate vertices into a single buffer
	glGenVertexArrays(1, &vertArr); // Generate array and buffer and put indices in variables
	glGenBuffers(1, &vertBuff);

	for (unsigned int i = 0; i < vertInds.size(); i++)
	{
		vertBufData[i] = { locs[vertInds[i].locInd], uvs[vertInds[i].uvInd], norms[vertInds[i].normInd] };
	}

	glBindVertexArray(vertArr); // Bind the array and buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertBuff);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertInds.size(), &vertBufData[0], GL_STATIC_DRAW); // Copy data to buffer: Destination, byte size, source, OGL setting

	glEnableVertexAttribArray(0); // Attribute index 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Attrib index, dimensions, datatype, normalize?, bytes per vertex, offset

	glEnableVertexAttribArray(1); // Attribute index 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3)); // Attrib index, dimensions, datatype, normalize?, bytes per vertex, offset

	glEnableVertexAttribArray(2); // Attribute index 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)+sizeof(glm::vec3))); // Attrib index, dimensions, datatype, normalize?, bytes per vertex, offset
	glBindVertexArray(0); // Unbind when finished editing

	return true; // Return success
}

void Model::Draw(void) // Draws the model to the screen
{
	glBindVertexArray(vertArr); // Render model
	glBindTexture(GL_TEXTURE_2D, texture); // Bind the model's texture
	glDrawArrays(GL_TRIANGLES, 0, vertCount); // Draw the model
	glBindVertexArray(0); // Unbind after drawing
}

Model::~Model(void)
{
}
