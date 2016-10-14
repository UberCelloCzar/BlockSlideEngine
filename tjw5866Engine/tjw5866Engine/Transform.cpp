// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the definitions of functions and variables for a game object's transform, which handles orientation

#include "Transform.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp> // No gimbal lock for me, thanks
#include <glm/gtx/quaternion.hpp>
#include <iostream>

Transform::Transform(void) // Unparameterized constructor
{
	lPosition = { 0, 0, 0 }; // Initialize vectors
	lRotation = { 0, 0, 0 };
	lScale = { 1, 1, 1 };
	forward = glm::vec4(1, 0, 0, 0);
}

Transform::Transform(glm::vec3 plPosition, glm::vec3 plRotation, glm::vec3 plScale) // Parameterized constructor setting up local positioning vectors
{
	lPosition = plPosition; // Initialize vectors
	lRotation = plRotation;
	lScale = plScale;
	forward = glm::toMat4(glm::quat(lRotation)) * glm::vec4(1, 0, 0, 0);
}



void Transform::Update(void) 
{
	localToWorld = glm::translate(lPosition) * // Calculate the object-world translation matrix
		glm::toMat4(glm::quat(lRotation)) *
		glm::scale(lScale);
	glUniformMatrix4fv(3, 1, GL_FALSE, &localToWorld[0][0]); // Pass the transform into the shader

}

void Transform::Rotate(char axis, float eulerAngle) // Rotates around specified axis for specified angle
{
	switch (axis)
	{
		case 'x':
			lRotation.x += eulerAngle;
			break;
		case 'y':
			lRotation.y += eulerAngle;
			break;
		case 'z':
			lRotation.z += eulerAngle;
			break;
	}
	forward = glm::toMat4(glm::quat(lRotation)) * glm::vec4(1, 0, 0, 0); // Rotate the forward vector to reflect change any time there is one
}

Transform::~Transform(void)
{
}
