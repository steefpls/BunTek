#pragma once
#include "SteefLibrary.h"
#include <string.h>

#define Screen struct screen

#define ButtonObject struct buttonObject

#define LerpDuration 1.0f

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
	//V make sure this is last in the ENUM
	Total_screen_number,
} Screen_name;

typedef enum {
	Move_to_test_room,
	Move_to_test_Menu,
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
ButtonObject CreateButtonObject(Vector2 position, float width, float height,float bounciness, float angle, CP_Color color, Button_effects effect, char* buttontext) {
	ButtonObject b;
	b.boxGameObject = CreateBoxGameObject(position, width, height, bounciness, angle);
	b.boxGameObject.gameObject.color = color;
	b.button_effect = effect;
	strcpy_s(b.buttontext, TextLimit, buttontext);
	return b;
}

//transition to black
bool screen_transition_to_black(int* transition_oppacity) {
	//fade to black
	TimeElapse += FrameTime*0.5f;
	*transition_oppacity = CP_Math_LerpInt(*transition_oppacity, 255, TimeElapse/LerpDuration);
	CP_Math_ClampInt(*transition_oppacity, 0, 255);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, *transition_oppacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if (*transition_oppacity == 255) {
		TimeElapse = 0;
		return true;
	}
	return false;
}
//transition from black
bool screen_transition_from_black(int* transition_oppacity) {
	//fade from black
	TimeElapse += FrameTime * 0.5f;
	*transition_oppacity = CP_Math_LerpInt(*transition_oppacity, 0, TimeElapse/LerpDuration);
	CP_Math_ClampInt(*transition_oppacity, 0 , 255);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, *transition_oppacity));
	CP_Graphics_DrawRectAdvanced(0, 0, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 0, 0);
	if (*transition_oppacity == 0) {
		TimeElapse = 0;
		return true;
	}
	return false;
}
//transition Control
void screen_transition(bool* isScreenTransiting, int* transition_oppacity, Screen_name* Current_screen_name, Screen_name* Next_screen_name, Screen* current_screen, const Screen* screen_array) {
	if (*isScreenTransiting) {//transitioning to new screen
		if (*Current_screen_name != *Next_screen_name) {
			if (screen_transition_to_black(transition_oppacity)) {
				*Current_screen_name = *Next_screen_name;
				*current_screen = screen_array[*Current_screen_name];
			}
		}
		else {
			*isScreenTransiting = !screen_transition_from_black(transition_oppacity);
		}
	}
	printf("Opacity: %i\n", *transition_oppacity);
	printf("time: %f\n", TimeElapse);
}


