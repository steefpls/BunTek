#include <stdio.h>
#include <math.h>
#include "cprocessing.h"

#define Vector2 struct vector2
#define CircleGameObject struct circleGameObject
#define BoxGameObject struct boxGameObject
#define GameObject struct gameObject
#define Particle struct particle

//Reinventing the wheel
typedef enum {false, true} bool;
#define false 0x0
#define true 0x1


struct vector2
{
	float x;
	float y;
};

struct gameObject
{
	// Main properties
	Vector2 position;
	Vector2 velocity;
	float angle;
	bool isActive;
	CP_Color color;
};


struct circleGameObject
{
	GameObject gameObject;
	float radius;
};

struct circleGameObject CreateCircleGameObject(Vector2 pos, Vector2 vel, float angle, CP_Color color, float radius)
{
	CircleGameObject c;

	c.gameObject.position.x = pos.x;
	c.gameObject.position.y = pos.y;
	c.gameObject.velocity.x = vel.x;
	c.gameObject.velocity.y = vel.y;
	c.gameObject.angle = angle;
	c.radius = radius;
	c.gameObject.color = color;



	return c;
}

struct boxGameObject
{
	// Main properties
	GameObject gameObject;
	float width;
	float height;

};

struct particle
{
	GameObject gameObject;
	float radius;

};

bool CircleCol(CircleGameObject c1, CircleGameObject c2)
{
	float distX = c1.gameObject.position.x - c2.gameObject.position.x;
	float distY = c1.gameObject.position.y - c2.gameObject.position.y;

	float distSq = distX * distX + distY * distY;

	//Check for Collision
	float totalRadius = c1.radius + c2.radius;
	float totalRadiusSq = totalRadius * totalRadius;


	return (distSq < totalRadiusSq);
}

Vector2 newVector2(float x, float y)
{
	Vector2 v2;
	v2.x = x;
	v2.y = y;
	
	return v2;
}