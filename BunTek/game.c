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
#include "Screen_control.h"
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

bool ButtonClicked = false;

//Initialize functions
void draw_framerate(void);
void DrawAllShapes(void);
void CalculateAllPhysics(void);
void AddLine(void);
bool CheckAllButtons(void);
void TriggerButtonEffects(ButtonObject* x);
void Initialize_Screens(void);

//Initialize array
Screen screen_array[Total_screen_number];

//Initialize Screen control
Screen_name Start_Screen = Test_Menu;
Screen_name Current_screen_name;
Screen_name Next_screen_name;

//struct screen being displayed and calculated
Screen Current_screen;
Screen* current_screen = &Current_screen;

//transition control
int transition_oppacity = 0;
bool isScreenTransiting = false;

bool isgamepaused = false;



void game_init(void)
{
    CP_Font_Set(CP_Font_GetDefault());
    if (debug) {
        CP_System_ShowConsole();
    }
    //initialize all screen data
    Initialize_Screens();
    //set the screens
    Current_screen_name = Start_Screen;
    Next_screen_name = Start_Screen;
    //initialize current screen
    *current_screen = screen_array[Current_screen_name];
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    MousePos = newVector2(CP_Input_GetMouseX(), CP_Input_GetMouseY());
    if (!isScreenTransiting) {//not transitioning to new screen
        //check for button Click and return bool, also run the buttons effect if any
        ButtonClicked = CheckAllButtons();
        if (!isgamepaused) {
            if (!ButtonClicked) {
                AddLine();
            }
            CalculateAllPhysics();
        }
    }
    DrawAllShapes();
    screen_transition(&isScreenTransiting, &transition_oppacity, &Current_screen_name, &Next_screen_name, current_screen, screen_array);
    printf("Mouse 1 held: %d\n", Mouse1Held);
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
            Current_screen.LineArray[LineCounter] = CreateBoxGameObject(LineStartPos, LineLength, 4.0f, DrawnLineBounciness,atan2f(LineEndPos.y - LineStartPos.y, LineEndPos.x - LineStartPos.x) / (float)PI * 180);
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
        CircleGameObject* x = &Current_screen.CircleGameObjectArray[i];

       
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
        BoxGameObject* x = &Current_screen.LineArray[i];

        if (x->width == 0 || x->height == 0)  // Box has no width or height
        {
            
        }
        else {
            CP_Settings_Fill(x->gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle,1);

        }
    }
    for (int i = 0; i < ButtonObjectArrayLength; i++)
    {
        ButtonObject* x = &Current_screen.ButtonObjectArray[i];

        if (x->boxGameObject.width == 0 || x->boxGameObject.height == 0)  // Box has no width or height
        {

        }
        else {
            CP_Settings_Fill(x->boxGameObject.gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
            CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
            CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
            CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width);
        }
    }
}

void CalculateAllPhysics(void)
{
    
    //Calculating circle physics
    for (int i = 0; i < CircleGameObjectArrayLength; i++)
    {
        if (Current_screen.CircleGameObjectArray[i].radius == 0.0f) // If radius of thing is 0
        {
            break;
        }
                
        CirclePhys(&Current_screen.CircleGameObjectArray[i]);
    }

    //Checking circle-circle collision
    for (int i = 0; i < CircleGameObjectArrayLength; i++)
    {
        CircleGameObject* c1 = &Current_screen.CircleGameObjectArray[i];
        if (Current_screen.CircleGameObjectArray[i].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
        {
            //End of Array
            break;
        }
        else 
        {
            for (int o = i+1; o < CircleGameObjectArrayLength; o++)
            {
                CircleGameObject* c2 = &Current_screen.CircleGameObjectArray[o];
                if (Current_screen.CircleGameObjectArray[o].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
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
                        if (CircleCol(c1, c2, true))  //If circle collides with cicle
                        {
                           
                            
                        }
                    }
                }
            }
        }
        
    }

    //Checking box-circle collision
    for (int x = 0; x < LineArrayLength; x++) {
        BoxGameObject* b1 = &Current_screen.LineArray[x];
        if (Current_screen.LineArray[x].height == 0.0f || Current_screen.LineArray[x].width == 0.0f) {
            
        }
        else {
            for (int y = 0; y < CircleGameObjectArrayLength; y++)
            {
                CircleGameObject* c1 = &Current_screen.CircleGameObjectArray[y];
                if (Current_screen.CircleGameObjectArray[y].radius == 0.0f) // If radius of thing is 0
                {
                    break;
                }
                else {
                    if (CircleRectCol(c1, b1, true)) {

                    }
                }
            }
        }

    }
    
}

bool CheckAllButtons(void) {
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {

        for (int i = 0; i < ButtonObjectArrayLength; i++)
        {
            ButtonObject* x = &Current_screen.ButtonObjectArray[i];
            //if mouse is coliding with button
            if (PointRectCol(MousePos, &x->boxGameObject)) {
                TriggerButtonEffects(x);
                return true;
            }
        }
    }
    return false;
}

void TriggerButtonEffects(ButtonObject* x) {
    //check what is the buttons effect and run accordingly
    switch (x->button_effect)
    {
    case Move_to_test_room:
        isgamepaused = false;
        Next_screen_name = Test_Room;
        isScreenTransiting = true;
        break;
    case Move_to_test_Menu:
        isgamepaused = false;
        Next_screen_name = Test_Menu;
        isScreenTransiting = true;
        break;
    case Pause_Game:
        isgamepaused = !isgamepaused;
        break;
    default:
        break;
    }
}

void Initialize_Screens(void) {
    //Create Test menu screen
    screen_array[Test_Menu].ButtonObjectArrayLengthCounter = 0;
    screen_array[Test_Menu].ButtonObjectArray[0] = CreateButtonObject(newVector2(10, 10), 100, 100, 0, 0, CP_Color_Create(255, 255, 255, 200), Move_to_test_room, "Move to Test Screen");

    //Create Test Room screen
    //Fill up array with test circles
    for (int i = 0; i < 200; i++) {
        float scale = CP_Random_RangeFloat(10, 15);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1800), CP_Random_RangeFloat(50, 1000)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale, 1);
        screen_array[Test_Room].CircleGameObjectArray[i] = tempc;
    }
    screen_array[Test_Room].LineArrayLengthCounter = 0;
    screen_array[Test_Room].CircleArrayLengthCounter = 0;
    screen_array[Test_Room].ButtonObjectArray[0] = CreateButtonObject(newVector2(10, 10), 100, 100, 0, 0, CP_Color_Create(255, 255, 255, 200), Move_to_test_Menu, "Move to Test Menu");
    screen_array[Test_Room].ButtonObjectArray[1] = CreateButtonObject(newVector2(120, 10), 100, 100, 0, 0, CP_Color_Create(255, 255, 255, 200), Pause_Game, "Pause Game");
}
void game_exit(void)
{

}




