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

#define CircleGameObjectArrayLength 1000

CircleGameObject CircleGameObjectArray[CircleGameObjectArrayLength];
//Initialize Variables



void game_init(void)
{
    if (debug) {
        CP_System_ShowConsole();
    }

    
    
    for (int i = 0; i < 100; i++) {
        float scale = CP_Random_RangeFloat(1, 2);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1220), CP_Random_RangeFloat(50, 680)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale,0.95f);
        CircleGameObjectArray[i] = tempc;
    }

    

}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    DrawAllShapes();
    CalculateAllPhysics();




    
    
        
    // Profiling info and frameRate testing
    if (debug) draw_framerate();
}

void DrawAllShapes(void)
{
    for (int i = 0; i < CircleGameObjectArrayLength; i++)
    {
        CircleGameObject* x = &CircleGameObjectArray[i];

       
        if ( x->radius == 0.0f) // If radius of thing is 0
        {
            
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
    
    //Calculating circle physics
    for (int i = 0; i < CircleGameObjectArrayLength; i++)
    {
        if (CircleGameObjectArray[i].radius == 0.0f) // If radius of thing is 0
        {
            break;
        }
                
        CirclePhys(&CircleGameObjectArray[i]);
    }

    //Checking circle-circle collision
    for (int i = 0; i < CircleGameObjectArrayLength; i++)
    {
        CircleGameObject* c1 = &CircleGameObjectArray[i];
        if (CircleGameObjectArray[i].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
        {
            //End of Array
            break;
        }
        else 
        {
            for (int o = i+1; o < CircleGameObjectArrayLength; o++)
            {
                CircleGameObject* c2 = &CircleGameObjectArray[o];
                if (CircleGameObjectArray[o].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
                {
                    //End of Array
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




