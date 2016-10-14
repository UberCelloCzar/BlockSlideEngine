// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the definitions of functions and variables for the rigidbody component of a game object, which handles movement physics

#include "RigidBody.h"
#include <GL/glew.h> // Use OpenGL, extensions, and extra tools
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

RigidBody::RigidBody(void) // Unparameterized constructor
{
	velocity = { 0, 0, 0 }; // Initialize movement values
	force = { 0, 0, 0 };
	mass = 1;
	maxVelocity = 0;
	maxVelocitySq = 0;
	surfaceType = 0;
}

RigidBody::RigidBody(float pMass, float pMaxVelocity, int pSurfaceType) // Parameterized constructor taking object's mass and max velocity, as well as type of surface it's on
{
	velocity = { 0, 0, 0 }; // Initialize movement values
	force = { 0, 0, 0 };
	mass = pMass;
	maxVelocity = pMaxVelocity;
	maxVelocitySq = maxVelocity * maxVelocity;
	surfaceType = pSurfaceType;
}

void RigidBody::moveForward(float deltaT, const glm::vec4 &forward) // Apply force in the direction faced; strength depends on surface type
{
	vForce = { forward.x, forward.y, forward.z };
	glm::normalize(vForce);
	if (surfaceType == 1) vForce *= .6; // On ice, a constant force is applied
	else if (surfaceType == 2) vForce *= ((maxVelocity / deltaT) * mass); // For the block, force is applied once, so max speed is reached instantly
	else vForce *= ((maxVelocity / deltaT) * mass * .02); // On stone, max velocity is reached quicker, but not too quick
	
	force += vForce; // Add force to total
}

void RigidBody::iceFriction(void) // Applies the friction of ice
{
	vForce = velocity; // Friction is in direction opposite velocity
	glm::normalize(vForce);
	vForce *= -1;
	force += vForce; // I love how glm overloaded shortcut operators too :D
}

void RigidBody::softStop(float deltaT) // Applies large but not maximum deceleration force
{
	vForce = (velocity / (float)deltaT) * mass;
	vForce *= -.06;
	force += vForce;
}

void RigidBody::fullStop(void) // Zeros out force and velocity vectors
{
	velocity = { 0, 0, 0 };
	force = { 0, 0, 0 };
}

void RigidBody::changeSurface(void) // Switches surface type from ice to stone or stone to ice
{
	surfaceType++;
	if (surfaceType > 1) surfaceType = 0;
}

void RigidBody::Update(float deltaT) // Calculates and limits movement
{
	velocity += (force / mass) * deltaT; // Calculate movement
	if (glm::length2(velocity) > maxVelocitySq) // Limit velocity by comparing its square magnitude to square maximum
	{
		glm::normalize(velocity);
		velocity *= maxVelocity;
	}
	if (abs(velocity.x) <= .0001 && abs(velocity.y) <= .0001) velocity = { 0, 0, 0 }; // If it's close to stopping, stop it
	force = { 0,0,0 }; // Zero out force to avoid issues
}


RigidBody::~RigidBody(void)
{
}
