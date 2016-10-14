// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains declarations for functions and variables of a game object's transform, which handles orientation

#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform(void); // Unparameterized constructor
	Transform(glm::vec3 plPosition, glm::vec3 plRotation, glm::vec3 plScale); // Parameterized constructor setting up local positioning vectors

	glm::vec3 lPosition, lRotation, lScale; // Object's location rotation and scale unmodified by other objects
	glm::vec4 forward; // Direction vector
	glm::mat4 localToWorld; // Matrix to transform object's attributes to world format for drawing
	void Update(void); // Calculates orientation matrix and sends to shader
	void Rotate(char axis, float eulerAngle); // Rotates around specified axis for specified angle using quaternions

	~Transform(void);
};

