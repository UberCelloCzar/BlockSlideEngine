// Trevor Walden, IGME 209 Section 2, 15 April 2016
// This file contains the declarations for functions and variables of a game object

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "RigidBody.h"
#include "Transform.h"
#include "Model.h"
#include "Collider.h"

class GameObject
{
public:
	GameObject(void); // Unparameterized constructor
	GameObject(GLuint pTexture, glm::vec3 plPosition, glm::vec3 plRotation, glm::vec3 plScale, float pMass, float pMaxVelocity, int pSurfaceType, int colliderType); // Parameterized constructor sets up components
	
	Transform transform; // Object's transform
	RigidBody rigidBody; // Object's physics data
	Collider collider; // Object's collider
	Model model; // Object's model
	bool init(char* objFile); // Initializes model+texture, and anything else that needs it
	void moveForward(float deltaT); // Route to rigidBody's moveForward
	bool collidesWith(GameObject &other); // Route to collider's collidesWith
	void Update(float deltaT); // Updates and draws
	glm::mat4 normalMat; // Normal matrix

	~GameObject(void);
};

