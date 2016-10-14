// Trevor Walden, IGME 209 Section 2, 28 April 2016
// This file contains the declarations for the functions and variables of a camera object

#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Transform.h"
#include "RigidBody.h"

class Camera
{
public:
	Camera(void); // Unparameterized constructor, initializes (0,0,0)
	Camera(glm::vec3 pLoc, glm::vec3 pRot, float pAspect, float pZNear, float pZFar, float pZoom, int pWidth, int pHeight); // Parameterized contstructor initializes camera with given values

	void Update(GLFWwindow *window, float deltaT); // Main loop updates camera and uploads matrix
	glm::mat4 cameraMat, lookAtMat, perspectiveMat; // Matrix used to transform the world to the camera's viewport; Matrix determining orientation of camera; Matrix determining view of camera
	glm::vec3 center, up; // Variables for orientation
	glm::mat3 rotMat; // Stores the rotation transform matrix for use in other calculations
	float aspect, zNear, zFar, zoom, fovy; // Viewport size, clipping planes, zoom factor, field of view
	Transform transform; // Used to hold location and rotation
	RigidBody rigidBody; // Used to do movement
	float sens, speed; // Cursor move sensitivity and camera movement speed
	double x, y; // Cursor position
	int width, height; // Width and height of the display window

	~Camera(void);
};

