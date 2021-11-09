#include <stdio.h>
#include <math.h>
#include "cprocessing.h"

#define PI 	3.14159265358979323846f
#define Vector2 struct vector2
#define CircleGameObject struct circleGameObject
#define BoxGameObject struct boxGameObject
#define BallSpawner struct ballSpawner
#define GameObject struct gameObject
#define Particle struct particle
#define FrameTime CP_System_GetDt()

#define bool _Bool

#define false 0x0
#define true 0x1

bool debug = false;
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
	float bounciness;
};

struct circleGameObject
{
	GameObject gameObject;
	float radius;
	bool outline;
	
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
	c.gameObject.bounciness = bounciness;

	return c;
}



struct boxGameObject
{
	// Main properties
	GameObject gameObject;
	float width;
	float height;
	CP_Image image;

};

struct boxGameObject CreateBoxGameObject(Vector2 position, float width, float height,float bounciness, float angle, CP_Image image) {
	BoxGameObject b;
	b.gameObject.isActive = true;
	b.gameObject.position = position;
	b.height = height;
	b.width = width;
	b.gameObject.angle = angle;
	b.gameObject.bounciness = bounciness;
	b.image = image;
	return b;
}

struct ballSpawner {
	BoxGameObject b;
	float spawnrate; // Balls per second
	float internalTimer; //Timer to spawn balls
	bool isSpawning; // Is spawning balls or not
	float spawnVel;
	float spreadAngle;

};

struct ballSpawner CreateBallSpawner(Vector2 position, float width, float height, float angle, float spawnrate, bool isSpawning, float spawnVel, float spreadAngle, CP_Image image) {
	BallSpawner ba;
	ba.b.gameObject.isActive = true;
	ba.b.gameObject.position = position;
	ba.b.height = height;
	ba.b.width = width;
	ba.b.gameObject.angle = angle;
	ba.b.image = image;
	ba.spawnrate = spawnrate;
	ba.isSpawning = isSpawning;
	ba.spawnVel;
	ba.spreadAngle;

	return ba;
}

struct particle
{
	GameObject gameObject;
	float radius;

};
float DistBetweenSquared(Vector2 v1, Vector2 v2) {
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}

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
float VectorMagnitudeSq(Vector2 v1) {
	return v1.x * v1.x + v1.y * v1.y;
}
float VectorMagnitude(Vector2 v1) {
	return sqrtf( v1.x * v1.x + v1.y * v1.y);
}
Vector2 VectorProject(Vector2 v1, Vector2 v2) { //Finds projection of v1 on v2
	return VectorMultiply(v2, (DotProd(v1, v2) / VectorMagnitudeSq(v2)));
}
float AngBet(Vector2 p1, Vector2 p2) {
	return atan2f(p1.y - p2.y, p1.x - p2.x);
}


void AddForce(GameObject* g, Vector2 force) {
	g->velocity.x += force.x;
	g->velocity.y += force.y;
}

Vector2 TranslatePointOnBox(Vector2 BoxPosition, Vector2 BoxRotation, Vector2 Point, Vector2 Sign)
{
	Vector2 LocalPoint = newVector2(Point.x * Sign.x, Point.y * Sign.y);

	Vector2 WorldPoint;

	// rotate
	WorldPoint.x = LocalPoint.x * BoxRotation.x - LocalPoint.y * BoxRotation.y + BoxPosition.x;
	WorldPoint.y = LocalPoint.y * BoxRotation.x + LocalPoint.x * BoxRotation.y + BoxPosition.y;

	// translate
	WorldPoint.x += BoxPosition.x;
	WorldPoint.y += BoxPosition.y;

	return WorldPoint;
}

//Call this function to check if a circle collides with another circle. Set bool doPhysics to true to make them bounce off each other.
bool CircleCol(CircleGameObject* c1, CircleGameObject* c2, bool doPhysics)
{
	if (doPhysics) {
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

				//Reposition appropriately
				float distX = c1->gameObject.position.x - c2->gameObject.position.x;
				float distY = c1->gameObject.position.y - c2->gameObject.position.y;

				float radiiSum = c1->radius + c2->radius;

				float unitX = distX / dist;
				float unitY = distY / dist;

				c1->gameObject.position.x = c2->gameObject.position.x + (radiiSum)*unitX;
				c1->gameObject.position.y = c2->gameObject.position.y + (radiiSum)*unitY;

				//Do correct physics
				float collisionPointX = ((c1X * c2->radius) + (c2X * c1->radius)) / (c1->radius + c2->radius);
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
					CP_Graphics_DrawLine(collisionPointX - tanVector.x * 50 / 2, collisionPointY - tanVector.y * 50 / 2, collisionPointX + tanVector.x * 50 / 2, collisionPointY + tanVector.y * 50 / 2);
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
				float newVelX1 = c1->gameObject.velocity.x - 2 * (velocityComponentPerpendicularToTangent.x) * (c2->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c1->gameObject.bounciness);
				float newVelY1 = c1->gameObject.velocity.y - 2 * (velocityComponentPerpendicularToTangent.y) * (c2->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c1->gameObject.bounciness);
				float newVelX2 = c2->gameObject.velocity.x + 2 * (velocityComponentPerpendicularToTangent.x) * (c1->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c2->gameObject.bounciness);
				float newVelY2 = c2->gameObject.velocity.y + 2 * (velocityComponentPerpendicularToTangent.y) * (c1->gameObject.mass / (c1->gameObject.mass + c2->gameObject.mass) * c2->gameObject.bounciness);


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
	else {
		float c1X = c1->gameObject.position.x;
		float c1Y = c1->gameObject.position.y;
		float c2X = c2->gameObject.position.x;
		float c2Y = c2->gameObject.position.y;
		
		//Are the two circles close? Do significantly cheaper bounding box check first
		bool inBoundingBox = c1X + c1->radius + c2->radius > c2X
			&& c1X < c2X + c1->radius + c2->radius
			&& c1Y + c1->radius + c2->radius > c2Y
			&& c1Y < c2Y + c1->radius + c2->radius;

		if (inBoundingBox) {
			if (DistBetweenSquared(c1->gameObject.position, c2->gameObject.position) < c1->radius * c1->radius + c2->radius * c2->radius) {
				return true;
			}
		}
		return false;
	}

}

// Call this function to check if a circle collides with a point. Set bool doPhysics to true to make the circle bounce off the point. Useful for buttons and UI, but remember to set doPhysics to false.
bool CirclePointCol(CircleGameObject* c1, Vector2 p1, bool doPhysics) {
	float c1X = c1->gameObject.position.x;
	float c1Y = c1->gameObject.position.y;
	float c2X = p1.x;
	float c2Y = p1.y;

	bool collided = false;

	if (!doPhysics) {
		//Are the two circles close? Do significantly cheaper bounding box check first
		bool inBoundingBox = c1X + c1->radius  > c2X
			&& c1X < c2X + c1->radius 
			&& c1Y + c1->radius > c2Y
			&& c1Y < c2Y + c1->radius;

		if (inBoundingBox) {
			if (DistBetweenSquared(c1->gameObject.position, p1) < c1->radius * c1->radius) {
				collided =  true;
			}
			else {
				collided = false;
			}
		}
		else {
			collided = false;
		}
		
	}
	if (collided) {


		float dist = DistBetween(c1->gameObject.position, p1);
		//Reposition appropriately
		float distX = c1->gameObject.position.x - p1.x;
		float distY = c1->gameObject.position.y - p1.y;

		float radiiSum = c1->radius;

		float unitX = distX / dist;
		float unitY = distY / dist;

		c1->gameObject.position.x = p1.x + (radiiSum)*unitX;
		c1->gameObject.position.y = p1.y + (radiiSum)*unitY;

		//Do correct physics
		float collisionPointX = ((c2X * c1->radius)) / (c1->radius);
		float collisionPointY = ((c2Y * c1->radius)) / (c1->radius);

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
			CP_Graphics_DrawLine(collisionPointX - tanVector.x * 50 / 2, collisionPointY - tanVector.y * 50 / 2, collisionPointX + tanVector.x * 50 / 2, collisionPointY + tanVector.y * 50 / 2);
			//Perp Vector is Green
			CP_Settings_Stroke(CP_Color_Create(0, 255, 0, 255));
			CP_Graphics_DrawLine(collisionPointX - perpVector.x / 2, collisionPointY - perpVector.y / 2, collisionPointX + perpVector.x / 2, collisionPointY + perpVector.y / 2);

			//draw colliding point
			CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
			CP_Graphics_DrawCircle(collisionPointX, collisionPointY, 5);
		}

		Vector2 relativeVelocity = c1->gameObject.velocity;
		float length = DotProd(relativeVelocity, tanVector);

		Vector2 velocityComponentOnTangent;
		velocityComponentOnTangent.x = tanVector.x * length;
		velocityComponentOnTangent.y = tanVector.y * length;

		Vector2 velocityComponentPerpendicularToTangent;
		velocityComponentPerpendicularToTangent.x = relativeVelocity.x - velocityComponentOnTangent.x;
		velocityComponentPerpendicularToTangent.y = relativeVelocity.y - velocityComponentOnTangent.y;

		//Calculate new velocity
		float newVelX1 = c1->gameObject.velocity.x - 2 * (velocityComponentPerpendicularToTangent.x);
		float newVelY1 = c1->gameObject.velocity.y - 2 * (velocityComponentPerpendicularToTangent.y);


		c1->gameObject.velocity.x = newVelX1;
		c1->gameObject.velocity.y = newVelY1;
	}
	return collided;
}

// Call this function to check if a circle collides with a rotated or normal rect. Set bool doPhysics to true to make the cirlce bounce off the rect.
bool CircleRectCol(CircleGameObject* c1, BoxGameObject* b1, bool doPhysics) {
	
	Vector2 boxTopLeft = b1->gameObject.position;
	Vector2 boxTopRight = newVector2(boxTopLeft.x+ b1->width*cosf(b1->gameObject.angle/180*PI), boxTopLeft.y + b1->width * sinf(b1->gameObject.angle / 180 * PI));
	Vector2 boxBottomLeft = newVector2(boxTopLeft.x + b1->height * cosf((b1->gameObject.angle+90) / 180 * PI), boxTopLeft.y + b1->height * sinf((b1->gameObject.angle+90) / 180 * PI));
	Vector2 boxBottomRight = newVector2(boxTopLeft.x + b1->width * cosf(b1->gameObject.angle / 180 * PI)+b1->height * cosf((b1->gameObject.angle + 90) / 180 * PI), boxTopLeft.y + b1->width * sinf(b1->gameObject.angle / 180 * PI)+ b1->height * sinf((b1->gameObject.angle + 90) / 180 * PI));
	Vector2 boxCenter = newVector2((boxTopLeft.x+boxBottomRight.x)/2, (boxTopLeft.y + boxBottomRight.y) / 2);

	
	float boxWidth = b1->width;
	

	float boxHeight = b1->height;

	Vector2 VectorDist = VectorMinus( c1->gameObject.position, boxCenter);

	//Check within X bounds
	float dotprodx = DotProd(Normalize(VectorMinus(boxTopLeft, boxTopRight)), VectorDist);
	float dotprody = DotProd(Normalize(VectorMinus(boxTopLeft, boxBottomLeft)), VectorDist);
	
	// Check if collided
	bool collided = false;

	if ((dotprodx < boxWidth / 2 + c1->radius && dotprodx > -(boxWidth / 2 + c1->radius)) && (dotprody< boxHeight / 2 + c1->radius && dotprody > -(boxHeight / 2 + c1->radius))) {
		collided = true;
	}
	else {
		return false;
	}

	if (collided) {
		bool collidedLeft = false;
		bool collidedTop = false;

		bool confirmedX = false;
		bool confirmedY = false;


		if (dotprodx < boxWidth / 2 && dotprodx > -(boxWidth / 2)) {
			confirmedX = true;
		}
		if (dotprodx > 0) {
			collidedLeft = true;
		}
		

		if (dotprody < boxHeight / 2 && dotprody > -(boxHeight / 2)) {
			confirmedY = true;
		}
		if (dotprody > 0) {
			collidedTop = true;
		}

		
		

		//If circle center is inside rect
		if (confirmedY && confirmedX) {
			float angbet = (AngBet(c1->gameObject.position, boxCenter)/PI*180)-b1->gameObject.angle;
			if (angbet < 0) {
				angbet +=360;
			}

			float topAngleRange = atan2f(boxHeight, boxWidth)/PI*180;

			if (angbet < topAngleRange || angbet >= 270+(90-topAngleRange)) {
				confirmedX = false;
				confirmedY = true;
				collidedLeft = false;
			}
			else if (angbet >= topAngleRange && angbet < 180 - topAngleRange) {
				confirmedX = true;
				confirmedY = false;
				collidedTop = false;
			}
			else if (angbet >= 180 - topAngleRange && angbet < 180 + topAngleRange) {
				confirmedX = false;
				confirmedY = true;
				collidedLeft = true;
			}
			else {
				confirmedX = true;
				confirmedY = false;
				collidedTop = true;
			}
		}

		if (confirmedX && !confirmedY) {
			if (!doPhysics) {
				return true;
			}
			if (collidedTop) { // collided on top vertex
				
				Vector2 NewLocation;
				Vector2 ReversedBallVelocity = Normalize(VectorMultiply(c1->gameObject.velocity, -1)); // Reversed Velocity of magnitude 1
				
				float ReversedDotY = DotProd(ReversedBallVelocity, Normalize(VectorMinus(boxTopLeft, boxBottomLeft)));

				float multiplier = 1/ReversedDotY;
				ReversedBallVelocity = VectorMultiply(ReversedBallVelocity, multiplier);
				NewLocation = VectorAdd(c1->gameObject.position, VectorMultiply(ReversedBallVelocity, (boxHeight / 2 + c1->radius) - dotprody));

				// Reflect ball velocity
				Vector2 newVelocityX = VectorProject(c1->gameObject.velocity, VectorMinus(boxTopRight, boxTopLeft));
				Vector2 newVelocityY = VectorMultiply(VectorMinus(c1->gameObject.velocity, newVelocityX), -1);
				
				Vector2 finalVelocity = VectorAdd(newVelocityX, newVelocityY);
				c1->gameObject.velocity = VectorMultiply(finalVelocity, c1->gameObject.bounciness * b1->gameObject.bounciness);
				c1->gameObject.position = NewLocation;

				if (debug) {
					//Draw correct Green location
					CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
					CP_Graphics_DrawCircle(NewLocation.x, NewLocation.y, c1->radius * 2);
				}
			}
			else { // collided on bottom vertex
				Vector2 NewLocation;
				Vector2 ReversedBallVelocity = Normalize(VectorMultiply(c1->gameObject.velocity, -1)); // Reversed Velocity of magnitude 1

				float ReversedDotY = DotProd(ReversedBallVelocity, Normalize(VectorMinus(boxTopLeft, boxBottomLeft)));

				float multiplier = 1 / ReversedDotY;
				ReversedBallVelocity = VectorMultiply(ReversedBallVelocity, multiplier);
				NewLocation = VectorAdd(c1->gameObject.position, VectorMultiply(ReversedBallVelocity, -(boxHeight / 2 + c1->radius) - dotprody));

				// Reflect ball velocity
				Vector2 newVelocityX = VectorProject(c1->gameObject.velocity, VectorMinus(boxTopLeft, boxTopRight));
				Vector2 newVelocityY = VectorMultiply(VectorMinus(c1->gameObject.velocity, newVelocityX), -1);

				Vector2 finalVelocity = VectorAdd(newVelocityX, newVelocityY);
				c1->gameObject.velocity = VectorMultiply(finalVelocity, c1->gameObject.bounciness * b1->gameObject.bounciness);
				c1->gameObject.position = NewLocation;

				if (debug) {
					//Draw correct Green location
					CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
					CP_Graphics_DrawCircle(NewLocation.x, NewLocation.y, c1->radius * 2);
				}
			}
			return true;
		}
		else if (confirmedY && !confirmedX) {
			if (!doPhysics) {
				return true;
			}
			if (collidedLeft) { //collided on left vertex
				Vector2 NewLocation;
				Vector2 ReversedBallVelocity = Normalize(VectorMultiply(c1->gameObject.velocity, -1)); // Reversed Velocity of magnitude 1

				float ReversedDotX = DotProd(ReversedBallVelocity, Normalize(VectorMinus(boxTopLeft, boxTopRight)));

				float multiplier = 1 / ReversedDotX;
				ReversedBallVelocity = VectorMultiply(ReversedBallVelocity, multiplier);
				NewLocation = VectorAdd(c1->gameObject.position, VectorMultiply(ReversedBallVelocity, (boxWidth / 2 + c1->radius) - dotprodx));

				// Reflect ball velocity
				Vector2 newVelocityX = VectorProject(c1->gameObject.velocity, VectorMinus(boxTopLeft, boxBottomLeft));
				Vector2 newVelocityY = VectorMultiply(VectorMinus(c1->gameObject.velocity, newVelocityX), -1);

				Vector2 finalVelocity = VectorAdd(newVelocityX, newVelocityY);
				c1->gameObject.velocity = VectorMultiply(finalVelocity, c1->gameObject.bounciness * b1->gameObject.bounciness);
				c1->gameObject.position = NewLocation;

				if (debug) {
					//Draw correct Green location
					CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
					CP_Graphics_DrawCircle(NewLocation.x, NewLocation.y, c1->radius * 2);
				}
			}
			else { //collided on right vertex
				Vector2 NewLocation;
				Vector2 ReversedBallVelocity = Normalize(VectorMultiply(c1->gameObject.velocity, -1)); // Reversed Velocity of magnitude 1

				float ReversedDotX = DotProd(ReversedBallVelocity, Normalize(VectorMinus(boxTopLeft, boxTopRight)));

				float multiplier = 1 / ReversedDotX;
				ReversedBallVelocity = VectorMultiply(ReversedBallVelocity, multiplier);
				NewLocation = VectorAdd(c1->gameObject.position, VectorMultiply(ReversedBallVelocity, -(boxWidth / 2 + c1->radius) - dotprodx));

				// Reflect ball velocity
				Vector2 newVelocityX = VectorProject(c1->gameObject.velocity, VectorMinus(boxBottomLeft, boxTopLeft));
				Vector2 newVelocityY = VectorMultiply(VectorMinus(c1->gameObject.velocity, newVelocityX), -1);

				Vector2 finalVelocity = VectorAdd(newVelocityX, newVelocityY);
				c1->gameObject.velocity = VectorMultiply( finalVelocity, c1->gameObject.bounciness*b1->gameObject.bounciness) ;
				c1->gameObject.position = NewLocation;

				if (debug) {
					//Draw correct Green location
					CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
					CP_Graphics_DrawCircle(NewLocation.x, NewLocation.y, c1->radius * 2);
				}
			}
			return true;
		}
		
		else {
			if (collidedTop) {
				if (collidedLeft) {
					//check top left point for collision
					if (DistBetweenSquared(boxTopLeft, c1->gameObject.position) < c1->radius * c1->radius) {
						if (!doPhysics) {
							return true;
						}
						else {
							//Collided with top left point!
							CirclePointCol(c1, boxTopLeft, true);
							return true;
						}
					}

				}
				else {
					//check top right point for collision
					if (DistBetweenSquared(boxTopRight, c1->gameObject.position) < c1->radius * c1->radius) {
						if (!doPhysics) {
							return true;
						}
						else {
							//Collided with top right point!
							CirclePointCol(c1, boxTopRight, true);
							return true;
						}
					}
				}
			}
			else {
				if (collidedLeft) {
					//check bottom left point for collision
					if (DistBetweenSquared(boxBottomLeft, c1->gameObject.position) < c1->radius * c1->radius) {
						if (!doPhysics) {
							return true;
						}
						else {
							//Collided with bottom Left point!
							CirclePointCol(c1, boxBottomLeft, true);
							return true;
						}
					}
				}
				else {
					//check bottom right point for collision
					if (DistBetweenSquared(boxBottomRight, c1->gameObject.position) < c1->radius * c1->radius) {
						if (!doPhysics) {
							return true;
						}
						else {
							//Collided with bottom Right point!
							CirclePointCol(c1, boxBottomRight, true);
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// Call this function to check if a point is colliding with a Box Game Object. Useful for buttons and UI.
bool PointRectCol(Vector2 p1, BoxGameObject* b1) {
	Vector2 boxTopLeft = b1->gameObject.position;
	Vector2 boxTopRight = newVector2(boxTopLeft.x + b1->width * cosf(b1->gameObject.angle / 180 * PI), boxTopLeft.y + b1->width * sinf(b1->gameObject.angle / 180 * PI));
	Vector2 boxBottomLeft = newVector2(boxTopLeft.x + b1->height * cosf((b1->gameObject.angle + 90) / 180 * PI), boxTopLeft.y + b1->height * sinf((b1->gameObject.angle + 90) / 180 * PI));
	
	Vector2 boxCenter = newVector2((boxTopRight.x + boxBottomLeft.x) / 2, (boxTopRight.y + boxBottomLeft.y) / 2);

	float boxWidth = b1->width;
	float boxHeight = b1->height;

	Vector2 VectorDist = VectorMinus(p1, boxCenter);

	//Check within X bounds
	float dotprodx = DotProd(Normalize(VectorMinus(boxTopLeft, boxTopRight)), VectorDist);
	float dotprody = DotProd(Normalize(VectorMinus(boxTopLeft, boxBottomLeft)), VectorDist);

	// Check if collided
	bool collided = false;

	if ((dotprodx < boxWidth / 2  && dotprodx > -(boxWidth / 2 )) && (dotprody< boxHeight / 2  && dotprody > -(boxHeight / 2 ))) {
		collided = true;
	}
	else {
		collided = false;
	}
	return collided;
}

void CirclePhys(CircleGameObject* c1) {
	c1->gameObject.velocity.y += gravity * FrameTime;
	//AddForce(&c1->gameObject, newVector2(0, gravity * FrameTime));

	c1->gameObject.position.x += c1->gameObject.velocity.x * FrameTime;
	c1->gameObject.position.y += c1->gameObject.velocity.y * FrameTime;

	if (c1->gameObject.position.x + c1->radius > CP_System_GetWindowWidth() ) {
		c1->gameObject.position.x = CP_System_GetWindowWidth() - c1->radius;
		c1->gameObject.velocity.x *= -1 * c1->gameObject.bounciness;
	}
	else if (c1->gameObject.position.x - c1->radius < 0) {
		c1->gameObject.position.x = 0+ c1->radius;
		c1->gameObject.velocity.x *= -1 * c1->gameObject.bounciness;
	}
	if (c1->gameObject.position.y + c1->radius > CP_System_GetWindowHeight()) {
		c1->gameObject.position.y = CP_System_GetWindowHeight() - c1->radius;
		c1->gameObject.velocity.y *= -1 * c1->gameObject.bounciness;
	}
	else if (c1->gameObject.position.y - c1->radius < 0) {
		c1->gameObject.position.y = 0 + c1->radius;
		c1->gameObject.velocity.y *= -1 * c1->gameObject.bounciness;
	}
}

Vector2 CenterOfBox(BoxGameObject* b1) {
	Vector2 boxTopLeft = b1->gameObject.position;
	Vector2 boxTopRight = newVector2(boxTopLeft.x + b1->width * cosf(b1->gameObject.angle / 180 * PI), boxTopLeft.y + b1->width * sinf(b1->gameObject.angle / 180 * PI));
	Vector2 boxBottomLeft = newVector2(boxTopLeft.x + b1->height * cosf((b1->gameObject.angle + 90) / 180 * PI), boxTopLeft.y + b1->height * sinf((b1->gameObject.angle + 90) / 180 * PI));
	return newVector2((boxTopRight.x + boxBottomLeft.x) / 2, (boxTopRight.y + boxBottomLeft.y) / 2);
}

// Call this function to overwrite the boxGameObject's image over itself (Usually for UI)
void OverriteBoxImage(CP_Image image, BoxGameObject* b, int alpha) {
	Vector2 Center = CenterOfBox(b);
	CP_Image_DrawAdvanced(image, Center.x, Center.y, b->width, b->height, alpha, b->gameObject.angle);
}

// Call this function to draw the boxGameObject's image over itself (Usually for UI)
void DrawBoxImage(BoxGameObject* b, int alpha) {
	Vector2 Center = CenterOfBox(b);
	CP_Image_DrawAdvanced(b->image, Center.x, Center.y, b->width, b->height, alpha, b->gameObject.angle);
}
