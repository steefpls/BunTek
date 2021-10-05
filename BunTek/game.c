/*---------------------------------------------------------
 * file:	game.c
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
#include "SteefLibrary.h"

//Initialize variables
bool drawFPS = false;

float frameTime = 0;

//Initialize functions
void draw_framerate(void);
void DrawAllShapes(void);
void CalculateAllPhysics(void);

//Initialize arrays
CircleGameObject* CircleGameObjectArray[100];

//Initialize Variables
CircleGameObject ctest;
CircleGameObject ctest2;

void game_init(void)
{
    CP_System_ShowConsole();
    ctest = CreateCircleGameObject(newVector2(50,50),newVector2(5,-5), 0, CP_Color_Create(255, 255, 255, 255), 20);
    ctest2 = CreateCircleGameObject(newVector2(40, 40), newVector2(5, -5), 0, CP_Color_Create(255, 255, 255, 255), 20);


    CircleGameObjectArray[0] = &ctest;
    CircleGameObjectArray[1] = &ctest2;


}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    DrawAllShapes();
    CalculateAllPhysics();
    frameTime = CP_System_GetDt();







    // Profiling info and frameRate testing
    if (drawFPS)
    {
        draw_framerate();
    }
}

void DrawAllShapes(void)
{
    for (int i = 0; i < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); i++)
    {
        CircleGameObject* x = CircleGameObjectArray[i];
        if (x == NULL) {
            break;
        }
        else {
            CP_Settings_Fill(x->gameObject.color);

            CP_Graphics_DrawCircle(x->gameObject.position.x, x->gameObject.position.y, x->radius * 2);

        }
    }
}

void CalculateAllPhysics(void)
{
    
    
    
    
    
    
    for (int i = 0; i < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); i++)
    {
        //Checking circle-circle collision
        CircleGameObject* c1 = CircleGameObjectArray[i];
        if (c1 == NULL) 
        {
            break;
        }
        else 
        {
            for (int o = 0; o < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); o++)
            {
                CircleGameObject* c2 = CircleGameObjectArray[o];
                if (c2 == NULL) 
                {
                    break;
                }
                else 
                {
                    if (c1 == c2) 
                    {
                        continue;
                    }
                    else 
                    {
                        if (CircleCol(*c1, *c2)) 
                        {
                            printf("Collided!");
                            //If collided
                        }
                    }

                }

            }
        }
    }
}


void game_exit(void)
{

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