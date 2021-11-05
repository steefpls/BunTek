#pragma once
#include "SteefLibrary.h"
#include <string.h>

#define Screen struct screen

#define ButtonObject struct buttonObject

#define LerpDuration 6.0f

#define LineArrayLength 1000
#define CircleGameObjectArrayLength 1000
#define BoxGameObjectArrayLength 1000
#define ButtonObjectArrayLength 50
#define TextLimit 100

float Overlay_bg_opacity = 0;
float Overlay_opacity = 0;

float* overlay_bg_opacity = &Overlay_bg_opacity;
float* overlay_opacity = &Overlay_opacity;


float TimeElapse = 0;

//names of the screen
typedef enum{
	Test_Menu,
	Test_Room,
	Splash_screen,
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
	Options,
	//V make sure this is last in the ENUM
	Total_screen_number,
} Screen_name;
//names of the overlays
typedef enum {
	No_overlay,
	pause_overlay,
	//V make sure this is last in the ENUM
	Total_overlay_number,
	
} Overlay_name;

typedef enum {
	None,
	Move_to_test_room,
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
	Move_to_test_Menu,
	Move_to_main_Menu,
	Move_to_options,
	Pause_Game,
	Restart,
} Button_effects;

struct buttonObject {
	BoxGameObject boxGameObject;
	Button_effects button_effect;
	char buttontext[TextLimit];
};

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
	Overlay_name overlay_name;
	
};
//create button object for screen
ButtonObject CreateButtonObject(Vector2 position, float width, float height,float bounciness, float angle, CP_Image image, CP_Color color, Button_effects effect, char* buttontext) {
	ButtonObject b;
	b.boxGameObject = CreateBoxGameObject(position, width, height, bounciness, angle, image);
	b.boxGameObject.gameObject.color = color;
	b.button_effect = effect;
	strcpy_s(b.buttontext, TextLimit, buttontext);
	return b;
}

//transition to black
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
void Screen_transition(bool* isScreenTransiting, bool* isoverlayActive, float* transition_opacity, Screen_name* Current_screen_name, Screen_name* Next_screen_name, Screen* current_screen, const Screen* screen_array) {
	if (*isScreenTransiting) {//transitioning to new screen
		if (*Current_screen_name != *Next_screen_name) {
			if (screen_transition_to_black(transition_opacity)) {
				*Current_screen_name = *Next_screen_name;
				*current_screen = screen_array[*Current_screen_name];
				*isoverlayActive = false;
				*overlay_bg_opacity = 0;
				*overlay_opacity = 0;
			}
		}
		else {
			*isScreenTransiting = !screen_transition_from_black(transition_opacity);

		}
	}
	printf("Opacity: %f\n", *transition_opacity);
}

//restart transition Control
void Restart_transition(bool* restartingLevel, bool* isoverlayActive, bool* isgamepaused, Screen_name* Current_screen_name, Screen* current_screen, const Screen* screen_array) {
	if (*restartingLevel) {//restart level transitioning
		if (*isoverlayActive == true) {
			if (screen_transition_to_black(overlay_bg_opacity)) {
				*isoverlayActive = false;//turn off overlay
				*current_screen = screen_array[*Current_screen_name];//restart screen
			}
		}
		else if(screen_transition_from_black(overlay_bg_opacity)){
			*restartingLevel = false;//finished restart
			*isgamepaused = false;//unpause the game
		}
	}
	printf("Opacity: %f\n", *overlay_bg_opacity);
}

//Stopwatch
bool Stopwatch(float duration) {
	TimeElapse += FrameTime;
	if (TimeElapse >= duration) {
		TimeElapse = 0;
		return true;
	}
	printf("time: %f\n", TimeElapse);
	return false;
}

void startupsequence(Screen_name* Current_screen_name, Screen_name* Next_screen_name, bool* isScreenTransiting, bool* startup, bool* isgamepaused) {
	switch (*Current_screen_name)
	{
	case Splash_screen:
		if (Stopwatch(2.0f)) {
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
void Drawoverlay(bool* isoverlayTransiting, bool* isoverlayActice, bool* isgamepaused, Overlay_name overlay_name, const Screen* overlay_array) {
	if (overlay_name != No_overlay) {
		//lerp to overlay -- turn on overlay
		if (*isoverlayTransiting && !*isoverlayActice) {//is overlay transition and is overlay not active
			*overlay_bg_opacity = CP_Math_LerpFloat(*overlay_bg_opacity, 150, LerpDuration * FrameTime);
			*overlay_opacity = CP_Math_LerpFloat(*overlay_opacity, 255, LerpDuration * FrameTime);
			if ((int)*overlay_bg_opacity >= 149 && (int)*overlay_opacity >= 254) {
				*isoverlayActice = true;
				*isoverlayTransiting = false;
				*isgamepaused = true;

			}
		}
		//lerp from overlay -- turn off overlay
		else if (*isoverlayTransiting && *isoverlayActice) {//is overlay transition and is overlay active
			*overlay_bg_opacity = CP_Math_LerpFloat(*overlay_bg_opacity, 0, LerpDuration * FrameTime);
			*overlay_opacity = CP_Math_LerpFloat(*overlay_opacity, 0, LerpDuration * FrameTime);
			if ((int)*overlay_bg_opacity == 0 && (int)*overlay_opacity == 0) {
				*isoverlayActice = false;
				*isoverlayTransiting = false;
				*isgamepaused = false;
			}
		}
		//draw the overlay
		if ((*isoverlayTransiting || *isoverlayActice)) {//is overlay transition or is overlay active
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)*overlay_bg_opacity));//set overlay background color
			CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);//draw overlay background
			//draw all overlay buttons
			for (int i = 0; i < ButtonObjectArrayLength; i++)
			{
				ButtonObject* x = &overlay_array[overlay_name].ButtonObjectArray[i];

				if (x->boxGameObject.width == 0 || x->boxGameObject.height == 0)  // Box has no width or height
				{

				}
				else {
					CP_Settings_Fill(CP_Color_Create(255, 255, 255, (int)*overlay_opacity));
					CP_Settings_NoStroke();
					if (x->boxGameObject.image != NULL) {
						DrawBoxImage(&x->boxGameObject, (int)*overlay_opacity);
					}
					else {
						CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
						CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)*overlay_opacity));
						CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
						CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width);
					}

				}
			}
		}
	}
}


