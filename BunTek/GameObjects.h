/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * file:		GameObjects.h
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:	Contains the functions for creating / initializing game objects into the game. 
 *
* ---------------------------------------------------------*/

#pragma once
#include "Screen_control.h"

//#define Vector2 struct vector2 
#define BOUNCE_LV_1 2.0f

// Colors (R,G,B,A) A -> '0' (Transparent) --------> '255' (Opaque)
#define COLOR_GREEN CP_Color_Create(0,255,0,255) 
#define COLOR_RED CP_Color_Create(255,0,0,80)
#define COLOR_WHITE CP_Color_Create(255,255,255,255)
#define COLOR_BLACK CP_Color_Create(0, 0, 0, 255)
#define COLOR_GREY CP_Color_Create(128, 128, 128 , 255)
#define COLOR_PASTEL_GREEN CP_Color_Create(181, 218, 157, 255)
#define COLOR_PASTEL_PINK CP_Color_Create(235, 167, 167, 255)

#define TEXT_COLOR CP_Color_Create(0,0,0,255)

#define ScoringContainer struct scoringContainer

/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * BoxGameObject createSuperBouncePlatform(Vector2 position, float angle, float width, float height, float bounce)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:       Function to create an entry of a [super-bounce platform] object. 
 *
* ---------------------------------------------------------*/

// Create Object : Super Bounce Platforms
BoxGameObject createSuperBouncePlatform(Vector2 position, float angle, float width, float height, float bounce) {
	BoxGameObject super_bounce;

	// Setting 'Super-bounce' settings as a <game object> 
	super_bounce.gameObject.position.x = position.x;
	super_bounce.gameObject.position.y = position.y;
	super_bounce.gameObject.angle = angle;
	super_bounce.gameObject.isActive = true;
	super_bounce.gameObject.color = COLOR_GREEN; // Color Code - Level 1 bounce 
	super_bounce.gameObject.bounciness = bounce;

	// Basic Dimensions of Super Bounce Platform
	super_bounce.width = width;
	super_bounce.height = height;

	return super_bounce;
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * BoxGameObject createNoDrawZone(Vector2 position, float angle, float width, float height)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:       Function to create an entry of a [no-draw zone] object. 
 *
* ---------------------------------------------------------*/
// Create Object : No Draw Zone 
BoxGameObject createNoDrawZone(Vector2 position, float angle, float width, float height) {
	BoxGameObject no_draw; 
	
	// Setting 'No Draw Zone' properties 
	no_draw.gameObject.position.x = position.x;
	no_draw.gameObject.position.y = position.y; 
	no_draw.gameObject.angle = angle; 
	no_draw.gameObject.isActive = true; 
	no_draw.gameObject.color = COLOR_RED; // How to set this to border colour (?)

	// Basic Dimensions of No-Draw Zones
	no_draw.width = width; 
	no_draw.height = height; 

	return no_draw; 

}

/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * ScoringContainerObject createScoringContainer(Vector2 position, float angle, float width, float height, int ballcount)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:      Function to create an entry of a [scoring container] object. 
 *
* ---------------------------------------------------------*/
// Create Object : Scoring Container 
ScoringContainerObject createScoringContainer(Vector2 position, float angle, float width, float height, int ballcount) {
	ScoringContainerObject scoring_container; 

	// Basic Information of the box
	scoring_container.box.gameObject.position.x = position.x;
	scoring_container.box.gameObject.position.y = position.y; 
	scoring_container.box.gameObject.angle = angle; 
	scoring_container.box.gameObject.isActive = true; 
	scoring_container.box.gameObject.color = COLOR_WHITE; 
	scoring_container.box.gameObject.bounciness = 1; 

	// Basic Dimensions of the box 
	scoring_container.box.width = width; 
	scoring_container.box.height = height;

	// Unique Properties to (Scoring Container)
	scoring_container.ballcountgoal = ballcount;
	scoring_container.textcolor = TEXT_COLOR; // Predefined color w opaque transparency.
	scoring_container.filled = false; 

	return scoring_container; 
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddScoringContainer(Screen* sc, ScoringContainer s1)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:       Initializing an instance of [scoring container] object 
 *				and storing it in [ScoringContainerArray] -> which stores all information of [scoring containers] in the game. 
 *
 *
 *
* ---------------------------------------------------------*/
//add NodrawZone to screen
void AddScoringContainer(Screen* sc, ScoringContainer s1) {
	sc->ScoringContainerArray[sc->ScoringContainerArrayLengthCounter] = s1;
	sc->ScoringContainerArrayLengthCounter++;
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddBallSpawner(Screen* sc, BallSpawner bs1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       This BallSpawner serves as a ball spawner
 *				Initializing an instance of [BallSpawner] object
 *				and storing it in [BallSpawnerArray] -> which stores all information of [Obstacles - BallSpawner] in the game.
 *
 *
* ---------------------------------------------------------*/
//add BallSpawner to screen
void AddBallSpawner(Screen* sc, BallSpawner bs1)
{
	sc->BallSpawnerArray[sc->BallSpawnerArrayLengthCounter] = bs1;
	sc->BallSpawnerArrayLengthCounter++;
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddBoxGameObject(Screen* sc, BoxGameObject b1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       This boxgameobject serves as obstacles (such as walls)
 *				Initializing an instance of [BoxGameObject] object 
 *				and storing it in [BoxGameObjectArray] -> which stores all information of [Obstacles - BoxGameObject] in the game. 
 *
 *
* ---------------------------------------------------------*/
//Add BoxObject to screen
void AddBoxGameObject(Screen* sc, BoxGameObject b1)
{
	sc->BoxGameObjectArray[sc->BoxGameObjectArrayLengthCounter] = b1;
	sc->BoxGameObjectArrayLengthCounter++;
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddBouncePlatform(Screen* sc, BoxGameObject bp1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [super bounce platform] object 
 *				and storing it in [SuperBouncPlatformArray] -> which stores all information of [super bounce platfomrs] in the game. 
 *
* ---------------------------------------------------------*/
//Add Bounce platform to screen
void AddBouncePlatform(Screen* sc, BoxGameObject bp1)
{
	sc->SuperBouncePlatformArray[sc->SuperBouncePlatformArrayLengthCounter] = bp1;
	sc->SuperBouncePlatformArrayLengthCounter++;
}

/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddButton(Screen* sc, ButtonObject b1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [ButtonObject object
 *				and storing it in [ButtonObjectArray] -> which stores all information of [ButtonObjectArray] in the game.
 *
* ---------------------------------------------------------*/
void AddButton(Screen* sc, ButtonObject b1) {
	sc->ButtonObjectArray[sc->ButtonObjectArrayLengthCounter] = b1;
	sc->ButtonObjectArrayLengthCounter++;

}

//add NodrawZone to screen
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddNoDrawZone(Screen* sc, BoxGameObject b1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [BoxGameObject] object
 *				and storing it in [NoDrawZoneArray] -> which stores all information of [No draw zones] in the game.
 *
* ---------------------------------------------------------*/
void AddNoDrawZone(Screen* sc, BoxGameObject b1) {
	sc->NoDrawZoneArray[sc->NoDrawZonesArrayLengthCounter] = b1;
	sc->NoDrawZonesArrayLengthCounter++;

}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddBall(Screen* sc, CircleGameObject c1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [CircleGameObject] object
 *				and storing it in [CircleGameObjectArray] -> which stores all information of [balls] in the game.
 *
* ---------------------------------------------------------*/
void AddBall(Screen* sc, CircleGameObject c1) {
	sc->CircleGameObjectArray[sc->CircleArrayLengthCounter] = c1;
	sc->CircleArrayLengthCounter++;

}

// add scoring container to screen 
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddScoreContainer(Screen* sc, ScoringContainerObject sc1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [ScoringContainerObject] object
 *				and storing it in [ScoringContainerArrayLengthCounter] -> which stores all information of [Scoring Container Object] in the game.
 *
* ---------------------------------------------------------*/
void AddScoreContainer(Screen* sc, ScoringContainerObject sc1) {
	sc->ScoringContainerArray[sc->ScoringContainerArrayLengthCounter] = sc1; // Accessing the particular scoring container array's position, and storing the newly defined scoring container object. 
	sc->ScoringContainerArrayLengthCounter++;
}
/*---------------------------------------------------------
 * Copyright © 2021 DigiPen, All rights reserved.
 *
 * void AddCircleportalpair(Screen* sc, Circleportalpair cpp1)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:       Initializing an instance of [Circleportalpair] object
 *				and storing it in [CircleportalpairArray] -> which stores all information of [Circle portal pairs] in the game.
 *
* ---------------------------------------------------------*/
void AddCircleportalpair(Screen* sc, Circleportalpair cpp1) {
	sc->CircleportalpairArray[sc->CircleportalpairArrayLengthCounter] = cpp1;
	sc->CircleportalpairArrayLengthCounter++;
}