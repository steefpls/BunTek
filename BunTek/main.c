/*---------------------------------------------------------
 * file:	game.c
 * author:	[Steven Koe]
 * email:	[steven.k@digipen.edu]
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
#include "cprocessing.h"

int recommendedWidth = 1920;
int recommendedHeight = 1080;

/*Forward declarations*/

void game_init(void);
void game_update(void);
void game_exit(void);

int main(void)
{
    CP_System_SetFrameRate(165.0f);
    //CP_System_Fullscreen();
    CP_System_SetWindowSize(recommendedWidth, recommendedHeight);

    CP_Engine_SetNextGameState(game_init, game_update, game_exit);
    CP_Engine_Run();
    return 0;
}