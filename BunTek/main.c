/*---------------------------------------------------------
 * file:	main.c
 * author:	[]
 * email:	[]
* 
 * brief:	Main entry point for the sample project
			* of the CProcessing library
* 
 * documentation link:
 * https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
* 
 * Copyright © 2020 DigiPen, All rights reserved.
* ---------------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "cprocessing.h"

//Initialize variables
bool drawFPS = true;
int recommendedWidth = 1280;
int recommendedHeight = 720;
float frameTime = 0;

//Initialize functions
void draw_framerate(void);

//Initialize structs


void game_init(void)
{
	
	
    
	
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));

    frameTime = CP_System_GetDt();





	// Profiling info and frameRate testing
	if (drawFPS)
	{
        draw_framerate();
	}
}


void game_exit(void)
{
    
}



int main(void)
{
    CP_System_SetFrameRate(165.0f);
    CP_System_SetWindowSize(recommendedWidth, recommendedHeight);

    CP_Engine_SetNextGameState(game_init, game_update, game_exit);
    CP_Engine_Run();
    return 0;
}

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