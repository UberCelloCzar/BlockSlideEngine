// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the definitions for functions and variables of a game object

#include "GameObject.h"
#include "RigidBody.h"
#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include "Model.h"
#include "Collider.h"

GameObject::GameObject(void) // Unparameterized constructor
{
	model = Model();
	transform = Transform();
	rigidBody = RigidBody();
	collider = Collider();
}

GameObject::GameObject(GLuint pTexture, glm::vec3 plPosition, glm::vec3 plRotation, glm::vec3 plScale, float pMass, float pMaxVelocity, int pSurfaceType, int colliderType) // Parameterized constructor sets up components
{
	model = Model(pTexture);
	transform = Transform(plPosition, plRotation, plScale);
	rigidBody = RigidBody(pMass, pMaxVelocity, pSurfaceType);
	collider = Collider(colliderType);
}

bool GameObject::init(char* objFile) // Initializes model+texture, and anything else that needs it
{
	if (!model.buffer(objFile)) return false; // Create mesh, break if failure
	return true; // Return success if no errors
}

void GameObject::moveForward(float deltaT) // Route to rigidBody's moveForward
{
	rigidBody.moveForward(deltaT, transform.forward);
}

bool GameObject::collidesWith(GameObject &other) // Route to collider's collidesWith
{
	return collider.collidesWith(transform, other);
}

void GameObject::Update(float deltaT) // Updates and draws
{
	rigidBody.Update(deltaT);
	transform.lPosition += rigidBody.velocity * deltaT; // Not sure how much cutting deltaT down to a float will affect anything
	transform.Update();

	normalMat = glm::transpose(glm::inverse(transform.localToWorld)); // Normal matrix
	glUniformMatrix4fv(5, 1, GL_FALSE, &normalMat[0][0]); // Pass the matrix into the shader

	model.Draw();
}

GameObject::~GameObject(void)
{
}
