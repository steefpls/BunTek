#include <stdio.h>
#include <math.h>
#include "cprocessing.h"

#define Vector2 struct vector2
#define CircleGameObject struct circleGameObject
#define BoxGameObject struct boxGameObject
#define GameObject struct gameObject
#define Particle struct particle
#define FrameTime CP_System_GetDt()

#define bool _Bool

#define false 0x0
#define true 0x1

bool debug = true;
float gravity = 500;


void draw_framerate(void)
{
	CP_Settings_TextSize(20);
	CP_Settings_BlendMode(CP_BLEND_ALPHA);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 128));
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(0, 0, 150, 30);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char buffer[100];
	sprintf_s(buffer, 100, "FPS: %f \n Frametime: %f", CP_System_GetFrameRate(), CP_System_GetDt());
	CP_Font_DrawText(buffer, 20, 20);
}

struct vector2
{
	float x;
	float y;
};
Vector2 newVector2(float x, float y)
{
	Vector2 v2;
	v2.x = x;
	v2.y = y;

	return v2;
}

struct gameObject
{
	// Main properties
	Vector2 position;
	Vector2 velocity; //pixels per second
	float angle;
	bool isActive;
	CP_Color color;
	float mass;

};


struct circleGameObject
{
	GameObject gameObject;
	float radius;
	bool outline;
	float bounciness;
};

struct circleGameObject CreateCircleGameObject(Vector2 pos, Vector2 vel, float angle, CP_Color color, float radius, bool outline, float mass, float bounciness)
{
	CircleGameObject c;

	c.gameObject.isActive = true;
	c.gameObject.position.x = pos.x;
	c.gameObject.position.y = pos.y;
	c.gameObject.velocity.x = vel.x;
	c.gameObject.velocity.y = vel.y;
	c.gameObject.angle = angle;
	c.radius = radius;
	c.gameObject.color = color;
	c.outline = outline;
	c.gameObject.mass = mass;
	c.bounciness = bounciness;

	return c;
}



struct boxGameObject
{
	// Main properties
	GameObject gameObject;
	float width;
	float height;

};

struct boxGameObject CreateBoxGameObject(Vector2 position) {
	BoxGameObject b;
	b.gameObject.isActive = true;
	b.gameObject.position = position;
	

	return b;
}

struct particle
{
	GameObject gameObject;
	float radius;

};

float DistBetween(Vector2 v1, Vector2 v2) {
	return sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}
float DotProd(Vector2 v1, Vector2 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}
Vector2 Normalize(Vector2 v1) {
	float length = sqrtf(v1.x * v1.x + v1.y * v1.y);
	return newVector2(v1.x / (length), v1.y / (length));
}
Vector2 VectorAdd(Vector2 v1, Vector2 v2) {

	return newVector2(v1.x + v2.x, v1.y + v2.y);
}
Vector2 VectorMinus(Vector2 v1, Vector2 v2) {
	return newVector2(v1.x - v2.x, v1.y - v2.y);
}
Vector2 VectorMultiply(Vector2 v1, float f1) {
	return newVector2(v1.x *f1, v1.y *f1);
}
Vector2 VectorDivide(Vector2 v1, float f1) {
	return newVector2(v1.x / f1, v1.y / f1);
}

void AddForce(GameObject* g, Vector2 force) {
	g->velocity.x += force.x;
	g->velocity.y += force.y;
}

bool CircleCol(CircleGameObject* c1, CircleGameObject* c2)
{
	float c1X = c1->gameObject.position.x;
	float c1Y = c1->gameObject.position.y;
	float c2X = c2->gameObject.position.x;
	float c2Y = c2->gameObject.position.y;
	//printf("\nCircle1pos: %f,%f\nCircle2pos: %f,%f\n",c1X,c1Y,c2X,c2Y);
	//Are the two circles close? Do significantly cheaper bounding box check first
	bool inBoundingBox = c1X + c1->radius + c2->radius > c2X
		&& c1X < c2X + c1->radius + c2->radius
		&& c1Y + c1->radius + c2->radius > c2Y
		&& c1Y < c2Y + c1->radius + c2->radius;

	if (inBoundingBox)
	{
		float dist = DistBetween(c1->gameObject.position, c2->gameObject.position);
		//Check for Collision
		float totalRadius = c1->radius + c2->radius;

		//If collided
		if (dist < totalRadius) {
			//printf("Circle 1 velocity: %f,%f \nCircle 2 velocity: %f,%f\n", c1->gameObject.velocity.x, c1->gameObject.velocity.y, c2->gameObject.velocity.x, c2->gameObject.velocity.y);
			//Reposition appropriately
			float distX = c1->gameObject.position.x - c2->gameObject.position.x;
			float distY = c1->gameObject.position.y - c2->gameObject.position.y;

			float radiiSum = c1->radius + c2->radius;

			float unitX = distX / dist;
			float unitY = distY / dist;

			c1->gameObject.position.x = c2->gameObject.position.x + (radiiSum)*unitX;
			c1->gameObject.position.y = c2->gameObject.position.y + (radiiSum)*unitY;

			//Do correct physics
			float collisionPointX = ((c1X * c2->radius) + (c2X * c1->radius)) / (c1->radius+c2->radius);
			float collisionPointY = ((c1Y * c2->radius) + (c2Y * c1->radius)) / (c1->radius + c2->radius);

			// circle1.Velocity.X -= velocityComponentPerpendicularToTangent.X; PSEUDOCODE FOR NEW VELOCITY
			Vector2 perpVector;
			perpVector.x = (c2X - c1X);
			perpVector.y = (c2Y - c1Y);

			Vector2 tanVector;		
			tanVector.y = -(c2X - c1X);
			tanVector.x = (c2Y - c1Y);
			tanVector = Normalize(tanVector);

			if (debug) {
				//Tangent Vector is white
				CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
				CP_Graphics_DrawLine(collisionPointX - tanVector.x * 50 / 2, collisionPointY - tanVector.y*50 / 2, collisionPointX + tanVector.x * 50 / 2, collisionPointY + tanVector.y * 50 / 2);
				//Perp Vector is Green
				CP_Settings_Stroke(CP_Color_Create(0, 255, 0, 255));
				CP_Graphics_DrawLine(collisionPointX - perpVector.x / 2, collisionPointY - perpVector.y / 2, collisionPointX + perpVector.x / 2, collisionPointY + perpVector.y / 2);

				//draw colliding point
				CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
				CP_Graphics_DrawCircle(collisionPointX, collisionPointY, 5);
			}

			Vector2 relativeVelocity = newVector2(c1->gameObject.velocity.x - c2->gameObject.velocity.x, c1->gameObject.velocity.y - c2->gameObject.velocity.y);
			float length = DotProd(relativeVelocity, tanVector);

			Vector2 velocityComponentOnTangent;
			velocityComponentOnTangent.x = tanVector.x * length;
			velocityComponentOnTangent.y = tanVector.y * length;

			Vector2 velocityComponentPerpendicularToTangent;
			velocityComponentPerpendicularToTangent.x = relativeVelocity.x - velocityComponentOnTangent.x;
			velocityComponentPerpendicularToTangent.y = relativeVelocity.y - velocityComponentOnTangent.y;

			//Calculate new velocity
			float newVelX1 = c1->gameObject.velocity.x - 2 * (velocityComponentPerpendicularToTangent.x) * (c2->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c1->bounciness);
			float newVelY1 = c1->gameObject.velocity.y - 2 * (velocityComponentPerpendicularToTangent.y) * (c2->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c1->bounciness);
			float newVelX2 = c2->gameObject.velocity.x + 2 * (velocityComponentPerpendicularToTangent.x) * (c1->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c2->bounciness);
			float newVelY2 = c2->gameObject.velocity.y + 2 * (velocityComponentPerpendicularToTangent.y) * (c1->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c2->bounciness);


			c1->gameObject.velocity.x = newVelX1;
			c1->gameObject.velocity.y = newVelY1;
			c2->gameObject.velocity.x = newVelX2;
			c2->gameObject.velocity.y = newVelY2;
		}

		return (dist < totalRadius);
	}

	else {
		return false;
	}

	
	
}

void CirclePhys(CircleGameObject* c1) {
	c1->gameObject.velocity.y += gravity * FrameTime;
	//AddForce(&c1->gameObject, newVector2(0, gravity * FrameTime));

	c1->gameObject.position.x += c1->gameObject.velocity.x * FrameTime;
	c1->gameObject.position.y += c1->gameObject.velocity.y * FrameTime;

	if (c1->gameObject.position.x + c1->radius > CP_System_GetWindowWidth() ) {
		c1->gameObject.position.x = CP_System_GetWindowWidth() - c1->radius;
		c1->gameObject.velocity.x *= -1 * c1->bounciness;
	}
	else if (c1->gameObject.position.x - c1->radius < 0) {
		c1->gameObject.position.x = 0+ c1->radius;
		c1->gameObject.velocity.x *= -1 * c1->bounciness;
	}
	if (c1->gameObject.position.y + c1->radius > CP_System_GetWindowHeight()) {
		c1->gameObject.position.y = CP_System_GetWindowHeight() - c1->radius;
		c1->gameObject.velocity.y *= -1 * c1->bounciness;
	}
	else if (c1->gameObject.position.y - c1->radius < 0) {
		c1->gameObject.position.y = 0 + c1->radius;
		c1->gameObject.velocity.y *= -1 * c1->bounciness;
	}
}



