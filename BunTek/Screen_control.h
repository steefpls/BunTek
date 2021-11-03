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
bool screen_transition_to_black(float* transition_oppacity) {
	//fade to black
	TimeElapse += FrameTime;
	
	*transition_oppacity = CP_Math_LerpFloat(*transition_oppacity, 255, LerpDuration * FrameTime);

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int) *transition_oppacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if ((int)*transition_oppacity >= 254) {
		TimeElapse = 0;
		return true;
	}
	return false;
}
//transition from black
bool screen_transition_from_black(float* transition_oppacity) {
	//fade from black
	TimeElapse += FrameTime;
	*transition_oppacity = CP_Math_LerpFloat(*transition_oppacity, 0, LerpDuration * FrameTime);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, (int)*transition_oppacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if ((int)*transition_oppacity == 0) {
		TimeElapse = 0;
		return true;
	}
	return false;
}

//transition Control
void screen_transition(bool* isScreenTransiting, float* transition_oppacity, Screen_name* Current_screen_name, Screen_name* Next_screen_name, Screen* current_screen, const Screen* screen_array) {
	if (*isScreenTransiting) {//transitioning to new screen
		if (*Current_screen_name != *Next_screen_name) {
			if (screen_transition_to_black(transition_oppacity)) {
				*Current_screen_name = *Next_screen_name;
				*current_screen = screen_array[*Current_screen_name];
			}
		}
		else if (*Next_screen_name == Options)
		{
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 0));
			CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
		}
		else {
			*isScreenTransiting = !screen_transition_from_black(transition_oppacity);
		}
	}
	printf("Opacity: %f\n", *transition_oppacity);
	printf("time: %f\n", TimeElapse);
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


