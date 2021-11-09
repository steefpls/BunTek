#pragma once
//#include "SteefLibrary.h"
#include "Screen_control.h"

//#define Vector2 struct vector2 
#define BOUNCE_LV_1 2.0f
#define COLOR_GREEN CP_Color_Create(0,255,0,255) 
#define COLOR_RED CP_Color_Create(255,0,0,80)


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

	// Basic Information of the platform 
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

	no_draw.width = width; 
	no_draw.height = height; 

	return no_draw; 

}

