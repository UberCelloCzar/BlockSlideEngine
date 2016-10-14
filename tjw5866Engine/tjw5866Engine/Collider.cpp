// Trevor Walden, IGME 209 Section 2, 23 April 2016
// This file contains the definitions for functions and variables involved in collision detection

#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"



Collider::Collider(void) // Unparameterized constructor initializes no collider
{
	colliderType = None;
}

Collider::Collider(int type) // Parameterized constructor sets up collider type
{
	switch (type)
	{
		case 0:
			colliderType = None;
			break;
		case 1:
			colliderType = AABB;
			break;
		case 2:
			colliderType = Sphere;
			break;
	}
}

bool Collider::collidesWith(Transform &object, GameObject &other) // Checks if this object collides with another, returns bool
{
	if (colliderType == None || other.collider.colliderType == None) return false; // If either object has no collider, return no collision

	dist = object.lPosition - other.transform.lPosition; // Distance between centers is used for multiple checks

	if (colliderType == Sphere && other.collider.colliderType == Sphere) // If both colliders are spheres, check radiuses against distance of centers
	{
		if (glm::dot(dist, dist) > ((object.lScale.x + other.transform.lScale.x)*(object.lScale.x + other.transform.lScale.x)))
		{
			return false; // If the distance between sphere centers is not less than the radiuses, they are not colliding
		}
	}
	else if (colliderType == AABB && other.collider.colliderType == AABB) // If both colliders are axis aligned bounding boxes, check all edges
	{
		dist.x = fabs(dist.x);
		dist.y = fabs(dist.y);
		dist.z = fabs(dist.z);
		if (dist.x > fabs(object.lScale.x + other.transform.lScale.x) ||
			dist.y > fabs(object.lScale.y + other.transform.lScale.y) ||
			dist.z > fabs(object.lScale.z + other.transform.lScale.z)) // If the objects are further apart than any of their bounds, they can't be colliding
		{
			return false;
		}
	}
	else if (colliderType == Sphere && other.collider.colliderType == AABB) // If this object collider is a sphere and the other is a bounding box
	{
		distanceSq = 0; // Unrelated to dist, this is the square of the distance between a sphere collider's center and the closest point on an aabb collider
		if (object.lPosition.x < (other.transform.lPosition.x - other.transform.lScale.x)) // Sphere is to the left of the box
		{
			distanceSq += ((other.transform.lPosition.x - other.transform.lScale.x) - object.lPosition.x)*((other.transform.lPosition.x - other.transform.lScale.x) - object.lPosition.x); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (object.lPosition.x > (other.transform.lPosition.x + other.transform.lScale.x)) // Sphere is to the right of the box
		{
			distanceSq += (object.lPosition.x - (other.transform.lPosition.x + other.transform.lScale.x))*(object.lPosition.x - (other.transform.lPosition.x + other.transform.lScale.x));
		}

		if (object.lPosition.y < (other.transform.lPosition.y - other.transform.lScale.y)) // Sphere is to the top of the box
		{
			distanceSq += ((other.transform.lPosition.y - other.transform.lScale.y) - object.lPosition.y)*((other.transform.lPosition.y - other.transform.lScale.y) - object.lPosition.y); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (object.lPosition.y > (other.transform.lPosition.y + other.transform.lScale.y)) // Sphere is to the bottom of the box
		{
			distanceSq += (object.lPosition.y - (other.transform.lPosition.y + other.transform.lScale.y))*(object.lPosition.y - (other.transform.lPosition.y + other.transform.lScale.y));
		}

		if (object.lPosition.z < (other.transform.lPosition.z - other.transform.lScale.z)) // Sphere is above the box
		{
			distanceSq += ((other.transform.lPosition.z - other.transform.lScale.z) - object.lPosition.z)*((other.transform.lPosition.z - other.transform.lScale.z) - object.lPosition.z); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (object.lPosition.z > (other.transform.lPosition.z + other.transform.lScale.z)) // Sphere is below the box
		{
			distanceSq += (object.lPosition.z - (other.transform.lPosition.z + other.transform.lScale.z))*(object.lPosition.z - (other.transform.lPosition.z + other.transform.lScale.z));
		}

		if (distanceSq > (object.lScale.x*object.lScale.x)) // If the distance between the sphere's center and the closest point on the aabb is greater than the sphere's radius, they don't collide
		{
			return false;
		}
	}
	else if (colliderType == AABB && other.collider.colliderType == Sphere) // If this object collider is a bounding box and the other is a sphere
	{
		dist = other.transform.lPosition - object.lPosition; // Invert distance between centers to more easily copy paste code from above
		distanceSq = 0; // Unrelated to dist, this is the square of the distance between a sphere collider's center and the closest point on an aabb collider
		if (other.transform.lPosition.x < (object.lPosition.x - object.lScale.x)) // Sphere is to the left of the box
		{
			distanceSq += ((object.lPosition.x - object.lScale.x) - other.transform.lPosition.x)*((object.lPosition.x - object.lScale.x) - other.transform.lPosition.x); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (other.transform.lPosition.x >(object.lPosition.x + object.lScale.x)) // Sphere is to the right of the box
		{
			distanceSq += (other.transform.lPosition.x - (object.lPosition.x + object.lScale.x))*(other.transform.lPosition.x - (object.lPosition.x + object.lScale.x));
		}

		if (other.transform.lPosition.y < (object.lPosition.y - object.lScale.y)) // Sphere is to the top of the box
		{
			distanceSq += ((object.lPosition.y - object.lScale.y) - other.transform.lPosition.y)*((object.lPosition.y - object.lScale.y) - other.transform.lPosition.y); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (other.transform.lPosition.y >(object.lPosition.y + object.lScale.y)) // Sphere is to the bottom of the box
		{
			distanceSq += (other.transform.lPosition.y - (object.lPosition.y + object.lScale.y))*(other.transform.lPosition.y - (object.lPosition.y + object.lScale.y));
		}

		if (other.transform.lPosition.z < (object.lPosition.z - object.lScale.z)) // Sphere is above the box
		{
			distanceSq += ((object.lPosition.z - object.lScale.z) - other.transform.lPosition.z)*((object.lPosition.z - object.lScale.z) - other.transform.lPosition.z); // I really wish there was a square function that didn't have the risk of crashing on negative numbers (might make one myself later)
		}
		else if (other.transform.lPosition.z >(object.lPosition.z + object.lScale.z)) // Sphere is below the box
		{
			distanceSq += (other.transform.lPosition.z - (object.lPosition.z + object.lScale.z))*(other.transform.lPosition.z - (object.lPosition.z + object.lScale.z));
		}

		if (distanceSq > (other.transform.lScale.x*other.transform.lScale.x)) // If the distance between the sphere's center and the closest point on the aabb is greater than the sphere's radius, they don't collide
		{
			return false;
		}
	}

	return true; // If there are no cases proving no collision, they must be colliding
}

bool Collider::isOutOfBounds(Transform &object) // Checks if object is in bounds or not
{
	if ((object.lPosition.y + object.lScale.y) > .75 || (object.lPosition.y - object.lScale.y) < -.75 || (object.lPosition.x + object.lScale.x) > .9 || (object.lPosition.x - object.lScale.x) < -1) // Check against greater bounds
	{
		return true;
	}
	else if ((object.lPosition.x - object.lScale.x) < -.8 && ((object.lPosition.y + object.lScale.y) > .15 || (object.lPosition.y - object.lScale.y) < -.05)) // Check against 2 inside left boxes
	{
		return true;
	}
	else if ((object.lPosition.x + object.lScale.x) > .7 && ((object.lPosition.y + object.lScale.y) > .15 || (object.lPosition.y - object.lScale.y) < -.05)) // Check against 2 inside right boxes
	{
		return true;
	}

	return false; // If none of the cases were true, it's in bounds
}

bool Collider::isOffIce(Transform &object) // Checks if an object is colliding with the stone walkways
{
	if ((object.lPosition.x - object.lScale.x) < -.6 || (object.lPosition.x + object.lScale.x) > .7 || (object.lPosition.y + object.lScale.y) > .55 || (object.lPosition.y - object.lScale.y) < -.55) // Check greater boundaries
	{
		return true;
	}
	else if ((object.lPosition.x + object.lScale.x) > .4 && ((object.lPosition.y + object.lScale.y) > .15 || (object.lPosition.y - object.lScale.y) < -.05)) // Check 2 right rects
	{
		return true;
	}

	return false; // If not off the ice, return so
}

Collider::~Collider(void)
{
}
