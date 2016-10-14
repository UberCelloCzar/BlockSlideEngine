// Trevor Walden, IGME 209 Section 2, 28 April 2016
// This file contains the code definitions for the functions and variables of a camera object

#include "Camera.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "InputManager.h"

Camera::Camera(void) // Unparameterized constructor initializes (0,0,0)
{
	transform = Transform();
	aspect = 1;
	zoom = 1;
	zNear = .01f;
	zFar = 2;
	rigidBody = RigidBody();
	sens = .005f;
	width = 1280;
	height = 960;
	speed = .5;
}

Camera::Camera(glm::vec3 pLoc, glm::vec3 pRot, float pAspect, float pZNear, float pZFar, float pZoom, int pWidth, int pHeight) // Parameterized contstructor initializes camera with given values
{
	transform = Transform(pLoc, pRot, { 1,1,1 });
	aspect = pAspect;
	zNear = pZNear;
	zFar = pZFar;
	zoom = pZoom;
	rigidBody = RigidBody(1, 2, 0);
	sens = .005f;
	width = pWidth;
	height = pHeight;
	speed = .5;
}

void Camera::Update(GLFWwindow *window, float deltaT) // Main loop updates camera and uploads matrix
{
	/*glfwGetCursorPos(window, &x, &y); // Find the cursor to turn with the mouse
	transform.lRotation.y -= sens * (x - width * .5f); // Yaw
	transform.lRotation.x -= sens * (y - height * .5f); // Pitch
	transform.lRotation.x = glm::clamp(transform.lRotation.x, -.5f*3.141592f, .5f*3.141592f); // Limit rotation
	glfwSetCursorPos(window, width*.5f, height*.5f); // Keep the cursor in the center of the window*/



	rotMat = (glm::mat3)glm::yawPitchRoll(transform.lRotation.y, transform.lRotation.x, transform.lRotation.z); // Rotate the camera, without quaternions b/c it's a camera
	if (keyIsDown(GLFW_KEY_LEFT)) // Move the camera based on input
	{
		rigidBody.velocity += rotMat * glm::vec3(-1, 0, 0);
	}
	else if (keyIsDown(GLFW_KEY_RIGHT))
	{
		rigidBody.velocity += rotMat * glm::vec3(1, 0, 0);
	}
	else if (keyIsDown(GLFW_KEY_UP))
	{
		rigidBody.velocity += rotMat * glm::vec3(0, 0, -1);
	}
	else if (keyIsDown(GLFW_KEY_DOWN))
	{
		rigidBody.velocity += rotMat * glm::vec3(0, 0, 1);
	}
	else
	{
		rigidBody.fullStop();
	}

	if (rigidBody.velocity != glm::vec3())
	{
		rigidBody.velocity = glm::normalize(rigidBody.velocity) * speed;
	}
	transform.lPosition += rigidBody.velocity * deltaT; // Update position

	center = transform.lPosition + rotMat * glm::vec3(0, 0, -1); // Calculate the camera's orientation
	up = rotMat * glm::vec3(0, 1, 0); // Calculate up vector
	lookAtMat = glm::lookAt(transform.lPosition, center, up); // Get the orientation matrix

	fovy =  3.141592f * .4f / zoom; // Calculate the field of view (angle between planes in radians, scaled by zoom)
	perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar); // Get the view projection matrix

	cameraMat = perspectiveMat * lookAtMat; // Calculate the camera's matrix
	glUniformMatrix4fv(4, 1, GL_FALSE, &cameraMat[0][0]); // Pass the matrix into the shader
	glUniform3fv(6, 1, &transform.lPosition[0]); // Pass in location
}

Camera::~Camera(void)
{
}
