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
bool Mouse1Held = false; //mouse held bool for previous frame
Vector2 MousePosPrev;
Vector2 MousePos;
Vector2 AveragePos;
int LineCounter = 0;
float LineTimer;
float LineLength = 5;
float NewLineInterval = 0.05f;
Vector2 LineStartPos;
Vector2 LineEndPos;
const float DrawnLineBounciness = 1.0f;

//Initialize functions
void draw_framerate(void);
void DrawAllShapes(void);
void CalculateAllPhysics(void);
void AddLine(void);

//Initialize arrays

//Array for all collidable circles
int CircleArrayLengthCounter = 0;
#define CircleGameObjectArrayLength 1000
CircleGameObject CircleGameObjectArray[CircleGameObjectArrayLength];

//Array for all collidable rects
int LineArrayLengthCounter = 0;
#define LineArrayLength 1000
BoxGameObject LineArray[LineArrayLength];




void game_init(void)
{
    if (debug) {
        CP_System_ShowConsole();
    }
    
    //Fill up array with test circles
    for (int i = 0; i < 200; i++) {
        float scale = CP_Random_RangeFloat(10, 15);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1800), CP_Random_RangeFloat(50, 1000)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale,1);
        CircleGameObjectArray[i] = tempc;
    }

    

}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    MousePos = newVector2(CP_Input_GetMouseX(), CP_Input_GetMouseY());
    AddLine();
    
    DrawAllShapes();
    CalculateAllPhysics();

    printf("Mouse 1 held: %d\n",Mouse1Held);


    
    
        
    // Profiling info and frameRate testing
    if (debug) draw_framerate();
}

void AddLine(void) {
    
    if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
        
        if (!Mouse1Held) // New Line needs to be made
        {
            LineTimer = 0;
            LineStartPos = MousePos;
            LineEndPos = MousePos;
            LineLength = 0;
        }
        else //continue from previous line
        {
            LineTimer += FrameTime;
            if ((LineTimer >= NewLineInterval && LineLength>10)||LineLength>100)
            {
                LineStartPos = LineEndPos;
                if (LineCounter + 1 < LineArrayLength) {
                    LineCounter += 1;
                }
                LineTimer = 0;
            }
            LineEndPos = MousePos;

            LineLength = DistBetween(LineStartPos, LineEndPos);
        }
        if (MousePos.x != MousePosPrev.x || MousePos.y != MousePosPrev.y) {
            LineArray[LineCounter] = CreateBoxGameObject(LineStartPos, LineLength, 4.0f, DrawnLineBounciness,atan2f(LineEndPos.y - LineStartPos.y, LineEndPos.x - LineStartPos.x) / (float)PI * 180);
        }
    }
    else if (Mouse1Held) 
    {
        if (LineCounter +1 < LineArrayLength) {
            LineCounter += 1;
        }
    }
    Mouse1Held = CP_Input_MouseDown(MOUSE_BUTTON_1);
    MousePosPrev = MousePos;
    
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

    for (int i = 0; i < LineArrayLength; i++)
    {
        BoxGameObject* x = &LineArray[i];

        if (x->width == 0 || x->height == 0)  // Box has no width or height
        {
            
        }
        else {
            CP_Settings_Fill(x->gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle,1);

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
                        if (CircleCol(c1, c2))  //If circle collides with cicle
                        {
                           
                            
                        }
                    }
                }
            }
        }
        
    }
    //Checking box-circle collision
    for (int x = 0; x < LineArrayLength; x++) {
        BoxGameObject* b1 = &LineArray[x];
        if (LineArray[x].height == 0.0f || LineArray[x].width == 0.0f) {
            
        }
        else {
            for (int y = 0; y < CircleGameObjectArrayLength; y++)
            {
                CircleGameObject* c1 = &CircleGameObjectArray[y];
                if (CircleGameObjectArray[y].radius == 0.0f) // If radius of thing is 0
                {
                    break;
                }
                else {
                    if (CircleRectCol(c1, b1)) {

                    }
                }
            }
        }

    }
    
}


void game_exit(void)
{

}




