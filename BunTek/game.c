/*---------------------------------------------------------
 * file:	game.c
 * author:	[]
 * email:	[]
*
 * brief:	Main entry point for the sample project
            * of the CProcessing library
*
 * documentation link:
 * https://github.com/DigiPen-Faculty/CProcessing/wiki
*
 * Copyright © 2020 DigiPen, All rights reserved.
* ---------------------------------------------------------*/
#include "SteefLibrary.h"

//Initialize variables




//Initialize functions
void draw_framerate(void);
void DrawAllShapes(void);
void CalculateAllPhysics(void);

//Initialize arrays
CircleGameObject CircleGameObjectArray[50];

//Initialize Variables
CircleGameObject ctest;
CircleGameObject ctest2;


void game_init(void)
{
    if (debug) {
        CP_System_ShowConsole();
        
    }

    ctest = CreateCircleGameObject(newVector2(300,320),newVector2(50,0), 0, CP_Color_Create(0, 0, 0, 255), 30,false,15);
    ctest2 = CreateCircleGameObject(newVector2(700, 300), newVector2(-50, 0), 0, CP_Color_Create(25, 25, 25, 255), 20, false,10);

    CircleGameObjectArray[0] = ctest;
    CircleGameObjectArray[1] = ctest2;
    
    for (int i = 0; i < 50; i++) {
        float scale = CP_Random_RangeFloat(5, 40);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1220), CP_Random_RangeFloat(50, 680)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale);
        CircleGameObjectArray[i] = tempc;
    }

    

}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    DrawAllShapes();
    CalculateAllPhysics();

    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        ctest.gameObject.position.x = CP_Input_GetMouseX();
        ctest.gameObject.position.y = CP_Input_GetMouseY();
    }
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
    {
        ctest2.gameObject.position.x = CP_Input_GetMouseX();
        ctest2.gameObject.position.y = CP_Input_GetMouseY();
    }




    
    
        
    // Profiling info and frameRate testing
    if (debug) draw_framerate();
}

void DrawAllShapes(void)
{
    for (int i = 0; i < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); i++)
    {
        CircleGameObject* x = &CircleGameObjectArray[i];
        if (x == NULL) {
            break;
        }
        else {
            CP_Settings_Fill(x->gameObject.color);
            
            if (x->outline) {
                CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
            }
            else {
                CP_Settings_NoStroke();
            }
            CP_Graphics_DrawCircle(x->gameObject.position.x, x->gameObject.position.y, x->radius * 2);
            if (debug) {
                CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
                CP_Graphics_DrawLine(x->gameObject.position.x, x->gameObject.position.y, x->gameObject.position.x + x->gameObject.velocity.x, x->gameObject.position.y + x->gameObject.velocity.y);
            }
        }
    }
}

void CalculateAllPhysics(void)
{
    
    //Calculating circle-circle physics
    for (int i = 0; i < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); i++)
    {
        if (&CircleGameObjectArray[i] == NULL)
        {
            break;
        }
        CirclePhys(&CircleGameObjectArray[i]);
    }

    //Checking circle-circle collision
    for (int i = 0; i < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); i++)
    {
        CircleGameObject* c1 = &CircleGameObjectArray[i];
        if (c1 == NULL) 
        {
            printf("End of array detected");
            break;
        }
        else 
        {
            for (int o = 0; o < sizeof(CircleGameObjectArray) / sizeof(CircleGameObjectArray[0]); o++)
            {
                CircleGameObject* c2 = &CircleGameObjectArray[o];
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
                        if (CircleCol(c1, c2))
                        {
                            
                            
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




