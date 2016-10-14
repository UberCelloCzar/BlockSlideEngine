// Trevor Walden, IGME 209 Section 2, 23 April 2016
// This file contains the declarations for functions and variables involved in collision detection

#pragma once
#include "Transform.h"

class GameObject; // Removes issue with circular dependency in header files

class Collider
{
public:
	Collider(void); // Unparameterized constructor initializes as no collider
	Collider(int type); // Parameterized constructor sets up collider type

	enum ColliderType { None, AABB, Sphere }; // Type of collider
	ColliderType colliderType; // Type of this collider
	bool collidesWith(Transform &object, GameObject &other); // Checks if this object collides with another, returns 0=no collision, 1=right collision, 2=left collision, 3=up collision, 4=down collision, [5=top collision, 6=bottom collision (these are for later 3d use and not used in this project)] 
	bool isOutOfBounds(Transform &object); // Checks if object is in bounds or not
	bool Collider::isOffIce(Transform &object); // Checks if an object is colliding with the stone walkways

	~Collider(void);

private:
	glm::vec3 dist; // Distance between centers of colliders being checked
	float distanceSq; // Unrelated to dist, this is the square of the distance between a sphere collider's center and the closest point on an aabb collider
};

