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
#include "Sound_Effect.h"
#include <gl/GL.h>

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
bool CheckAllButtons(void);
void TriggerButtonEffects(ButtonObject* x);
void Initialize_Screens(void);
void Initialize_Sprites(void);

//Initialize array
Screen screen_array[Total_screen_number];
Screen overlay_array[Total_overlay_number];

//Initialize Screen control
Screen_name Start_Screen = Splash_screen;
Screen_name Current_screen_name;
Screen_name Next_screen_name;

//struct screen being displayed and calculated
Screen Current_screen;
Screen* current_screen = &Current_screen;

//transition control
float transition_opacity = 0;

bool isScreenTransiting = false;
bool isgamepaused = true;

bool isoverlayTransiting = false;
bool isoverlayActive = false;

bool ButtonClicked = false;
bool startup;

bool restartingLevel = false;

//Sprites
CP_Image TestDoge = NULL;
CP_Image DigipenLogo = NULL;



void game_init(void)
{
    Initialize_Sprites();
    CP_Font_Set(CP_Font_GetDefault());

    if (debug) {
        CP_System_ShowConsole();
    }
    //initialize all screen data
    Initialize_Screens();
    //set the screens
    startup = true;
    Current_screen_name = Start_Screen;
    Next_screen_name = Start_Screen;
    //initialize current screen
    *current_screen = screen_array[Current_screen_name];
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255));
    MousePos = newVector2(CP_Input_GetMouseX(), CP_Input_GetMouseY());
    
    if (startup) {
        startupsequence(&Current_screen_name, &Next_screen_name, &isScreenTransiting, &startup , &isgamepaused);
    }
    
    if (!isScreenTransiting && !isoverlayTransiting && !restartingLevel) {//not transitioning to new screen or overlay or restarting
        if (current_screen->overlay_name != No_overlay) {
            if (CP_Input_KeyTriggered(KEY_ESCAPE))
            {
                isoverlayTransiting = true;//start overlay transition
            }
        }
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

    Drawoverlay(&isoverlayTransiting, &isoverlayActive, &isgamepaused, current_screen->overlay_name, overlay_array);
    if (restartingLevel) {
        Restart_transition(&restartingLevel, &isoverlayActive, &isgamepaused, &Current_screen_name, current_screen, screen_array);
    }
    else {
        Screen_transition(&isScreenTransiting, &isoverlayActive, &transition_opacity, &Current_screen_name, &Next_screen_name, current_screen, screen_array);
    }
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
            Current_screen.LineArray[LineCounter] = CreateBoxGameObject(LineStartPos, LineLength, 4.0f, DrawnLineBounciness,atan2f(LineEndPos.y - LineStartPos.y, LineEndPos.x - LineStartPos.x) / (float)PI * 180, TestDoge);
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
            if (x->image != NULL) {
                DrawBoxImage( x, 255);
            }
            else {
                CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);
            }
            

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
            if (x->boxGameObject.image != NULL) {
                DrawBoxImage(&x->boxGameObject, 255);
            }
            else {
                CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
                CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
                CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width);
            }
            
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
                            //PlayPitchedSoundEffect(BallBounce, 0.1f); //  Audio : ball bouncing off other balls 
                            
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
                        //PlayPitchedSoundEffect(BallBounce, 0.1f); //  Audio : ball bouncing off other balls 
                    }
                }
            }
        }

    }
    
}

bool CheckAllButtons(void) {
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
        if (isoverlayActive && !isoverlayTransiting) {
            for (int i = 0; i < ButtonObjectArrayLength; i++)
            {
                ButtonObject* x = &overlay_array[current_screen->overlay_name].ButtonObjectArray[i];
                //if mouse is coliding with button
                if (PointRectCol(MousePos, &x->boxGameObject)) {
                    TriggerButtonEffects(x);
                    return true;
                }
            }
        }
        else if(!isoverlayActive){
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
    }
    return false;
}

void TriggerButtonEffects(ButtonObject* x) {
    //check what is the buttons effect and run accordingly
    switch (x->button_effect)
    {
    case Move_to_options:
        isgamepaused = true;
        Next_screen_name = Options;
        isScreenTransiting = false;
        break;
    case Move_to_main_Menu:
        isgamepaused = false;
        Next_screen_name = Main_menu;
        isScreenTransiting = true;
        break;
    case Move_to_Level_Select:
        isgamepaused = false;
        Next_screen_name = Level_Select;
        isScreenTransiting = true;
        break;
    case Move_to_Level_1:
        isgamepaused = false;
        Next_screen_name = Level_1;
        isScreenTransiting = true;
        break;
    case Move_to_Level_2:
        isgamepaused = false;
        Next_screen_name = Level_2;
        isScreenTransiting = true;
        break;
    case Move_to_Level_3:
        isgamepaused = false;
        Next_screen_name = Level_3;
        isScreenTransiting = true;
        break;
    case Move_to_Level_4:
        isgamepaused = false;
        Next_screen_name = Level_4;
        isScreenTransiting = true;
        break;
    case Move_to_Level_5:
        isgamepaused = false;
        Next_screen_name = Level_5;
        isScreenTransiting = true;
        break;
    case Move_to_Level_6:
        isgamepaused = false;
        Next_screen_name = Level_6;
        isScreenTransiting = true;
        break;
    case Move_to_Level_7:
        isgamepaused = false;
        Next_screen_name = Level_7;
        isScreenTransiting = true;
        break;
    case Move_to_Level_8:
        isgamepaused = false;
        Next_screen_name = Level_8;
        isScreenTransiting = true;
        break;
    case Move_to_Level_9:
        isgamepaused = false;
        Next_screen_name = Level_9;
        isScreenTransiting = true;
        break;
    case Move_to_Level_10:
        isgamepaused = false;
        Next_screen_name = Level_10;
        isScreenTransiting = true;
        break;
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
    case Restart:
        //isScreenTransiting = true;
        restartingLevel = true;
        break;
    default:
        break;
    }
}

void Initialize_Screens(void) {
    //create overlay;
    overlay_array[pause_overlay].ButtonObjectArrayLengthCounter = 0;
    overlay_array[pause_overlay].ButtonObjectArray[1] = CreateButtonObject(newVector2(700, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_main_Menu, "Move to Main Menu");
    overlay_array[pause_overlay].ButtonObjectArray[2] = CreateButtonObject(newVector2(900, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Restart, "Restart");
    overlay_array[pause_overlay].ButtonObjectArray[3] = CreateButtonObject(newVector2(1100, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_Select, "Level Select");

    //create splash screen
    screen_array[Splash_screen].ButtonObjectArrayLengthCounter = 0;
    screen_array[Splash_screen].ButtonObjectArray[0] = CreateButtonObject(newVector2(0+CP_System_GetWindowWidth()/2.0f - 1026.0f /2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 249.0f /2.0f), 1026, 249, 50, 0, DigipenLogo, CP_Color_Create(255, 255, 255, 200), None, "");

    //Create Main Menu Screen
    screen_array[Main_menu].ButtonObjectArrayLengthCounter = 0;
    screen_array[Main_menu].ButtonObjectArray[0] = CreateButtonObject(newVector2(900, 500), 100, 100, 50, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_Select, "Level Select");

    screen_array[Level_Select].ButtonObjectArrayLengthCounter = 0;
    screen_array[Level_Select].ButtonObjectArray[0] = CreateButtonObject(newVector2(500 , 70), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_1, "Level 1");
    screen_array[Level_Select].ButtonObjectArray[1] = CreateButtonObject(newVector2(700, 70), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_2, "Level 2");
    screen_array[Level_Select].ButtonObjectArray[2] = CreateButtonObject(newVector2(900, 70), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_3, "Level 3");
    screen_array[Level_Select].ButtonObjectArray[3] = CreateButtonObject(newVector2(1100, 70), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_4, "Level 4");
    screen_array[Level_Select].ButtonObjectArray[4] = CreateButtonObject(newVector2(1300, 70), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_5, "Level 5");
    screen_array[Level_Select].ButtonObjectArray[5] = CreateButtonObject(newVector2(500, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_6, "Level 6");
    screen_array[Level_Select].ButtonObjectArray[6] = CreateButtonObject(newVector2(700, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_7, "Level 7");
    screen_array[Level_Select].ButtonObjectArray[7] = CreateButtonObject(newVector2(900, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_8, "Level 8");
    screen_array[Level_Select].ButtonObjectArray[8] = CreateButtonObject(newVector2(1100, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_9, "Level 9");
    screen_array[Level_Select].ButtonObjectArray[9] = CreateButtonObject(newVector2(1300, 300), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_Level_10, "Level 10");

    screen_array[Options].ButtonObjectArrayLengthCounter = 0;
    screen_array[Options].ButtonObjectArray[0] = CreateButtonObject(newVector2(900, 10), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_main_Menu, "Main Menu");

    //Create Test Room screen
    //Fill up array with test circles
    for (int i = 0; i < 200; i++) {
        float scale = CP_Random_RangeFloat(10, 15);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1800), CP_Random_RangeFloat(50, 1000)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale, 1);
        screen_array[Test_Room].CircleGameObjectArray[i] = tempc;
        screen_array[Level_1].CircleGameObjectArray[i] = tempc;
        screen_array[Level_2].CircleGameObjectArray[i] = tempc;
        screen_array[Level_3].CircleGameObjectArray[i] = tempc;
        screen_array[Level_4].CircleGameObjectArray[i] = tempc;
        screen_array[Level_5].CircleGameObjectArray[i] = tempc;
        screen_array[Level_6].CircleGameObjectArray[i] = tempc;
        screen_array[Level_7].CircleGameObjectArray[i] = tempc;
        screen_array[Level_8].CircleGameObjectArray[i] = tempc;
        screen_array[Level_9].CircleGameObjectArray[i] = tempc;
        screen_array[Level_10].CircleGameObjectArray[i] = tempc;
    }

    screen_array[Test_Room].LineArrayLengthCounter = 0;
    screen_array[Test_Room].CircleArrayLengthCounter = 0;
    screen_array[Test_Room].ButtonObjectArray[0] = CreateButtonObject(newVector2(10, 10), 100, 100, 0, 0, NULL, CP_Color_Create(255, 255, 255, 200), Move_to_main_Menu, "Move to Main Menu");

    screen_array[Level_1].LineArrayLengthCounter = 0;
    screen_array[Level_1].CircleArrayLengthCounter = 0;
    screen_array[Level_1].overlay_name = pause_overlay;

    screen_array[Level_2].LineArrayLengthCounter = 0;
    screen_array[Level_2].CircleArrayLengthCounter = 0;
    screen_array[Level_2].overlay_name = pause_overlay;

    screen_array[Level_3].LineArrayLengthCounter = 0;
    screen_array[Level_3].CircleArrayLengthCounter = 0;
    screen_array[Level_3].overlay_name = pause_overlay;

    screen_array[Level_4].LineArrayLengthCounter = 0;
    screen_array[Level_4].CircleArrayLengthCounter = 0;
    screen_array[Level_4].overlay_name = pause_overlay;

    screen_array[Level_5].LineArrayLengthCounter = 0;
    screen_array[Level_5].CircleArrayLengthCounter = 0;
    screen_array[Level_5].overlay_name = pause_overlay;

    screen_array[Level_6].LineArrayLengthCounter = 0;
    screen_array[Level_6].CircleArrayLengthCounter = 0;
    screen_array[Level_6].overlay_name = pause_overlay;

    screen_array[Level_7].LineArrayLengthCounter = 0;
    screen_array[Level_7].CircleArrayLengthCounter = 0;
    screen_array[Level_7].overlay_name = pause_overlay;

    screen_array[Level_8].LineArrayLengthCounter = 0;
    screen_array[Level_8].CircleArrayLengthCounter = 0;
    screen_array[Level_8].overlay_name = pause_overlay;
    
    screen_array[Level_9].LineArrayLengthCounter = 0;
    screen_array[Level_9].CircleArrayLengthCounter = 0;
    screen_array[Level_9].overlay_name = pause_overlay;

    screen_array[Level_10].LineArrayLengthCounter = 0;
    screen_array[Level_10].CircleArrayLengthCounter = 0;
    screen_array[Level_10].overlay_name = pause_overlay;
}
void Initialize_Sprites(void) {
    TestDoge = CP_Image_Load("./Sprites/MahLe.jpg");
    DigipenLogo = CP_Image_Load("./Assets/DigiPen_WHITE.png");
}
void game_exit(void)
{

}




