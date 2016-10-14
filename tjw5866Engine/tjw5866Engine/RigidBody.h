// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file holds the declarations of functions and variables for the rigidbody component of a game object, which handles movement physics

#pragma once
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

class RigidBody
{
public:
	RigidBody(void); // Unparameterized constructor
	RigidBody(float pMass, float pMaxVelocity, int pSurfaceType); // Parameterized constructor taking object's mass and max velocity, as well as type of surface it's on
	void moveForward(float deltaT, const glm::vec4 &forward); // Provides force to move in faced direction based on surface 
	void iceFriction(void); // Applies friction of ice
	void softStop(float deltaT); // Applies large but not maximum deceleration force
	void fullStop(void); // Zeros out force and velocity vectors
	void changeSurface(void); // Switches surface type from ice to stone or stone to ice
	void Update(float deltaT); // Calculates and limits movement

	glm::vec3 velocity, force; // Acceleration is a middleman that doesn't need to be stored unless it's being limited
	float mass, maxVelocity, maxVelocitySq; 
	int surfaceType; // Type of surface object is on. 0 is player on stone, 1 is player on ice, 2 is box on ice
						 // Player on stone and block on ice have 100% startup acceleration, then none, then 100% deceleration
						 // Player on ice has asteroids-like friction with slow initial and final acceleration/deceleration

	~RigidBody(void);

private:
	glm::vec3 vForce;
};

