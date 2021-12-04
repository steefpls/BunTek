/*---------------------------------------------------------
 * file:	game.c
 * author:	    []
 * email:	    []
 * Co-author:	[]
 * email:	    []
 * 
 * brief:	Main entry point for the sample project
            of the CProcessing library
 * documentation link:
 * https://github.com/DigiPen-Faculty/CProcessing/wiki
 * 
 * Copyright © 2020 DigiPen, All rights reserved.
* ---------------------------------------------------------*/

//#include "Screen_control.h"
#include "GameObjects.h"
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
const float DrawnLineBounciness = 1.0;
bool colided = false; 
bool colided_previous = false; 

//Initialize functions
void draw_framerate(void);
void DrawAllShapes(void);
void CalculateAllPhysics(void);
void AddLine(void);
bool CheckAllButtons(void);
void TriggerButtonEffects(ButtonObject* x);
void Initialize_Screens(void);
void Initialize_Sprites(void);
void UpdateAllSpawners(void);
void Initialize_Screen_Keys(void);
void TitlecardTransition(void);
void victorycontrol(void);
void bgm_control(Screen_name* current_sc_name);
void play_ballbounce_sfx();
void AddVolumeMeter(Vector2 position);
void InitializeVolumeControl(); 

//Initialize array
Screen screen_array[Total_screen_number];
Screen overlay_array[Total_overlay_number];

int key_count = 0;
Screen_keys Screen_key_array[Total_screen_number];

//Initialize Screen control
Screen_name Start_Screen = Splash_screen;
Screen_name Current_screen_name;
Screen_name Next_screen_name;
Overlay_name Current_overlay_name;
Screen_name* current_screen_name = &Current_screen_name; 

//struct screen being displayed and calculated
Screen Current_screen;
Screen* current_screen = &Current_screen;
//oveylay of current screen
Screen Current_overlay;
Screen* current_overlay = &Current_overlay;


//transition control
float transition_opacity = 0;

bool isScreenTransiting = false;
bool isgamepaused = true;

bool isoverlayTransiting = false;
bool isoverlayActive = false;

bool ButtonClicked = false;
bool startup;

bool restartingLevel = false;

bool titlecard = false;

bool victory = false;

// Volume Control 



//Sprites
CP_Image L1 = NULL;
CP_Image L2 = NULL;
CP_Image L3 = NULL;
CP_Image L4 = NULL;
CP_Image L5 = NULL;
CP_Image L6 = NULL;
CP_Image L7 = NULL;
CP_Image L8 = NULL;
CP_Image L9 = NULL;
CP_Image L10 = NULL;
CP_Image mainpage = NULL;
CP_Image pausepage = NULL;
CP_Image victorypage = NULL;
CP_Image DigipenLogo = NULL;
CP_Image GameLogo = NULL;
CP_Image HowToPlay = NULL;
CP_Image Credit1 = NULL;
CP_Image Credit2 = NULL;
CP_Image Credit3 = NULL;
CP_Image Spawner = NULL;
CP_Image addvolume = NULL; 
CP_Image decvolume = NULL; 
CP_Image volumepage = NULL;
CP_Image bgm = NULL;
CP_Image sfx = NULL;
ButtonbgInfo basebuttonbackground;
ButtonbgInfo nobuttonbackground;
ButtonbgInfo incvolbuttonbackground; 
ButtonbgInfo decvolbuttonbackground;

void game_init(void)
{
     
    Initialize_Sprites();
    CP_Font_Set(CP_Font_GetDefault());

    if (debug) {
        CP_System_ShowConsole();
    }
    //initialize all screen data
    Initialize_Screen_Keys();
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
    victorycontrol();
    if (!isScreenTransiting && !isoverlayTransiting && !restartingLevel) {//not transitioning to new screen or overlay or restarting
        if (titlecard) {
            TitlecardTransition();
        }
        if ((current_screen->overlay_name != No_overlay) && !victory) {
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
            UpdateAllSpawners();

        }
    }
    DrawAllShapes();

    Drawoverlay(&isoverlayTransiting, &isoverlayActive, &isgamepaused, Current_overlay_name, current_overlay);
    if (restartingLevel) {
        Restart_transition(&restartingLevel, &isoverlayActive, &isgamepaused, &Current_screen_name, current_screen, screen_array, &Current_overlay_name, current_overlay, overlay_array);
    }
    else {
        Screen_transition(&isScreenTransiting, &isoverlayActive, &transition_opacity, &Current_screen_name, &Next_screen_name, current_screen, screen_array, &Current_overlay_name, current_overlay, overlay_array);
    }

    // Testing 
    if (CP_Input_KeyTriggered(KEY_A)) {
        play_ballbounce_sfx(); 
    }
    



    // Profiling info and frameRate testing
    if (debug) draw_framerate();

    // Sound Control
    bgm_control(current_screen_name);
}

void AddLine(void) {
    
    if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {

        colided = false;  // reset boolean. 
        BoxGameObject* x;
        for (int i = 0; i < Current_screen.NoDrawZonesArrayLengthCounter; i++) {
            x = &Current_screen.NoDrawZoneArray[i];

            if (PointRectCol(MousePos, x) == true) { // if draw collides box , colided == true 
                colided = true;
            }

        }
        
        if (!colided) {
            if (!Mouse1Held || colided_previous) // New Line needs to be made // if mouse is not held. 
            {
                LineTimer = 0;
                LineStartPos = MousePos;
                LineEndPos = MousePos;
                LineLength = 0;
            }
            else //continue from previous line // if mouse is held.
            {
                LineTimer += FrameTime;
                if ((LineTimer >= NewLineInterval && LineLength > 10) || LineLength > 100)   // LineLength = end point - start point
                {
                    LineStartPos = LineEndPos;
                    if (Current_screen.LineArrayLengthCounter + 1 < LineArrayLength)
                    {
                        Current_screen.LineArrayLengthCounter += 1;
                    }
                    LineTimer = 0;
                }

                // Creates new line for every frame. 
                LineEndPos = MousePos;

                LineLength = DistBetween(LineStartPos, LineEndPos);
            }

            // if colided == false. 
            if ((MousePos.x != MousePosPrev.x || (MousePos.y != MousePosPrev.y) && (colided == false))) {
                BoxGameObject templine = CreateBoxGameObject(LineStartPos, LineLength, 4.0f, DrawnLineBounciness, atan2f(LineEndPos.y - LineStartPos.y, LineEndPos.x - LineStartPos.x) / (float)PI * 180, NULL, CP_Color_Create(255, 255, 255, 255));
                for (int i = 0; i < Current_screen.NoDrawZonesArrayLengthCounter; i++) {
                    x = &Current_screen.NoDrawZoneArray[i];
                    if (RectRectCol(&templine, x)) {
                        colided = true;
                        break;
                    }
                }
                if (!colided) {
                    Current_screen.LineArray[Current_screen.LineArrayLengthCounter] = templine;
                }


                // TODO: Delete lines if colide. (move to fast into the box - box detection didnt detect , so to solve this we delete the line. 

            }
        }
    }
    Mouse1Held = CP_Input_MouseDown(MOUSE_BUTTON_1);
    MousePosPrev = MousePos;
    colided_previous = colided; 
}


void DrawAllShapes(void)
{
    // Draw Obstacles -> Draw no-draw zone
    for (int i = 0; i < Current_screen.NoDrawZonesArrayLengthCounter; i++) {

        BoxGameObject* x = &Current_screen.NoDrawZoneArray[i];

        if (x->width == 0 || x->height == 0) {

        }
        else {
            CP_Settings_Fill(x->gameObject.color);

            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);

        }
    }

    for (int i = 0; i < Current_screen.CircleportalpairArrayLengthCounter; i++) {
        Circleportalpair* cpp = &Current_screen.CircleportalpairArray[i];

        CP_Settings_Fill(cpp->portal_1.gameObject.color);

        if (cpp->portal_1.outline) {
            CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

        }
        else {
            CP_Settings_NoStroke();
        }
        if (cpp->image != NULL) {
            CP_Image_DrawAdvanced(cpp->image, cpp->portal_1.gameObject.position.x, cpp->portal_1.gameObject.position.y, cpp->portal_1.radius * 2, cpp->portal_1.radius * 2, 255, cpp->portal_1.gameObject.angle);
            CP_Image_DrawAdvanced(cpp->image, cpp->portal_2.gameObject.position.x, cpp->portal_2.gameObject.position.y, cpp->portal_2.radius * 2, cpp->portal_2.radius * 2, 255, cpp->portal_2.gameObject.angle);
        }
        else {
            CP_Graphics_DrawCircle(cpp->portal_1.gameObject.position.x, cpp->portal_1.gameObject.position.y, cpp->portal_1.radius * 2);
            CP_Graphics_DrawCircle(cpp->portal_2.gameObject.position.x, cpp->portal_2.gameObject.position.y, cpp->portal_2.radius * 2);
        }


    }

    for (int i = 0; i < Current_screen.CircleArrayLengthCounter; i++)
    {
        CircleGameObject* x = &Current_screen.CircleGameObjectArray[i];
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
    for (int i = 0; i < Current_screen.LineArrayLengthCounter; i++)
    {
        BoxGameObject* x = &Current_screen.LineArray[i];

        CP_Settings_Fill(x->gameObject.color);
        CP_Settings_NoStroke();
        if (x->image != NULL) {
            DrawBoxImage(x, 255);
        }
        else {
            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);

            if (x->width == 0 || x->height == 0)  // Box has no width or height
            {

            }
            else {
                CP_Settings_Fill(x->gameObject.color);
                CP_Settings_NoStroke();
                if (x->image != NULL) {
                    DrawBoxImage(x, 255);
                }
                else {
                    CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);
                }

            }
        }
    }

    for (int i = 0; i < Current_screen.ButtonObjectArrayLengthCounter; i++)
    {
        ButtonObject* x = &Current_screen.ButtonObjectArray[i];

        if (x->boxGameObject.width == 0 || x->boxGameObject.height == 0)  // Box has no width or height
        {

        }
        else {
            CP_Settings_Fill(x->boxGameObject.gameObject.color);
            CP_Settings_NoStroke();
            //draw button background
            DrawButtonBackgroundImage(&x->boxGameObject, &x->buttonbgInfo, &isoverlayTransiting);
            if (x->boxGameObject.image != NULL) {
                DrawBoxImage(&x->boxGameObject, 255);
            }
            else {
                CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
                CP_Settings_Fill(x->TextColor);
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
                CP_Settings_TextSize(20);
                CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y + x->boxGameObject.height/3, x->boxGameObject.width);
            }

        }
    }
    // Draw Obstacles -> Draw Super Bounce Platform
    for (int i = 0; i < BoxGameObjectArrayLength; i++)
    {
        BoxGameObject* x = &Current_screen.SuperBouncePlatformArray[i];

        // Volume Options flow
        if (Current_screen_name == Options) {
            if (x->width == 0 || x->height == 0) {

            }
            else {
                CP_Settings_Fill(x->gameObject.color);
                CP_Settings_NoStroke();
                CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);
            }
        }
        // Invalid Object - if there is no width & height (the object drawn won't be seen)
        if (x->width == 0 || x->height == 0) {

        }
        else {
            CP_Settings_Fill(x->gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);
        }

    }

    // Draw Obstacles -> Draw Testing Box Objects
    for (int i = 0; i < BoxGameObjectArrayLength; i++)
    {
        BoxGameObject* x = &Current_screen.BoxGameObjectArray[i];
       
        // Invalid Object - if there is no width & height (the object drawn won't be seen)
        if (x->width == 0 || x->height == 0) {

        }
        else {
            CP_Settings_Fill(x->gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);
        }

    }

    //Draw BallSpawner to spawn ball objects
    for (int i = 0; i < SpawnerGameObjectArrayLength; i++)
    {
        BallSpawner* x = &Current_screen.BallSpawnerArray[i];
        if (x->b.image != NULL)
        {
            DrawBoxImage(&x->b, 255);
        }
        else
        {
        }
    }

    // Draw Scoring Container
    for (int i = 0; i < Current_screen.ScoringContainerArrayLengthCounter; i++) {

        ScoringContainerObject * x = &Current_screen.ScoringContainerArray[i]; 

        
        if (x->box.width == 0 || x->box.height == 0) {

        } 
        else {
            // Draw Code : while container has not meet it's ball count. 
            if (x->ballcountgoal != 0) {           
                CP_Graphics_ClearBackground(CP_Color_Create(50, 50, 50, 255)); 
                // Draw Container 
                CP_Settings_Fill(x->box.gameObject.color);
                CP_Graphics_DrawRectAdvanced(x->box.gameObject.position.x, x->box.gameObject.position.y, x->box.width, x->box.height, x->box.gameObject.angle, 1);
                CP_Settings_Fill(x->textcolor);
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
                CP_Settings_TextSize(30);

                char buffer_count_string[100];
                char* buffer_ptr = buffer_count_string;

                snprintf(buffer_ptr, 100, "%d", x->ballcountgoal);

                CP_Font_DrawTextBox(buffer_ptr, x->box.gameObject.position.x, x->box.gameObject.position.y + x->box.height / 2, x->box.width);
            } else {

                // Draw Code : while container has met it's ball count.
                // Draw Container 
                CP_Settings_Fill(CP_Color_Create(50, 205, 50, 255));  // lime green
                CP_Graphics_DrawRectAdvanced(x->box.gameObject.position.x, x->box.gameObject.position.y, x->box.width, x->box.height, x->box.gameObject.angle, 1);
                CP_Settings_Fill(x->textcolor);
                CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
                CP_Settings_TextSize(30);

                char buffer_count_string[100];
                char* buffer_ptr = buffer_count_string;

                snprintf(buffer_ptr, 100, "%d", x->ballcountgoal);

                CP_Font_DrawTextBox(buffer_ptr, x->box.gameObject.position.x, x->box.gameObject.position.y + x->box.height / 2, x->box.width);
            }

        }
    }
    
    // Volume Boxes
    for (int i = 0; i < VolumeObjectArrayLength; i++)
    {
        VolumeObject* x = &Current_screen.VolumeObjectArray[i];
        if (x->boxGameObject.width == 0 || x->boxGameObject.height == 0) {

        }
        else
        {
            if (x->lit == true) {
                CP_Settings_Fill(COLOR_PASTEL_GREEN);
                CP_Settings_Stroke(COLOR_WHITE);
                CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
            }
            else 
            {
                CP_Settings_Fill(COLOR_GREY);
                CP_Settings_Stroke(COLOR_WHITE);
                CP_Graphics_DrawRectAdvanced(x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y, x->boxGameObject.width, x->boxGameObject.height, x->boxGameObject.gameObject.angle, 1);
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
    for (int i = 0; i < Current_screen.CircleArrayLengthCounter; i++)
    {
        CircleGameObject* c1 = &Current_screen.CircleGameObjectArray[i];
        
        if(c1->teleportinfo.teleportStatus == Not_teleporting)
        {
            for (int o = i+1; o < Current_screen.CircleArrayLengthCounter; o++)
            {
                CircleGameObject* c2 = &Current_screen.CircleGameObjectArray[o];
                
                 if(c2->teleportinfo.teleportStatus == Not_teleporting)
                 {
                    if (c1 == c2) 
                    {
                        continue;
                    }
                    else 
                    {
                        if (CircleCol(c1, c2, true))  //If circle collides with cicle
                        {
                            play_ballbounce_sfx(); 
                        }
                    }
                 }
            }
            RemoveOOB(&Current_screen, *c1);
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
                        play_ballbounce_sfx(); 
                    }
                }
            }
        }

    }

    //Checking box-circle collision
    for (int x = 0; x < BoxGameObjectArrayLength; x++) {
        BoxGameObject* b1 = &Current_screen.BoxGameObjectArray[x];
        if (Current_screen.BoxGameObjectArray[x].height == 0.0f || Current_screen.BoxGameObjectArray[x].width == 0.0f) {

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
                        play_ballbounce_sfx(); 
                    }
                }
            }
        }

    }

    // Calculate physics between super-bounce and balls. 
    for (int x = 0; x < BoxGameObjectArrayLength; x++) {
        BoxGameObject* b1 = &Current_screen.SuperBouncePlatformArray[x];
        if (Current_screen.SuperBouncePlatformArray[x].height == 0.0f || Current_screen.SuperBouncePlatformArray[x].width == 0.0f) {

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
                        play_ballbounce_sfx(); 
                    }
                }
            }
        }
        
    }

    // Calculate ball collision into scoringContainer
    for (int x = 0; x < BoxGameObjectArrayLength; x++) {
        ScoringContainerObject* b1 = &Current_screen.ScoringContainerArray[x]; 
        if (Current_screen.ScoringContainerArray[x].box.height == 0.0f || Current_screen.ScoringContainerArray[x].box.width == 0.0f) {
            // do nothing. (impossible to calculate collision)
        }
        else {
            for (int y = 0; y < CircleGameObjectArrayLength; y++) 
            {
                CircleGameObject* c1 = &Current_screen.CircleGameObjectArray[y]; 
                if (Current_screen.CircleGameObjectArray[y].radius == 0.0f) // if radius of ball is 0, can't calculate collision
                {
                    break;
                }
                else {
                    if (CircleRectCol(c1, &b1->box, true) && !b1->filled) // if collision into the container true. 
                    {   
                        // Destroy the ball 
                        RemoveBall(&Current_screen, *c1);               
                        b1->ballcountgoal -= 1;     // decrease counter by 1. this value will be updated on the container
                        if (b1->ballcountgoal <= 0) {
                            b1->filled = true; 
                        } 
                    }
                }
            }
        }

    }
    
    //Calculate ball collision into circle portal pair
    for (int x = 0; x < Current_screen.CircleportalpairArrayLengthCounter; x++) {
        Circleportalpair* cpp = &Current_screen.CircleportalpairArray[x];
        if (cpp->portal_1.radius == 0 || cpp->portal_2.radius == 0) {
            break;
        }
        else {

            for (int o = 0; o < Current_screen.CircleArrayLengthCounter; o++) {
                CircleGameObject* c = &Current_screen.CircleGameObjectArray[o];
                if (c->radius == 0.0f) {
                    break;
                }
                else {
                    //teleportation logic
                    Vector2 resultant;
                    bool collided_portal_1 = CircleCol(c, &cpp->portal_1, false);
                    bool collided_portal_2 = CircleCol(c, &cpp->portal_2, false);
                    //colide with protal 1
                    if (collided_portal_1) {
                        resultant = VectorMinus(c->gameObject.position, cpp->portal_1.gameObject.position);
                        //Moving into portal
                        if (c->teleportinfo.teleportStatus == Not_teleporting) {
                            c->teleportinfo.original_resultant = resultant;
                            c->teleportinfo.original_velocity = c->gameObject.velocity;
                            c->teleportinfo.original_radius = c->radius;
                            c->teleportinfo.teleportStatus = teleporting;
                        }
                        //in entrance portal
                        else if (c->teleportinfo.teleportStatus == teleporting) {
                            c->gameObject.velocity = newVector2(0, 0);
                            resultant = VectorDivide(resultant, CP_Math_LerpFloat(1, PortalBallDistDivisor ,FrameTime));
                            resultant = RotateVector(resultant, PortalRotation * FrameTime);
                            c->gameObject.position = VectorAdd(cpp->portal_1.gameObject.position, resultant);
                            if (c->radius > 0) {
                                c->radius = c->radius / CP_Math_LerpFloat(1, PortalBallRadiusDivisor, FrameTime);
                                if (c->radius < 1) {
                                   c->gameObject.position = cpp->portal_2.gameObject.position;
                                   c->gameObject.velocity = VectorMultiply(Normalize(c->teleportinfo.original_velocity), cpp->Exit_vel_scale);
                                   //c->gameObject.velocity = c->teleportinfo.original_velocity;
                                   c->teleportinfo.teleportStatus = teleported;
                                }
                            }
                            
                        }
                        //At exit portal
                        else if (c->teleportinfo.teleportStatus == teleported) {
                            c->radius = c->teleportinfo.original_radius * (VectorMagnitude(resultant) / cpp->portal_1.radius);
                            //exited portal boundary
                            if (c->radius >= c->teleportinfo.original_radius) {
                                c->radius = c->teleportinfo.original_radius;
                                c->teleportinfo.teleportStatus = Not_teleporting;
                            }
                        }       

                    }
                    //colide with protal 2
                    else if (collided_portal_2) {
                        resultant = VectorMinus(c->gameObject.position, cpp->portal_2.gameObject.position);
                        //Moving into portal
                        if (c->teleportinfo.teleportStatus == Not_teleporting) {
                            c->teleportinfo.original_resultant = resultant;
                            c->teleportinfo.original_velocity = c->gameObject.velocity;
                            c->teleportinfo.original_radius = c->radius;
                            c->teleportinfo.teleportStatus = teleporting;
                        }
                        //in entrance portal
                        else if (c->teleportinfo.teleportStatus == teleporting) {
                            c->gameObject.velocity = newVector2(0, 0);
                            resultant = VectorDivide(resultant, CP_Math_LerpFloat(1, PortalBallDistDivisor, FrameTime));
                            resultant = RotateVector(resultant, PortalRotation * FrameTime);
                            c->gameObject.position = VectorAdd(cpp->portal_2.gameObject.position, resultant);
                            if (c->radius > 0) {
                                c->radius = c->radius / CP_Math_LerpFloat(1, PortalBallRadiusDivisor, FrameTime);
                                if (c->radius < 1) {
                                    c->gameObject.position = cpp->portal_1.gameObject.position;
                                    c->gameObject.velocity = VectorMultiply(Normalize(c->teleportinfo.original_velocity), cpp->Exit_vel_scale);
                                    //c->gameObject.velocity = c->teleportinfo.original_velocity;
                                    c->teleportinfo.teleportStatus = teleported;
                                }
                            }
                        }
                        //At exit portal
                        else if (c->teleportinfo.teleportStatus == teleported) {
                            c->radius = c->teleportinfo.original_radius * (VectorMagnitude(resultant) / cpp->portal_2.radius);
                            //exited portal boundary
                            if (c->radius >= c->teleportinfo.original_radius) {
                                c->radius = c->teleportinfo.original_radius;
                                c->teleportinfo.teleportStatus = Not_teleporting;
                            }
                        }
                    }
                    
                }
            }
                
        }

    }
}

bool CheckAllButtons(void){
    //lerp buttons
    ButtonLerp(MousePos, &isoverlayActive, current_screen, current_overlay);
    //check overlay
    if (isoverlayActive && !isoverlayTransiting) {
        for (int i = 0; i < Current_overlay.ButtonObjectArrayLengthCounter; i++)
        {
            ButtonObject* x = &current_overlay->ButtonObjectArray[i];
            //if mouse is coliding with button
            if (PointRectCol(MousePos, &x->boxGameObject)) {
                if (CP_Input_MouseReleased(MOUSE_BUTTON_LEFT)) {
                    TriggerButtonEffects(x);
                }
                return true;
            }
        }
    }
    //check screen
    else if (!isoverlayActive) {
        for (int i = 0; i < Current_screen.ButtonObjectArrayLengthCounter; i++)
        {
            ButtonObject* x = &Current_screen.ButtonObjectArray[i];
            //if mouse is coliding with button
            if (PointRectCol(MousePos, &x->boxGameObject)) {
                if (CP_Input_MouseReleased(MOUSE_BUTTON_LEFT)) {
                    TriggerButtonEffects(x);
                }
                return true;
            }
        }
    }
    return false;
}

void TriggerButtonEffects(ButtonObject* x) {
    //check what is the buttons effect and run accordingly

    /*int volume_BGM_active = 0;*/
   /* int volume_SFX_active = 0;*/

    bool vol_added; 
    bool vol_decreased;
    bool once = false;

    int index_to_delete_from_right = 0;
    int bars_lit_counter = 0;

    switch (x->button_effect)
    {
    
    case Pause_Game:
        isgamepaused = !isgamepaused;
        break;
    case Exit_game :
        CP_Engine_Terminate();
        break;
    case Restart:
        //isScreenTransiting = true;
        restartingLevel = true;
        break;
    case Increase_Volume_BGM:

        // Visual Feedback (add bar)
        vol_added = false;
        bars_lit_counter = 0;       

        for (int i = 0; i < 5; i++)
        {
            VolumeObject* v = &Current_screen.VolumeObjectArray[i];

            if (v->lit == false)
            {
                if (vol_added == false && i < 5) {
                    v->lit = true; 
                    vol_added = true; 
                }
            }

        }

        // Effects on sound (add volume by 1 fold) - if not full
        //                  (do not do anything)   - if full

        // Check how many bars are lit. 
        for (int check = 0; check < 5; check++)
        {
            //printf("%d\n", check);
            VolumeObject* v = &Current_screen.VolumeObjectArray[check];

            if (v->lit == true) // based on the first cell?
            {
                bars_lit_counter++;
                //printf("bar lit counter increased. final value %d!\n", bars_lit_counter);
            }
        }

        if (bars_lit_counter == 0) {
            break; 
        }

        SetMusicVolume(0.2f * (float) (bars_lit_counter - 1)); // re-adjust based on the number of bars lit
        break; 
    case Decrease_Volume_BGM :

        vol_decreased = false;
        index_to_delete_from_right = 0;
        bars_lit_counter = 0;

        // Algorithm to check how many bars lit up
        for (int check = 0; check < 5; check++) 
        {
            //printf("%d\n", check);
            VolumeObject* v = &Current_screen.VolumeObjectArray[check];

            if (v->lit == true) // based on the first cell?
            {
                bars_lit_counter++; 
                //printf("bar lit counter increased. final value %d!\n", bars_lit_counter);
            }
        }
        
        if (once == false) {
            //printf("Number of Lit Bars: %d\n", bars_lit_counter);
            once = true; 
        }

        index_to_delete_from_right = bars_lit_counter - 1;  // bars_lit_counter = how many elements. (must account for which index to delete so -1)

        for (int i = 0; i < 5; i++)
        {
            VolumeObject* v = &Current_screen.VolumeObjectArray[i];

            //printf("%d\n", i);

            if (v->lit == true)
            {
                (v + index_to_delete_from_right)->lit = false;
                printf("Deleted Element %d\n", i + index_to_delete_from_right);
                index_to_delete_from_right--;
            }

        }
        
        if (bars_lit_counter != 0) {
            SetMusicVolume(0.2f * (float)(bars_lit_counter - 1)); // re-adjust based on the number of bars lit
        }
        else {
            SetMusicVolume(0.0f);
        }
        
        printf("Music set to level %d\n", bars_lit_counter - 1); // for debugging
        break; 
    case Increase_Volume_SFX :

        // Check how many bars are lit. 
        for (int check = 0; check < 5; check++)
        {
            //printf("%d\n", check);
            VolumeObject* v = &Current_screen.VolumeObjectArray[check];

            if (v->lit == true) // based on the first cell?
            {
                bars_lit_counter++;
                //printf("bar lit counter increased. final value %d!\n", bars_lit_counter);
            }
        }

        SetSFXVolume(0.2f * (float)(bars_lit_counter - 1)); // re-adjust based on the number of bars lit
        play_ballbounce_sfx(); // sample for users
        printf("Music set to level %d\n", bars_lit_counter - 1); // for debugging

        vol_added = false;

        for (int j = 5; j < 10; j++)
        {
            VolumeObject* v = &Current_screen.VolumeObjectArray[j];

            if (v->lit == false)
            {
                if (vol_added == false && (j >= 5 && j <= 9)) {
                    v->lit = true;
                    vol_added = true;
                }
            }

        }
        break;

    case Decrease_Volume_SFX :

        vol_decreased = false;
        bars_lit_counter = 0;
        index_to_delete_from_right = 0;

        // Algorithm to check how many bars lit up
        for (int check = 5; check < 10; check++)
        {
            //printf("%d\n", check);
            VolumeObject* v = &Current_screen.VolumeObjectArray[check];

            if (v->lit == true) // based on the first cell?
            {
                bars_lit_counter++;
                printf("bar lit counter increased. final value %d!\n", bars_lit_counter);
            }
        }

        if (once == false) {
            //printf("Number of Lit Bars: %d\n", bars_lit_counter);
            once = true;
        }

        index_to_delete_from_right = bars_lit_counter - 1;  // bars_lit_counter = how many elements. (must account for which index to delete so -1)

        for (int j = 5; j < 10; j++)
        {
            VolumeObject* v = &Current_screen.VolumeObjectArray[j];

            if (v->lit == true)
            {
                if (vol_decreased == false) {
                    (v + index_to_delete_from_right)->lit = false;
                    vol_decreased = true;
                }
            }

        }

        if (bars_lit_counter != 0) {
            SetSFXVolume(0.2f * (float)(bars_lit_counter - 1)); // re-adjust based on the number of bars lit
        }
        play_ballbounce_sfx(); // sample for users
        printf("Music set to level %d\n", bars_lit_counter - 1); // for debugging

        break; 
        
       
    default:
        for (int i = 0; i < key_count; i++) {
            if (Screen_key_array[i].key == x->button_effect) {
                if ((x->button_effect == Next_Level) && (Screen_key_array[i].value == Current_screen_name)) {
                    isgamepaused = false;
                    Next_screen_name = Screen_key_array[i].subvalue;
                    isScreenTransiting = true;
                    titlecard = true;
                    break;
                }
                else if (x->button_effect != Next_Level) {
                    isgamepaused = false;
                    Next_screen_name = Screen_key_array[i].value;
                    isScreenTransiting = true;
                    if (Screen_key_array[i].subvalue != No_screen) {
                        titlecard = true;
                        break;
                    }
                    titlecard = false;
                    break;
                }
            }
        }
        break;
    }
}

void TitlecardTransition(void) {
    if (titlecard) {
        if (Stopwatch(2.0f)) {
            for (int i = 0; i < key_count; i++) {
                if (Screen_key_array[i].value == Current_screen_name) {
                    isgamepaused = false;
                    Next_screen_name = Screen_key_array[i].subvalue;
                    isScreenTransiting = true;
                    titlecard = false;
                    break;
                }
            }

        }
    }
}

bool VictoryCheck(void) {
    if (Current_screen.ScoringContainerArrayLengthCounter != 0) {
        for (int x = 0; x < Current_screen.ScoringContainerArrayLengthCounter; x++) {
            if (!Current_screen.ScoringContainerArray[x].filled) {
                return false;
            }
        }
        return true;
    }
    return false;
}
void victorycontrol(void) {
    if (Current_overlay_name != victory_overlay) {
        victory = false;
    }
    if (VictoryCheck() && !victory) {
        victory = true;
        isoverlayTransiting = true;
        Current_overlay_name = victory_overlay;
        Current_overlay = overlay_array[victory_overlay];
    }
}


void Initialize_Screen_Keys(void) {
    AddScreenKey(&key_count, Move_to_Level_1, Level_1_title, Level_1, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_2, Level_2_title, Level_2, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_3, Level_3_title, Level_3, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_4, Level_4_title, Level_4, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_5, Level_5_title, Level_5, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_6, Level_6_title, Level_6, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_7, Level_7_title, Level_7, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_8, Level_8_title, Level_8, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_9, Level_9_title, Level_9, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_10, Level_10_title, Level_10, Screen_key_array);
    AddScreenKey(&key_count, Move_to_main_Menu, Main_menu, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_options, Options, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_credits, Credits_screen, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_credits2, Credits_screen2, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_credits3, Credits_screen3, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_tutorial, Tutorial, No_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_Select, Level_Select, No_screen, Screen_key_array);

    AddScreenKey(&key_count, Next_Level, Level_1, Level_2_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_2, Level_3_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_3, Level_4_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_4, Level_5_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_5, Level_6_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_6, Level_7_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_7, Level_8_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_8, Level_9_title, Screen_key_array);
    AddScreenKey(&key_count, Next_Level, Level_9, Level_10_title, Screen_key_array);

}

void Initialize_Screens(void) {
    //create pause overlay;
    overlay_array[pause_overlay].ButtonObjectArrayLengthCounter = 0;
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(740, 100), 480, 270, 0, 0, pausepage, CP_Color_Create(0, 0, 0, 0), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(700, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(900, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Restart, CP_Color_Create(255, 255, 255, 255), "Restart Game", basebuttonbackground));
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(1100, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Level Select", basebuttonbackground));

    //create victory overlay;
    overlay_array[victory_overlay].ButtonObjectArrayLengthCounter = 0;
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(740, 100), 480, 270, 0, 0, victorypage, CP_Color_Create(0, 0, 0, 0), None, CP_Color_Create(255, 255, 255, 255), "Victory", nobuttonbackground));
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(700, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Restart, CP_Color_Create(255, 255, 255, 255), "Restart Game", basebuttonbackground));
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(900, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Next_Level, CP_Color_Create(255, 255, 255, 255), "Next Level", basebuttonbackground));
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(1100, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Level Select", basebuttonbackground));


    //titles
    AddButton(&screen_array[Level_1_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L1, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "1", nobuttonbackground));
    AddButton(&screen_array[Level_2_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L2, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "2", nobuttonbackground));
    AddButton(&screen_array[Level_3_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L3, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "3", nobuttonbackground));
    AddButton(&screen_array[Level_4_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L4, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "4", nobuttonbackground));
    AddButton(&screen_array[Level_5_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L5, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "5", nobuttonbackground));
    AddButton(&screen_array[Level_6_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L6, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "6", nobuttonbackground));
    AddButton(&screen_array[Level_7_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L7, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "7", nobuttonbackground));
    AddButton(&screen_array[Level_8_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L8, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "8", nobuttonbackground));
    AddButton(&screen_array[Level_9_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L9, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "9", nobuttonbackground));
    AddButton(&screen_array[Level_10_title], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, L10, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "10", nobuttonbackground));


    //create splash screen
    AddButton(&screen_array[Splash_screen], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, DigipenLogo, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));

    //create splash logo screen
    AddButton(&screen_array[Splash_Logo], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1920.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 1080.0f / 2.0f), 1920, 1080, 50, 0, GameLogo, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));


    //Create Main Menu Screen
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(740, 50), 480, 270, 0, 0, mainpage, CP_Color_Create(0, 0, 0, 0), None , CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 350), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Start Game", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_tutorial, CP_Color_Create(255, 255, 255, 255), "How to play", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 650), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits, CP_Color_Create(255, 255, 255, 255), "Credits", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_options, CP_Color_Create(255, 255, 255, 255), "Options", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 950), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Exit_game, CP_Color_Create(255, 255, 255, 255), "Quit game", basebuttonbackground));

    //Create Tutorial screen
    AddButton(&screen_array[Tutorial], CreateButtonObject(newVector2(900, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    AddButton(&screen_array[Tutorial], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1600.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 900.0f / 2.0f), 1600, 900, 50, 0, HowToPlay, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));

  
    //Create Credits 1
    AddButton(&screen_array[Credits_screen], CreateButtonObject(newVector2(900, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    AddButton(&screen_array[Credits_screen], CreateButtonObject(newVector2(1100, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits2, CP_Color_Create(255, 255, 255, 255), "Next", basebuttonbackground));
    AddButton(&screen_array[Credits_screen], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1600.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 950.0f / 2.0f), 1600, 900, 50, 0, Credit1, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));

    AddButton(&screen_array[Credits_screen2], CreateButtonObject(newVector2(900, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    AddButton(&screen_array[Credits_screen2], CreateButtonObject(newVector2(1100, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits3, CP_Color_Create(255, 255, 255, 255), "Next", basebuttonbackground));
    AddButton(&screen_array[Credits_screen2], CreateButtonObject(newVector2(700, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits, CP_Color_Create(255, 255, 255, 255), "Back", basebuttonbackground));
    AddButton(&screen_array[Credits_screen2], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1600.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 950.0f / 2.0f), 1600, 900, 50, 0, Credit2, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));

    AddButton(&screen_array[Credits_screen3], CreateButtonObject(newVector2(900, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    AddButton(&screen_array[Credits_screen3], CreateButtonObject(newVector2(700, 980), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits2, CP_Color_Create(255, 255, 255, 255), "Back", basebuttonbackground));
    AddButton(&screen_array[Credits_screen3], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1600.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 950.0f / 2.0f), 1600, 900, 50, 0, Credit3, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));

    //level select
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(500, 70), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_1, CP_Color_Create(255, 255, 255, 255), "Level 1", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(700, 70), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_2, CP_Color_Create(255, 255, 255, 255), "Level 2", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(900, 70), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_3, CP_Color_Create(255, 255, 255, 255), "Level 3", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(1100, 70), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_4, CP_Color_Create(255, 255, 255, 255), "Level 4", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(1300, 70), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_5, CP_Color_Create(255, 255, 255, 255), "Level 5", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(500, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_6, CP_Color_Create(255, 255, 255, 255), "Level 6", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(700, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_7, CP_Color_Create(255, 255, 255, 255), "Level 7", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(900, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_8, CP_Color_Create(255, 255, 255, 255), "Level 8", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(1100, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_9, CP_Color_Create(255, 255, 255, 255), "Level 9", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(1300, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_10, CP_Color_Create(255, 255, 255, 255), "Level 10", basebuttonbackground));
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));

    // Volume Control
    screen_array[Options].ButtonObjectArrayLengthCounter = 0;
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(900, 150), 200, 80, 0, 0, volumepage, CP_Color_Create(0, 0, 0, 0), None, CP_Color_Create(255, 255, 255, 255), " ", nobuttonbackground));
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Return to Main Menu", basebuttonbackground));
    InitializeVolumeControl(); 

    // Volume Buttons 
    // [Main Volume]
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(450, 300), 120, 60, 0, 0, bgm, CP_Color_Create(0, 0, 0, 0), None, CP_Color_Create(255, 255, 255, 255), " ", nobuttonbackground));
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(1300, 310), 50, 50, 0, 0, addvolume, CP_Color_Create(0, 0, 0, 0), Increase_Volume_BGM, CP_Color_Create(0, 0, 0, 0), " ", incvolbuttonbackground));
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(650, 300), 50, 25, 0, 0, decvolume, CP_Color_Create(0, 0, 0, 0), Decrease_Volume_BGM, CP_Color_Create(0, 0, 0, 0), " ", decvolbuttonbackground));

    // [SFX]
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(450, 600), 120, 60, 0, 0, sfx, CP_Color_Create(0, 0, 0, 0), None, CP_Color_Create(255, 255, 255, 255), " ", nobuttonbackground));
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(1300, 610), 50, 50, 0, 0, addvolume, CP_Color_Create(0, 0, 0, 0), Increase_Volume_SFX, CP_Color_Create(0, 0, 0, 0), " ", incvolbuttonbackground));
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(650, 600), 50, 25, 0, 0, decvolume, CP_Color_Create(0, 0, 0, 0), Decrease_Volume_SFX, CP_Color_Create(0, 0, 0, 0), " ", decvolbuttonbackground));

    
    // Level 1 (warm up) - 1 container (20 balls)
    screen_array[Level_1].LineArrayLengthCounter = 0;
    screen_array[Level_1].CircleArrayLengthCounter = 0;
    screen_array[Level_1].overlay_name = pause_overlay;
    AddScoringContainer(&screen_array[Level_1], createScoringContainer(newVector2(600, 600), 0, 100, 100, 20));
    AddBallSpawner(&screen_array[Level_1], CreateBallSpawner(newVector2(200, 250), 100.0f, 100.0f, 0, 2.0f, true, 400, 15, Spawner));
    

    // Level 2 - 2 containers 
    screen_array[Level_2].LineArrayLengthCounter = 0;
    screen_array[Level_2].CircleArrayLengthCounter = 0;
    screen_array[Level_2].overlay_name = pause_overlay;
    AddBoxGameObject(&screen_array[Level_2], CreateBoxGameObject(newVector2(1000, 800), 200, 50, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddScoringContainer(&screen_array[Level_2], createScoringContainer(newVector2(1000, 700), 0, 100, 100, 30));
    AddBoxGameObject(&screen_array[Level_2], CreateBoxGameObject(newVector2(1200, 500), 200, 50, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddScoringContainer(&screen_array[Level_2], createScoringContainer(newVector2(1300, 400), 0, 100, 100, 35));
    AddBallSpawner(&screen_array[Level_2], CreateBallSpawner(newVector2(400, 200), 100.0f, 100.0f, 90, 2.0f, true, 400, 15, Spawner));

    // Level 3 - introduction to super-bounce - 3 containers
    screen_array[Level_3].LineArrayLengthCounter = 0;
    screen_array[Level_3].CircleArrayLengthCounter = 0;
    screen_array[Level_3].overlay_name = pause_overlay;

    AddBouncePlatform(&screen_array[Level_3], createSuperBouncePlatform(newVector2(200, 1000), 0, 350, 50, 1.5));
    AddBoxGameObject(&screen_array[Level_3], CreateBoxGameObject(newVector2(1000, 800), 400, 50, 1.0f, 0, NULL,CP_Color_Create(0,0,0,255)));
    AddBoxGameObject(&screen_array[Level_3], CreateBoxGameObject(newVector2(1200, 350), 600, 50, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_3], CreateBoxGameObject(newVector2(1800, 350), 50, 600, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddScoringContainer(&screen_array[Level_3], createScoringContainer(newVector2(450, 350), 0, 100, 100, 20));
    AddScoringContainer(&screen_array[Level_3], createScoringContainer(newVector2(1200, 250), 0, 100, 100, 30));
    AddScoringContainer(&screen_array[Level_3], createScoringContainer(newVector2(1350, 550), 0, 100, 100, 30));
    AddBallSpawner(&screen_array[Level_3], CreateBallSpawner(newVector2(300, 150), 100.0f, 100.0f, 90, 2.0f, true, 400, 15, Spawner));

    // Level 4 - 
    screen_array[Level_4].LineArrayLengthCounter = 0;
    screen_array[Level_4].CircleArrayLengthCounter = 0;
    screen_array[Level_4].overlay_name = pause_overlay;
    AddBouncePlatform(&screen_array[Level_4], createSuperBouncePlatform(newVector2(400, 750), 45, 250, 50, 1.5));
    AddBouncePlatform(&screen_array[Level_4], createSuperBouncePlatform(newVector2(1200, 350), 45, 250, 50, 1.5));
    AddScoringContainer(&screen_array[Level_4], createScoringContainer(newVector2(400, 350), 0, 100, 100, 20));
    AddScoringContainer(&screen_array[Level_4], createScoringContainer(newVector2(1200, 800), 0, 100, 100, 20));
    AddScoringContainer(&screen_array[Level_4], createScoringContainer(newVector2(1500, 200), 0, 100, 100, 20));
    AddBallSpawner(&screen_array[Level_4], CreateBallSpawner(newVector2(1000, 150), 100.0f, 100.0f, 120, 2.0f, true, 400, 15, Spawner));

    // Level 5 - introduce no draw zones
    screen_array[Level_5].LineArrayLengthCounter = 0;
    screen_array[Level_5].CircleArrayLengthCounter = 0;
    screen_array[Level_5].overlay_name = pause_overlay;
    AddNoDrawZone(&screen_array[Level_5], createNoDrawZone(newVector2(900, 350), 0, 400, 400));
    AddScoringContainer(&screen_array[Level_5], createScoringContainer(newVector2(600, 350), 0, 100, 100, 30));
    AddScoringContainer(&screen_array[Level_5], createScoringContainer(newVector2(1000, 800), 0, 100, 100, 30));
    AddScoringContainer(&screen_array[Level_5], createScoringContainer(newVector2(1500, 200), 0, 100, 100, 25));
    AddBallSpawner(&screen_array[Level_5], CreateBallSpawner(newVector2(250, 800), 100.0f, 100.0f, 315, 1.0f, true, 550, 15, Spawner));

    //Level 6
    screen_array[Level_6].LineArrayLengthCounter = 0;
    screen_array[Level_6].CircleArrayLengthCounter = 0;
    screen_array[Level_6].overlay_name = pause_overlay;
    AddBallSpawner(&screen_array[Level_6], CreateBallSpawner(newVector2(100, 450), 100.0f, 100.0f, 0, 2.0f, true, 500, 30, Spawner));
    AddNoDrawZone(&screen_array[Level_6], createNoDrawZone(newVector2(350, 0), 0, 900, 1400));
    AddNoDrawZone(&screen_array[Level_6], createNoDrawZone(newVector2(0, 300), 0, 350, 1400));
    AddBouncePlatform(&screen_array[Level_6], createSuperBouncePlatform(newVector2(1700, 500), 45, 250, 50, 1.65f));
    AddScoringContainer(&screen_array[Level_6], createScoringContainer(newVector2(100, 100), 0, 100, 100, 50));

    //Level 7
    screen_array[Level_7].LineArrayLengthCounter = 0;
    screen_array[Level_7].CircleArrayLengthCounter = 0;
    screen_array[Level_7].overlay_name = pause_overlay;
    AddBallSpawner(&screen_array[Level_7], CreateBallSpawner(newVector2(975, 975), 100.0f, 100.0f, 270, 2.0f, true, 300, 45, Spawner));
    AddNoDrawZone(&screen_array[Level_7], createNoDrawZone(newVector2(0,50), 0, 1975, 650));
    AddNoDrawZone(&screen_array[Level_7], createNoDrawZone(newVector2(50, 0), 0, 1825, 50));
    AddScoringContainer(&screen_array[Level_7], createScoringContainer(newVector2(0, 0), 0, 50, 50, 50));
    AddScoringContainer(&screen_array[Level_7], createScoringContainer(newVector2(1875, 0), 0, 50, 50, 50));
    AddBouncePlatform(&screen_array[Level_7], createSuperBouncePlatform(newVector2(50, 800), 0, 50, 25, 1.65f));
    AddBouncePlatform(&screen_array[Level_7], createSuperBouncePlatform(newVector2(100, 900), 0, 50, 25, 1.65f));
    AddBouncePlatform(&screen_array[Level_7], createSuperBouncePlatform(newVector2(1800, 900), 0, 50, 25, 1.65f));
    AddBouncePlatform(&screen_array[Level_7], createSuperBouncePlatform(newVector2(1850, 800), 0, 50, 25, 1.65f));

    //Level 8 
    screen_array[Level_8].LineArrayLengthCounter = 0;
    screen_array[Level_8].CircleArrayLengthCounter = 0;
    screen_array[Level_8].overlay_name = pause_overlay;
    AddCircleportalpair(&screen_array[Level_8], Createcircleportalpair(newVector2(875, 350), newVector2(250, 500), CP_Color_Create(255, 255, 255, 255), NULL, 25, 300));
    AddBoxGameObject(&screen_array[Level_8], CreateBoxGameObject(newVector2(500, 0), 25, 1200 , 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBallSpawner(&screen_array[Level_8], CreateBallSpawner(newVector2(50, 0), 50.0f, 50.0f, 45, 2.0f, true, 300, 45, Spawner));
    AddNoDrawZone(&screen_array[Level_8], createNoDrawZone(newVector2(800, 0), 0,950 , 450));
    AddScoringContainer(&screen_array[Level_8], createScoringContainer(newVector2(1000, 0), 0, 50, 50, 50));
    AddScoringContainer(&screen_array[Level_8], createScoringContainer(newVector2(1500, 0), 0, 50, 50, 50));
    AddScoringContainer(&screen_array[Level_8], createScoringContainer(newVector2(1500, 900), 0, 50, 50, 50));
    AddBouncePlatform(&screen_array[Level_8], createSuperBouncePlatform(newVector2(1700, 400), 135, 125, 25, 1.3f));
    AddBouncePlatform(&screen_array[Level_8], createSuperBouncePlatform(newVector2(900, 400), 45, 125, 25, 1.3f));

    //Level 9
    screen_array[Level_9].LineArrayLengthCounter = 0;
    screen_array[Level_9].CircleArrayLengthCounter = 0;
    screen_array[Level_9].overlay_name = pause_overlay;
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(0,700), 2000, 25, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(300,700), 25, 500, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(1800, 600), 25, 100, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(1700, 400), 25, 250, 1.0f, 270, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(200, 400), 25, 250, 1.0f, 90, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_9], CreateBoxGameObject(newVector2(1800, 600), 25, 100, 1.0f, 90, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBallSpawner(&screen_array[Level_9], CreateBallSpawner(newVector2(50, 0), 50.0f, 50.0f, 45, 2.0f, true, 300, 15, Spawner));
    AddBallSpawner(&screen_array[Level_9], CreateBallSpawner(newVector2(1900, 35), 50.0f, 50.0f, 135, 2.0f, true, 300, 15, Spawner));
    AddCircleportalpair(&screen_array[Level_9], Createcircleportalpair(newVector2(100, 75), newVector2(1800, 95), CP_Color_Create(255, 125, 255, 255), NULL, 25, 300));
    AddCircleportalpair(&screen_array[Level_9], Createcircleportalpair(newVector2(100, 650), newVector2(1800, 800), CP_Color_Create(255, 255, 125, 255), NULL, 25, 300));
    AddCircleportalpair(&screen_array[Level_9], Createcircleportalpair(newVector2(1750, 650), newVector2(100, 800), CP_Color_Create(0, 0, 0, 25), NULL, 25, 300));
    AddCircleportalpair(&screen_array[Level_9], Createcircleportalpair(newVector2(900, 75), newVector2(400, 800), CP_Color_Create(125, 255, 255, 255), NULL, 25, 300));
    AddScoringContainer(&screen_array[Level_9], createScoringContainer(newVector2(100, 900), 0, 50, 50, 100));
    AddScoringContainer(&screen_array[Level_9], createScoringContainer(newVector2(975, 900), 0, 50, 50, 100));

    //Level 10
    screen_array[Level_10].LineArrayLengthCounter = 0;
    screen_array[Level_10].CircleArrayLengthCounter = 0;
    screen_array[Level_10].overlay_name = pause_overlay;
    AddNoDrawZone(&screen_array[Level_10], createNoDrawZone(newVector2(1500, 0), 0, 450, 400));
    AddNoDrawZone(&screen_array[Level_10], createNoDrawZone(newVector2(1300, 0), 0, 200, 900));
    AddBallSpawner(&screen_array[Level_10], CreateBallSpawner(newVector2(1850, 0), 50.0f, 50.0f, 90, 2.0f, true, 300, 15, Spawner));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(1500, 0), 2000, 25, 1.0f, 90, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(1500, 150),25, 100, 1.0f, 270, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(1700, 0), 25, 100, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(0, 800), 250, 25, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(250, 800), 25, 400, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(1300, 0), 800, 25, 1.0f, 90, NULL, CP_Color_Create(0, 0, 0, 255)));
    AddBoxGameObject(&screen_array[Level_10], CreateBoxGameObject(newVector2(250, 800), 800, 25, 1.0f, 0, NULL, CP_Color_Create(0, 0, 0, 5)));
    AddBouncePlatform(&screen_array[Level_10], createSuperBouncePlatform(newVector2(1500, 400), 0, 125, 25, 1.015f));
    AddBouncePlatform(&screen_array[Level_10], createSuperBouncePlatform(newVector2(1800, 900), 0, 125, 25, 1.015f));
    AddBouncePlatform(&screen_array[Level_10], createSuperBouncePlatform(newVector2(1300, 900), 0, 175, 25, 1.65f));
    AddCircleportalpair(&screen_array[Level_10], Createcircleportalpair(newVector2(1550, 50), newVector2(100, 50), CP_Color_Create(125, 255, 255, 255), NULL, 25, 300));
    AddCircleportalpair(&screen_array[Level_10], Createcircleportalpair(newVector2(1550, 1000), newVector2(100, 900), CP_Color_Create(255, 125, 255, 255), NULL, 25, 200));
    AddCircleportalpair(&screen_array[Level_10], Createcircleportalpair(newVector2(1335, 35), newVector2(300, 900), CP_Color_Create(255, 255, 125, 25), NULL, 25, 300));
    AddCircleportalpair(&screen_array[Level_10], Createcircleportalpair(newVector2(1900, 1000), newVector2(200, 1000), CP_Color_Create(125, 255, 125, 0), NULL, 25, 200));
    AddScoringContainer(&screen_array[Level_10], createScoringContainer(newVector2(1400, 0), 0, 50, 50, 50));
    AddScoringContainer(&screen_array[Level_10], createScoringContainer(newVector2(1400, 950), 0, 50, 50, 50));
}

void Initialize_Sprites(void) {
    L1 = CP_Image_Load("./Assets/Art/Level1.png");
    L2 = CP_Image_Load("./Assets/Art/Level2.png");
    L3 = CP_Image_Load("./Assets/Art/Level3.png");
    L4 = CP_Image_Load("./Assets/Art/Level4.png");
    L5 = CP_Image_Load("./Assets/Art/Level5.png");
    L6 = CP_Image_Load("./Assets/Art/Level6.png");
    L7 = CP_Image_Load("./Assets/Art/Level7.png");
    L8 = CP_Image_Load("./Assets/Art/Level8.png");
    L9 = CP_Image_Load("./Assets/Art/Level9.png");
    L10 = CP_Image_Load("./Assets/Art/Level10.png");
    mainpage = CP_Image_Load("./Assets/Art/Main.png");
    pausepage = CP_Image_Load("./Assets/Art/Pause.png");
    victorypage = CP_Image_Load("./Assets/Art/Victory.png");
    DigipenLogo = CP_Image_Load("./Assets/Art/DigiPen_WHITE.png");
    volumepage = CP_Image_Load("./Assets/Art/volume_title.png");
    bgm = CP_Image_Load("./Assets/Art/BGM.png");
    sfx = CP_Image_Load("./Assets/Art/sfx.png");
    Credit1 = CP_Image_Load("./Assets/Art/credit1.png");
    Credit2 = CP_Image_Load("./Assets/Art/credit2.png");
    Credit3 = CP_Image_Load("./Assets/Art/credit3.png");
    GameLogo = CP_Image_Load("./Assets/Art/GameLogo.png");
    HowToPlay = CP_Image_Load("./Assets/Art/How to play.png");


    //initialize base button background
    basebuttonbackground.border = CP_Image_Load("./Assets/Buttons/ButtonDeactivated.png");
    basebuttonbackground.borderalpha = 255;
    basebuttonbackground.hoverbg = CP_Image_Load("./Assets/Buttons/ButtonHovered.png");
    basebuttonbackground.hoveralpha = 0;
    basebuttonbackground.clickedbg = CP_Image_Load("./Assets/Buttons/ButtonDown.png");
    basebuttonbackground.clickedalpha = 0;

    // Initialize increase volume button
    incvolbuttonbackground.border = CP_Image_Load("./Assets/Buttons/Add_Volume.png"); 
    incvolbuttonbackground.borderalpha = 255; 
    incvolbuttonbackground.hoverbg = NULL; 
    incvolbuttonbackground.hoveralpha = 0;
    incvolbuttonbackground.clickedbg = NULL; 
    incvolbuttonbackground.clickedalpha = 0;

    // Initialize decrease volume button 
    decvolbuttonbackground.border = CP_Image_Load("./Assets/Buttons/Dec_Volume.png");
    decvolbuttonbackground.borderalpha = 255;
    decvolbuttonbackground.hoverbg = NULL;
    decvolbuttonbackground.hoveralpha = 0;
    decvolbuttonbackground.clickedbg = NULL;
    decvolbuttonbackground.clickedalpha = 0;
    //initialise background for spawner
    Spawner = CP_Image_Load("./Assets/Art/Spawner.png");
}

void UpdateAllSpawners(void)
{
    for (int i = 0; i < current_screen->BallSpawnerArrayLengthCounter; i++) {
        BallSpawner* bs = &current_screen->BallSpawnerArray[i];
        if (bs->isSpawning == true)
        {
            bs->internalTimer -= FrameTime;
            if (bs->internalTimer <= 0)
            {
                float ballVelocity = bs->spawnVel;
                float spreadAngle = bs->spreadAngle;
                float scale = bs->ballScale + CP_Random_RangeFloat(-bs->ballScaleSpread, bs->ballScaleSpread);

                // Ball spawn position: Take center of box, add the box's "forward" vector scaled by the (width/2)*1.1 number
                Vector2 positionVector = VectorAdd(CenterOfBox(&bs->b), VectorMultiply(BoxForward(&bs->b), (bs->b.width/2) * 1.1f));

                // Temp Velocity for testing
                Vector2 tempVel = VectorMultiply(BoxForward(&bs->b), ballVelocity);
                float randAng = CP_Random_RangeFloat(-spreadAngle, spreadAngle);
                tempVel = RotateVector(tempVel, randAng);

                CircleGameObject tempvariable = CreateCircleGameObject(positionVector, tempVel, 0, RandomColor(), scale, false, scale, 1);
                AddBall(current_screen, tempvariable);

                bs->internalTimer = bs->internalTimer + 1.0f / bs->spawnrate;
            }

        }
    }
    
}

void AddVolumeMeter(Vector2 position) {
    float rect_posx = position.x;
    float rect_posy = position.y;

    float width = 50; 
    float height = 50; 

    CP_Graphics_DrawRect(position.x, position.y, width, height);
    

    // Draw 4 other rects (5 rectangles in total)
    for (int i = 0; i < 4; i++) {
        rect_posx += 50; 
        rect_posy += 50;
        CP_Graphics_DrawRect(rect_posx, rect_posy, width, height); 
    }
}

void InitializeVolumeControl() {
    
    // Volume Meter -  BGM 
    
    // 2 meters of 5 elements. - total array of 10 elements to be accessed
    // [BGM] - Elements 0 - 4, [SFX] - Elements 5 - 9
    // [BGM] - 1 row, [SFX] - 1 row
    // 2 rows of volume meter to initialize
    for (int i = 0; i < 2; i++) {
        // 5 cols / cells of the volume meter to initalize
        for (int j = 0; j < 5; j++) {

            if (i == 0) {
                float rect_distance_from_1st = (float)j * 100; // consecutively initialize boxes side by side. 
                screen_array[Options].VolumeObjectArray[j] = CreateVolumeObject(newVector2(750 + rect_distance_from_1st, 300), 100, 50, CP_Color_Create(0,0,0,0), true);
               /* screen_array[Options].VolumeObjectArray[j] = CreateVolumeObject(newVector2(750 + rect_distance_from_1st, 300), 100, 50, COLOR_PASTEL_GREEN, true);*/
            }

            
            if (i == 1) {
                float rect_distance_from_1st = (float)j * 100; // consecutively initialize boxes side by side. 
                screen_array[Options].VolumeObjectArray[5 + j] = CreateVolumeObject(newVector2(750 + rect_distance_from_1st, 600), 100, 50, CP_Color_Create(0, 0, 0, 0), true);
                //screen_array[Options].VolumeObjectArray[j] = CreateVolumeObject(newVector2(750 + rect_distance_from_1st, 300), 100, 50, COLOR_PASTEL_GREEN, true);
            }
         
        }
    }
}

void game_exit(void)
{

}




