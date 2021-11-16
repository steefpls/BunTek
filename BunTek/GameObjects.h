#pragma once
//#include "SteefLibrary.h"
#include "Screen_control.h"

//#define Vector2 struct vector2 
#define BOUNCE_LV_1 2.0f

// Colors (R,G,B,A) A -> '0' (Transparent) --------> '255' (Opaque)
#define COLOR_GREEN CP_Color_Create(0,255,0,255) 
#define COLOR_RED CP_Color_Create(255,0,0,80)
#define COLOR_WHITE CP_Color_Create(255,255,255,255)
#define TEXT_COLOR CP_Color_Create(0,0,0,255)

#define ScoringContainer struct scoringContainer



// Create Object : Super Bounce Platforms
BoxGameObject createSuperBouncePlatform(Vector2 position, float angle, float width, float height) {
	BoxGameObject super_bounce; 
	
	// Setting 'Super-bounce' settings as a <game object> 
	super_bounce.gameObject.position.x = position.x; 
	super_bounce.gameObject.position.y = position.y; 
	super_bounce.gameObject.angle = angle; 
	super_bounce.gameObject.isActive = true; 
	super_bounce.gameObject.color = COLOR_GREEN; // Color Code - Level 1 bounce 
	super_bounce.gameObject.bounciness = BOUNCE_LV_1; 

	// Basic Dimensions of Super Bounce Platform
	super_bounce.width = width; 
	super_bounce.height = height; 

	return super_bounce; 
}

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

//// Converting 'int type' ballcount to 'char * str' type. - for printing in CP_Draw_text -> number on the container. 
//const char* ballCountString(int ballcount) {
//	char countString[100]; 
//	snprintf(countString, 100, "%d", ballcount);
//	return countString; 
//}
//
//

