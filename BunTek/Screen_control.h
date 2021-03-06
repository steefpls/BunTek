/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 * 
 * file:		Screen_control.h
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 * Co-author:	[Steven Koe]
 * email:	    [steven.k@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 * Co-author:	[Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * 
 * brief:	Contains the functions for screen flow control and screen creation and transition
 * 
* ---------------------------------------------------------*/
#pragma once
#include "SteefLibrary.h"
#include <string.h>

#define Screen struct screen

#define ButtonObject struct buttonObject
#define ButtonbgInfo struct buttonbgInfo

#define Screen_keys struct screen_keys

#define LerpDuration 9.0f
#define OverlaypDuration 14.0f
#define ButtonLerpDuration 18.0f

#define LineArrayLength 1000
#define CircleGameObjectArrayLength 1000
#define BoxGameObjectArrayLength 1000
#define ButtonObjectArrayLength 50
#define TextLimit 100
#define SpawnerGameObjectArrayLength 1000
#define VolumeObjectArrayLength 10
#define PortalArrayLength 10

float Overlay_bg_opacity = 0;
float Overlay_opacity = 0;

float* overlay_bg_opacity = &Overlay_bg_opacity;
float* overlay_opacity = &Overlay_opacity;



float TimeElapse = 0;

//names of the screen
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Enum:	enum Screen_name
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 * Co-author:	[Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:	Contains the name of all the screens
 *
* ---------------------------------------------------------*/
typedef enum Screen_name {
	No_screen,
	Splash_screen,
	Splash_Logo,
	Main_menu,
	Level_Select,
	Level_1,
	Level_2,
	Level_3,
	Level_4,
	Level_5,
	Level_6,
	Level_7,
	Level_8,
	Level_9,
	Level_10,
	Level_1_title,
	Level_2_title,
	Level_3_title,
	Level_4_title,
	Level_5_title,
	Level_6_title,
	Level_7_title,
	Level_8_title,
	Level_9_title,
	Level_10_title,
	Options,
	Credits_screen,
	Credits_screen2,
	Credits_screen3,
	Tutorial,
	//V make sure this is last in the ENUM
	Total_screen_number,
} Screen_name;


//names of the overlays
//names of the screen
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Enum:	enum Overlay_name
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Contains the name of all the overlay screens
 *
* ---------------------------------------------------------*/
typedef enum Overlay_name{
	No_overlay,
	pause_overlay,
	victory_overlay,
	//V make sure this is last in the ENUM
	Total_overlay_number,
	
} Overlay_name;
//names of the screen
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Enum:	enum Button_effects 
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 * Co-author:	[Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:	Contains the name of all the button effects
 *
* ---------------------------------------------------------*/
typedef enum Button_effects {
	None,
	Move_to_Level_Select,
	Move_to_Level_1,
	Move_to_Level_2,
	Move_to_Level_3,
	Move_to_Level_4,
	Move_to_Level_5,
	Move_to_Level_6,
	Move_to_Level_7,
	Move_to_Level_8,
	Move_to_Level_9,
	Move_to_Level_10,
	Move_to_main_Menu,
	Move_to_options,
	Move_to_credits,
	Move_to_credits2,
	Move_to_credits3,
	Move_to_tutorial,
	Next_Level,
	Pause_Game,
	Restart,
	Exit_game,
	Increase_Volume_BGM, 
	Decrease_Volume_BGM,
	Increase_Volume_SFX, 
	Decrease_Volume_SFX, 
} Button_effects;

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Struct:	struct buttonbgInfo
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Defination of Button Background Info struct
 *
* ---------------------------------------------------------*/
struct buttonbgInfo {
	CP_Image border;
	CP_Image hoverbg;
	CP_Image clickedbg;
	float borderalpha;
	float hoveralpha;
	float clickedalpha;
	
};

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Struct:	struct screen_keys
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Defination of Screen Keys struct
 *
* ---------------------------------------------------------*/
struct screen_keys {
	Button_effects key;
	Screen_name value;
	Screen_name subvalue;
};

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Struct:	struct buttonObject
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Defination of Button object struct
 *
* ---------------------------------------------------------*/
struct buttonObject {
	BoxGameObject boxGameObject;
	Button_effects button_effect;
	CP_Color TextColor;
	char buttontext[TextLimit];
	ButtonbgInfo buttonbgInfo;
};

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Struct:	struct volume object
 * author:	[Cheong Ming Lun]
 * email:	[m.cheong@digipen.edu]
 *
 * brief:	Defination of screen struct
 *
* ---------------------------------------------------------*/
typedef struct volumeobject {
	BoxGameObject boxGameObject;
	Vector2 vector;
	bool lit;
} volumeObject;

typedef volumeObject volume[2];

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Struct:	struct screen 
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 * Co-author:	[Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:	Defination of screen struct
 *
* ---------------------------------------------------------*/
struct screen {
	
	//Array for all collidable circles
	int CircleArrayLengthCounter;
	CircleGameObject CircleGameObjectArray[CircleGameObjectArrayLength];

	//Array for all collidable Line
	int LineArrayLengthCounter;
	BoxGameObject LineArray[LineArrayLength];

	//Array for all collidable rects
	int BoxGameObjectArrayLengthCounter;
	BoxGameObject BoxGameObjectArray[BoxGameObjectArrayLength];

	//Array for all buttons
	int ButtonObjectArrayLengthCounter;
	ButtonObject ButtonObjectArray[ButtonObjectArrayLength];

	// Array for all no-draw zones
	int NoDrawZonesArrayLengthCounter; 
	BoxGameObject NoDrawZoneArray[BoxGameObjectArrayLength]; 

	// Array for all portal pairs
	int CircleportalpairArrayLengthCounter;
	Circleportalpair CircleportalpairArray[PortalArrayLength];
	
	// Array for super-bounce platforms 
	int SuperBouncePlatformArrayLengthCounter;
	BoxGameObject SuperBouncePlatformArray[BoxGameObjectArrayLength]; 

	// Array for scoring containers 
	int ScoringContainerArrayLengthCounter; 
	ScoringContainerObject ScoringContainerArray[BoxGameObjectArrayLength]; 

	//overlay name
	Overlay_name overlay_name;

	//Array for all Spawners
	int BallSpawnerArrayLengthCounter;
	BallSpawner BallSpawnerArray[SpawnerGameObjectArrayLength];
	
	// Array of Volume Components 
	int VolumeObjectArrayLengthCounter; 
	VolumeObject VolumeObjectArray[VolumeObjectArrayLength];
};


/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	VolumeObject CreateVolumeObject(Vector2 position, float width, float height, CP_Color color, bool litbool)
 * author:		[Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 *
 * brief:	Creates an instance of a volume object and returns it
 *
* ---------------------------------------------------------*/
VolumeObject CreateVolumeObject(Vector2 position, float width, float height, CP_Color color, bool litbool) {
	VolumeObject v; 
	v.boxGameObject = CreateBoxGameObject(position, width, height, 0, 0, NULL, color); 
	v.lit = litbool;
	return v; 
}
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	ButtonObject CreateButtonObject(Vector2 position, float width, float height,float bounciness, float angle, CP_Image image, 
				CP_Color color, Button_effects effect, CP_Color textcolor, char* buttontext, ButtonbgInfo background)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Creates an instance of a button object and returns it
 *
* ---------------------------------------------------------*/
ButtonObject CreateButtonObject(Vector2 position, float width, float height,float bounciness, float angle, CP_Image image, CP_Color color, Button_effects effect, CP_Color textcolor, char* buttontext, ButtonbgInfo background) {
	ButtonObject b;
	b.boxGameObject = CreateBoxGameObject(position, width, height, bounciness, angle, image,color);
	b.boxGameObject.gameObject.color = color;
	b.button_effect = effect;
	b.TextColor = textcolor;
	strcpy_s(b.buttontext, TextLimit, buttontext);
	b.buttonbgInfo = background;
	return b;
}

//transition to black
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	bool screen_transition_to_black(float* transition_opacity)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Controls the animations for the transitions to black
 *
* ---------------------------------------------------------*/
bool screen_transition_to_black(float* transition_opacity) {
	//fade to black
	*transition_opacity = CP_Math_LerpFloat(*transition_opacity, 255, LerpDuration * FrameTime);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int) *transition_opacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if ((int)*transition_opacity >= 254) {
		return true;
	}
	return false;
}
//transition from black
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	bool screen_transition_from_black(float* transition_opacity)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Controls the animations for the transitions from black
 *
* ---------------------------------------------------------*/
bool screen_transition_from_black(float* transition_opacity) {
	//fade from black
	*transition_opacity = CP_Math_LerpFloat(*transition_opacity, 0, LerpDuration * FrameTime);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)*transition_opacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if ((int)*transition_opacity == 0) {
		return true;
	}
	return false;
}
//transition Control
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void Screen_transition(bool* isScreenTransiting, bool* isoverlayActive, float* transition_opacity, Screen_name* Current_screen_name, 
				Screen_name* Next_screen_name, Screen* current_screen, const Screen* screen_array,
				Overlay_name* current_overlay_name ,Screen* current_screen_overlay, const Screen* overlay_array)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Controls the animations for the transitions between different screens
 *
* ---------------------------------------------------------*/
void Screen_transition(bool* isScreenTransiting, bool* isoverlayActive, float* transition_opacity, Screen_name* Current_screen_name, Screen_name* Next_screen_name, Screen* current_screen, const Screen* screen_array, Overlay_name* current_overlay_name ,Screen* current_screen_overlay, const Screen* overlay_array) {
	if (*isScreenTransiting) {//transitioning to new screen
		if (*Current_screen_name != *Next_screen_name) {
			if (screen_transition_to_black(transition_opacity)) {
				*Current_screen_name = *Next_screen_name;
				*current_screen = screen_array[*Current_screen_name];
				*isoverlayActive = false;
				*overlay_bg_opacity = 0;
				*overlay_opacity = 0;
				*current_overlay_name = current_screen->overlay_name;
				if (current_screen->overlay_name != No_overlay) {
					*current_screen_overlay = overlay_array[current_screen->overlay_name];
				}
				else {
					*current_screen_overlay = overlay_array[No_overlay];
				}

			}
		}
		else {
			*isScreenTransiting = !screen_transition_from_black(transition_opacity);

		}
	}
	//printf("Opacity: %f\n", *transition_opacity);
}

//restart transition Control
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void Restart_transition(bool* restartingLevel, bool* isoverlayActive, bool* isgamepaused, Screen_name* Current_screen_name, Screen* current_screen,
				const Screen* screen_array, Overlay_name* current_overlay_name, Screen* current_screen_overlay, const Screen* overlay_array)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Controls the animations for the transitions to restart a level
 *
* ---------------------------------------------------------*/
void Restart_transition(bool* restartingLevel, bool* isoverlayActive, bool* isgamepaused, Screen_name* Current_screen_name, Screen* current_screen, const Screen* screen_array, Overlay_name* current_overlay_name, Screen* current_screen_overlay, const Screen* overlay_array) {
	if (*restartingLevel) {//restart level transitioning
		if (*isoverlayActive == true) {
			if (screen_transition_to_black(overlay_bg_opacity)) {
				*isoverlayActive = false;//turn off overlay
				*current_screen = screen_array[*Current_screen_name];//restart screen
				*current_overlay_name = current_screen->overlay_name;
				*current_screen_overlay = overlay_array[current_screen->overlay_name];//reset array
			}
		}
		else if(screen_transition_from_black(overlay_bg_opacity)){
			*restartingLevel = false;//finished restart
			*isgamepaused = false;//unpause the game
		}
	}
	//printf("Opacity: %f\n", *overlay_bg_opacity);
}

//Stopwatch
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	bool Stopwatch(float duration)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Stopwatch to count the time passed
 *
* ---------------------------------------------------------*/
bool Stopwatch(float duration) {
	TimeElapse += FrameTime;
	if (TimeElapse >= duration) {
		TimeElapse = 0;
		return true;
	}
	return false;
}
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void startupsequence(Screen_name* Current_screen_name, Screen_name* Next_screen_name, bool* isScreenTransiting, bool* startup, bool* isgamepaused)
 * author:	    [Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:	Controls the startup display at the beginning of the game
 *
* ---------------------------------------------------------*/
void startupsequence(Screen_name* Current_screen_name, Screen_name* Next_screen_name, bool* isScreenTransiting, bool* startup, bool* isgamepaused) {
	switch (*Current_screen_name)
	{
	case Splash_screen:
		if (Stopwatch(3.0f)) {
			*Next_screen_name = Splash_Logo;
			*isScreenTransiting = true;
		}
		break;
	case Splash_Logo:
		if (Stopwatch(3.0f)) {
			*Next_screen_name = Main_menu;
			*isScreenTransiting = true;
		}
		break;
	default:
		*isgamepaused = false;
		*startup = false;
		break;
	}
}
//draw button badcjground sprite
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void DrawButtonBackgroundImage(BoxGameObject* b, ButtonbgInfo* bg, bool* isoverlayTransiting)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Draws the background images for the buttons
 *
* ---------------------------------------------------------*/
void DrawButtonBackgroundImage(BoxGameObject* b, ButtonbgInfo* bg, bool* isoverlayTransiting) {
	Vector2 Center = CenterOfBox(b);
	if (*isoverlayTransiting) {
		
		CP_Image_DrawAdvanced(bg->border, Center.x, Center.y, b->width, b->height, (int)*overlay_opacity, b->gameObject.angle);
	}
	else {
		CP_Image_DrawAdvanced(bg->hoverbg, Center.x, Center.y, b->width, b->height, (int)bg->hoveralpha, b->gameObject.angle);
		CP_Image_DrawAdvanced(bg->clickedbg, Center.x, Center.y, b->width, b->height, (int)bg->clickedalpha, b->gameObject.angle);
		CP_Image_DrawAdvanced(bg->border, Center.x, Center.y, b->width, b->height, (int)bg->borderalpha, b->gameObject.angle);
	}
}
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void Drawoverlay(bool* isoverlayTransiting, bool* isoverlayActive, bool* isgamepaused, Overlay_name overlay_name, Screen* currentOverLay)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 * Co-author:	[Goh Kiat Beng]
 * email:	    [kiatbeng.goh@digipen.edu]
 *
 * brief:	Controls the transitions and draws the screen overlays
 *
* ---------------------------------------------------------*/
void Drawoverlay(bool* isoverlayTransiting, bool* isoverlayActive, bool* isgamepaused, Overlay_name overlay_name, Screen* currentOverLay) {
	
	if (overlay_name != No_overlay) {
		//lerp to overlay -- turn on overlay
		if (*isoverlayTransiting && !*isoverlayActive) {//is overlay transition and is overlay not active
			*overlay_bg_opacity = CP_Math_LerpFloat(*overlay_bg_opacity, 150, OverlaypDuration * FrameTime);
			*overlay_opacity = CP_Math_LerpFloat(*overlay_opacity, 255, OverlaypDuration * FrameTime);
			if ((int)*overlay_bg_opacity >= 149 && (int)*overlay_opacity >= 254) {
				*isoverlayActive = true;
				*isoverlayTransiting = false;
				*isgamepaused = true;

			}
		}
		//lerp from overlay -- turn off overlay
		else if (*isoverlayTransiting && *isoverlayActive) {//is overlay transition and is overlay active
			*overlay_bg_opacity = CP_Math_LerpFloat(*overlay_bg_opacity, 0, OverlaypDuration * FrameTime);
			*overlay_opacity = CP_Math_LerpFloat(*overlay_opacity, 0, OverlaypDuration * FrameTime);
			if ((int)*overlay_bg_opacity == 0 && (int)*overlay_opacity == 0) {
				*isoverlayActive = false;
				*isoverlayTransiting = false;
				*isgamepaused = false;
			}
		}
		//draw the overlay
		if ((*isoverlayTransiting || *isoverlayActive)) {//is overlay transition or is overlay active
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)*overlay_bg_opacity));//set overlay background color
			CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);//draw overlay background
			//draw all overlay buttons
			for (int i = 0; i < currentOverLay->ButtonObjectArrayLengthCounter; i++)
			{
				ButtonObject* x = &currentOverLay->ButtonObjectArray[i];

				if (x->boxGameObject.width == 0 || x->boxGameObject.height == 0)  // Box has no width or height
				{

				}
				else {
					CP_Settings_Fill(x->boxGameObject.gameObject.color);
					CP_Settings_NoStroke();
					//draw background sprites
					DrawButtonBackgroundImage(&x->boxGameObject, &x->buttonbgInfo , isoverlayTransiting);
					if (x->boxGameObject.image != NULL) {
						DrawBoxImage(&x->boxGameObject, (int)*overlay_opacity);
					}
					else {
						CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
						//overlay transiting text color
						if (*isoverlayTransiting){
							CP_Settings_Fill(CP_Color_Create(255, 255, 255, (int)*overlay_opacity));
						}
						//overlat active text color
						else {
							CP_Settings_Fill(x->TextColor);
						}
						CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
						CP_Settings_TextSize(20);
						CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y + x->boxGameObject.height / 3, x->boxGameObject.width);
					}
				}
			}
		}
	}
}//

/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void ButtonLerp(Vector2 MousePos, bool* isoverlayActive,  Screen* current_screen, Screen* currentOverLay)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Lerp the buttons colour
 *
* ---------------------------------------------------------*/
void ButtonLerp(Vector2 MousePos, bool* isoverlayActive,  Screen* current_screen, Screen* currentOverLay) {
	//lerp overlay buttons
	if (*isoverlayActive) {
		for (int i = 0; i < currentOverLay->ButtonObjectArrayLengthCounter; i++)
		{
			ButtonObject* x = &currentOverLay->ButtonObjectArray[i];
			//if mouse is coliding with button
			if (PointRectCol(MousePos, &x->boxGameObject)) {
				//button is clicked
				if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
					//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(x->buttonbgInfo.borderalpha, 0, ButtonLerpDuration * FrameTime
					x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 0, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 255, ButtonLerpDuration * FrameTime);
					x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(0, 0, 0, 255), ButtonLerpDuration * FrameTime);
				}
				//button hover
				else
				{
					//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(x->buttonbgInfo.borderalpha, 0, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 255, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 0, ButtonLerpDuration * FrameTime);
					x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(0, 0, 0, 255), ButtonLerpDuration * FrameTime);
				}
			}
			// lerp to default
			else
			{
				//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(*overlay_bg_opacity, 0, ButtonLerpDuration * FrameTime);
				x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 0, ButtonLerpDuration * FrameTime);
				x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 0, ButtonLerpDuration * FrameTime);
				x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(255, 255, 255, 255), ButtonLerpDuration * FrameTime);
			}
		}
	}
	//lerp screen button
	else if (!*isoverlayActive) {
		for (int i = 0; i < current_screen->ButtonObjectArrayLengthCounter; i++)
		{
			ButtonObject* x = &current_screen->ButtonObjectArray[i];
			//if mouse is coliding with button
			if (PointRectCol(MousePos, &x->boxGameObject)) {
				//button is clicked
				if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
					//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(x->buttonbgInfo.borderalpha, 0, ButtonLerpDuration * FrameTime
					x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 0, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 255, ButtonLerpDuration * FrameTime);
					x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(0, 0, 0, 255), ButtonLerpDuration * FrameTime);
				}
				//button hover
				else
				{
					//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(x->buttonbgInfo.borderalpha, 0, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 255, ButtonLerpDuration * FrameTime);
					x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 0, ButtonLerpDuration * FrameTime);
					x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(0, 0, 0, 255), ButtonLerpDuration * FrameTime);
				}
			}
			// lerp to default
			else
			{
				//x->buttonbgInfo.borderalpha = CP_Math_LerpFloat(*overlay_bg_opacity, 0, ButtonLerpDuration * FrameTime);
				x->buttonbgInfo.hoveralpha = CP_Math_LerpFloat(x->buttonbgInfo.hoveralpha, 0, ButtonLerpDuration * FrameTime);
				x->buttonbgInfo.clickedalpha = CP_Math_LerpFloat(x->buttonbgInfo.clickedalpha, 0, ButtonLerpDuration * FrameTime);
				x->TextColor = CP_Color_Lerp(x->TextColor, CP_Color_Create(255, 255, 255, 255), ButtonLerpDuration * FrameTime);
			}
		}
	}
}
//add Screen keys
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Function:	void AddScreenKey(int* count, Button_effects key, Screen_name value, Screen_name subvalue, Screen_keys* Screen_key_array)
 * author:	    [Bryan Boh]
 * email:	    [b.boh@digipen.edu]
 *
 * brief:	Add screen keys to the screen key array
 *
* ---------------------------------------------------------*/
void AddScreenKey(int* count, Button_effects key, Screen_name value, Screen_name subvalue, Screen_keys* Screen_key_array) {
	if (*count < Total_screen_number) {
		Screen_keys sk;
		sk.key = key;
		sk.value = value;
		sk.subvalue = subvalue;
		Screen_key_array[*count] = sk;
		(*count)++;
	}
}
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * Vector2 RemoveBall(Screen* sc, CircleGameObject c1)
 * author:	    [Steven Koe]
 * email:	    [steven.k@digipen.edu]
 *
 * brief:       Remove a ball in the array
 *
* ---------------------------------------------------------*/
Vector2 RemoveBall(Screen* sc, CircleGameObject c1) {
	for (int i = 0; i < sc->CircleArrayLengthCounter; i++) {
		if (sc->CircleGameObjectArray[i].gameObject.position.x == c1.gameObject.position.x && sc->CircleGameObjectArray[i].gameObject.position.y == c1.gameObject.position.y) {
			sc->CircleArrayLengthCounter--;
			if (sc->CircleArrayLengthCounter > 0) {
				sc->CircleGameObjectArray[i] = sc->CircleGameObjectArray[sc->CircleArrayLengthCounter];
			}
		}
	}
	return (c1.gameObject.position);
}
/*---------------------------------------------------------
 * Copyright ? 2021 DigiPen, All rights reserved.
 *
 * void AddCircleportalpair(Screen* sc, Circleportalpair cpp1)
 * author:	    [Steven Koe]
 * email:	    [steven.k@digipen.edu]
 *
 * brief:      Remove balls the are out of bounds of the screen
 *
* ---------------------------------------------------------*/
void RemoveOOB(Screen* sc, CircleGameObject c1) {
	if (c1.gameObject.position.x - c1.radius > CP_System_GetWindowWidth()) {
		RemoveBall(sc, c1);
	}
	else if (c1.gameObject.position.x + c1.radius < 0) {
		RemoveBall(sc, c1);
	}
	if (c1.gameObject.position.y - c1.radius > CP_System_GetWindowHeight()) {
		RemoveBall(sc, c1);
	}
	else if (c1.gameObject.position.y + c1.radius < 0) {
		RemoveBall(sc, c1);
	}
}