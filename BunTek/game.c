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
 * Copyright � 2020 DigiPen, All rights reserved.
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
void Initialize_SuperBounce(void); 
void UpdateAllSpawners(void);
void Initialize_Screen_Keys(void);


//Initialize array
Screen screen_array[Total_screen_number];
Screen overlay_array[Total_overlay_number];

int key_count = 0;
Screen_keys Screen_key_array[Total_screen_number];

//Initialize Screen control
Screen_name Start_Screen = Splash_screen;
Screen_name Current_screen_name;
Screen_name Next_screen_name;

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

//Sprites
CP_Image TestDoge = NULL;
CP_Image DigipenLogo = NULL;
CP_Image Spawner = NULL;
ButtonbgInfo basebuttonbackground;
ButtonbgInfo nobuttonbackground;



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
            UpdateAllSpawners();
        }
    }
    DrawAllShapes();

    Drawoverlay(&isoverlayTransiting, &isoverlayActive, &isgamepaused, current_screen->overlay_name, current_overlay);
    if (restartingLevel) {
        Restart_transition(&restartingLevel, &isoverlayActive, &isgamepaused, &Current_screen_name, current_screen, screen_array, current_overlay, overlay_array);
    }
    else {
        Screen_transition(&isScreenTransiting, &isoverlayActive, &transition_opacity, &Current_screen_name, &Next_screen_name, current_screen, screen_array, current_overlay, overlay_array);
    }

    // Testing 
    
    



    // Profiling info and frameRate testing
    if (debug) draw_framerate();
}

void AddLine(void) {
    
    if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {

        colided = false;  // reset boolean. 

        for (int i = 0; i < Current_screen.LineArrayLengthCounter; i++) {
            BoxGameObject* x = &Current_screen.NoDrawZoneArray[i];

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
                Current_screen.LineArray[Current_screen.LineArrayLengthCounter] = CreateBoxGameObject(LineStartPos, LineLength, 4.0f, DrawnLineBounciness, atan2f(LineEndPos.y - LineStartPos.y, LineEndPos.x - LineStartPos.x) / (float)PI * 180, TestDoge);

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
                CP_Font_DrawTextBox(x->buttontext, x->boxGameObject.gameObject.position.x, x->boxGameObject.gameObject.position.y + x->boxGameObject.height/2, x->boxGameObject.width);
            }

        }
    }
    // Draw Obstacles -> Draw Super Bounce Platform
    for (int i = 0; i < BoxGameObjectArrayLength; i++)
    {
        BoxGameObject* x = &Current_screen.SuperBouncePlatformArray[i];

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


     // Draw Obstacles -> Draw no-draw zone
    for (int i = 0; i < BoxGameObjectArrayLength; i++) {

        BoxGameObject* x = &Current_screen.NoDrawZoneArray[i];

        if (x->width == 0 || x->height == 0) {

        }
        else {
            CP_Settings_Fill(x->gameObject.color);

            CP_Graphics_DrawRectAdvanced(x->gameObject.position.x, x->gameObject.position.y, x->width, x->height, x->gameObject.angle, 1);

        }
    }

    for (int i = 0; i < SpawnerGameObjectArrayLength; i++)
    {
        BallSpawner* x = &Current_screen.BallSpawnerArray[i];
        if (x->b.image != NULL)
        {
            DrawBoxImage(&x->b, 255);
        }
        else
        {
           // CP_Graphics_DrawRectAdvanced(x->b.gameObject.position.x, x->b.gameObject.position.y, x->b.width, x->b.height, x->b.gameObject.angle, 1);
        }
    }

    // Draw Scoring Container
    for (int i = 0; i < BoxGameObjectArrayLength; i++) {

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
               
    

    /*
    for (int i = 0; i < Current_screen.RotatedboxportalpairArrayLengthCounter; i++) {
        Rotatedboxportalpair* rbpp = &Current_screen.RotatedboxportalpairArray[i];

        CP_Settings_Fill(rbpp->portal_1.gameObject.color);
        if (rbpp->portal_1.width == 0 || rbpp->portal_1.height == 0) {

        }
        else {
            CP_Settings_Fill(rbpp->portal_1.gameObject.color);
            CP_Settings_NoStroke();
            CP_Graphics_DrawRectAdvanced(rbpp->portal_1.gameObject.position.x, rbpp->portal_1.gameObject.position.y, rbpp->portal_1.width, rbpp->portal_1.height, rbpp->portal_1.gameObject.angle, 1);
            CP_Graphics_DrawRectAdvanced(rbpp->portal_2.gameObject.position.x, rbpp->portal_2.gameObject.position.y, rbpp->portal_2.width, rbpp->portal_2.height, rbpp->portal_2.gameObject.angle, 1);
        }
    }*/
        
    
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
        if (Current_screen.CircleGameObjectArray[i].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
        {
            //End of Array
            break;
        }
        else if(c1->teleportinfo.teleportStatus == Not_teleporting)
        {
            for (int o = i+1; o < Current_screen.CircleArrayLengthCounter; o++)
            {
                CircleGameObject* c2 = &Current_screen.CircleGameObjectArray[o];
                if (Current_screen.CircleGameObjectArray[o].radius == 0.0f) // TODO: SUB WITH CODE THAT DETECTS END OF ARRAY
                {
                    //End of Array
                    break;
                }
                else if(c2->teleportinfo.teleportStatus == Not_teleporting)
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
                        //PlayPitchedSoundEffect(BallBounce, 0.1f); //  Audio : ball bouncing off other balls 
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
                                c->teleportinfo.teleportStatus = Not_teleporting;
                            }
                        }
                    }
                    else if (!(collided_portal_1 || collided_portal_2) && (c->teleportinfo.teleportStatus != Not_teleporting)) {
                        if (c->radius != c->teleportinfo.original_radius) {
                            c->radius = c->teleportinfo.original_radius;
                        }
                        c->teleportinfo.teleportStatus = Not_teleporting;
                    }
                }
            }
                
        }

    }
    /*
    for (int x = 0; x < Current_screen.CircleportalpairArrayLengthCounter; x++) {
        Rotatedboxportalpair* rbpp = &Current_screen.RotatedboxportalpairArray[x];
        if (rbpp->portal_1.width == 0 || rbpp->portal_1.height == 0) {

        }
        else {

            for (int o = 0; o < Current_screen.CircleArrayLengthCounter; o++) {
                CircleGameObject* c = &Current_screen.CircleGameObjectArray[o];
                if (c->radius == 0.0f) {
                    break;
                }
                else {
                    Vector2 resultant;
                    bool collided_portal_1 = CircleRectCol(c, &rbpp->portal_1, false);
                    bool collided_portal_2 = CircleRectCol(c, &rbpp->portal_2, false);
                    float angleDiff = rbpp->portal_1.gameObject.angle - rbpp->portal_2.gameObject.angle;
                    if (collided_portal_1 && c->teleported == 0) {
                        resultant = RotateVector(VectorMinus(CenterOfBox(&rbpp->portal_1), c->gameObject.position), angleDiff);
                        c->gameObject.position = VectorAdd(CenterOfBox(&rbpp->portal_2), resultant);
                        c->gameObject.velocity = RotateVector(c->gameObject.velocity, angleDiff);
                        c->teleported = 2;

                    }
                    else if (collided_portal_2 && c->teleported == 0) {
                        resultant = RotateVector(VectorMinus(CenterOfBox(&rbpp->portal_2), c->gameObject.position), angleDiff);
                        c->gameObject.position = VectorAdd(CenterOfBox(&rbpp->portal_1), resultant);
                        c->gameObject.velocity = RotateVector(c->gameObject.velocity, angleDiff);
                        c->teleported = 2;
                    }
                    else if (!(collided_portal_1 ||collided_portal_2) && c->teleported == 2) {
                        c->teleported = 0;
                    }
                }
            }

        }

    }*/
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
    default:
        for (int i = 0; i < Total_screen_number; i++) {
            if (Screen_key_array[i].key == x->button_effect) {
                isgamepaused = false;
                Next_screen_name = Screen_key_array[i].value;
                isScreenTransiting = true;
                break;
            }
        }
        break;
    }
}

void Initialize_Screen_Keys(void) {
    AddScreenKey(&key_count, Move_to_Level_1, Level_1, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_2, Level_2, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_3, Level_3, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_4, Level_4, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_5, Level_5, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_6, Level_6, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_7, Level_7, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_8, Level_8, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_9, Level_9, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_10, Level_10, Screen_key_array);
    AddScreenKey(&key_count, Move_to_main_Menu, Main_menu, Screen_key_array);
    AddScreenKey(&key_count, Move_to_options, Options, Screen_key_array);
    AddScreenKey(&key_count, Move_to_credits, Credits_screen, Screen_key_array);
    AddScreenKey(&key_count, Move_to_tutorial, Tutorial, Screen_key_array);
    AddScreenKey(&key_count, Move_to_Level_Select, Level_Select, Screen_key_array);
}

void Initialize_Screens(void) {
    //create pause overlay;
    overlay_array[pause_overlay].ButtonObjectArrayLengthCounter = 0;
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(700, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Main Menu", basebuttonbackground));
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(900, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Restart, CP_Color_Create(255, 255, 255, 255), "Restart", basebuttonbackground));
    AddButton(&overlay_array[pause_overlay], CreateButtonObject(newVector2(1100, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Level Select", basebuttonbackground));

    //create victory overlay;
    overlay_array[victory_overlay].ButtonObjectArrayLengthCounter = 0;
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(700, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Restart, CP_Color_Create(255, 255, 255, 255), "Restart", basebuttonbackground));
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(900, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Next_Level, CP_Color_Create(255, 255, 255, 255), "Next Level", basebuttonbackground));
    AddButton(&overlay_array[victory_overlay], CreateButtonObject(newVector2(1100, 300), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Level Select", basebuttonbackground));

    //create splash screen
    screen_array[Splash_screen].ButtonObjectArrayLengthCounter = 0;
    AddButton(&screen_array[Splash_screen], CreateButtonObject(newVector2(0 + CP_System_GetWindowWidth() / 2.0f - 1026.0f / 2.0f, 0 + CP_System_GetWindowHeight() / 2.0f - 249.0f / 2.0f), 1026, 249, 50, 0, DigipenLogo, CP_Color_Create(255, 255, 255, 200), None, CP_Color_Create(255, 255, 255, 255), "", nobuttonbackground));


    //Create Main Menu Screen
    screen_array[Main_menu].ButtonObjectArrayLengthCounter = 0;
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 350), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_Level_Select, CP_Color_Create(255, 255, 255, 255), "Level Select", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 500), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_tutorial, CP_Color_Create(255, 255, 255, 255), "How to play", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 650), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_credits, CP_Color_Create(255, 255, 255, 255), "Credit", basebuttonbackground));
    AddButton(&screen_array[Main_menu], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Exit_game, CP_Color_Create(255, 255, 255, 255), "Quit game", basebuttonbackground));

    //Create Tutorial screen
    AddButton(&screen_array[Tutorial], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Main Menu", basebuttonbackground));

    //Create Credits
    AddButton(&screen_array[Credits_screen], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Main Menu", basebuttonbackground));

    //level select
    screen_array[Level_Select].ButtonObjectArrayLengthCounter = 0;
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
    AddButton(&screen_array[Level_Select], CreateButtonObject(newVector2(900, 800), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Main Menu", basebuttonbackground));


    screen_array[Options].ButtonObjectArrayLengthCounter = 0;
    AddButton(&screen_array[Options], CreateButtonObject(newVector2(900, 10), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Main Menu", basebuttonbackground));

    

    screen_array[Test_Room].LineArrayLengthCounter = 0;
    screen_array[Test_Room].CircleArrayLengthCounter = 0;
    AddButton(&screen_array[Test_Room], CreateButtonObject(newVector2(10, 10), 150, 75, 0, 0, NULL, CP_Color_Create(0, 0, 0, 0), Move_to_main_Menu, CP_Color_Create(255, 255, 255, 255), "Move to Main Menu", basebuttonbackground));
    
    screen_array[Level_1].LineArrayLengthCounter = 0;
    screen_array[Level_1].CircleArrayLengthCounter = 0;
    screen_array[Level_1].overlay_name = pause_overlay;
    screen_array[Level_1].SuperBouncePlatformArray[0] = createSuperBouncePlatform(newVector2(500, 500), 0, 100, 100); 
    screen_array[Level_1].NoDrawZoneArray[0] = createNoDrawZone(newVector2(1000, 500), 0,  100, 100);

    screen_array[Level_2].LineArrayLengthCounter = 0;
    screen_array[Level_2].CircleArrayLengthCounter = 0;
    screen_array[Level_2].overlay_name = pause_overlay; 
    screen_array[Level_2].NoDrawZoneArray[1] = createNoDrawZone(newVector2(1000, 500), 0, 100, 100); 

    screen_array[Level_3].LineArrayLengthCounter = 0;
    screen_array[Level_3].CircleArrayLengthCounter = 0;
    
    screen_array[Level_3].overlay_name = pause_overlay;

    screen_array[Level_4].LineArrayLengthCounter = 0;
    screen_array[Level_4].CircleArrayLengthCounter = 0;
    screen_array[Level_4].overlay_name = pause_overlay;
    screen_array[Level_4].BallSpawnerArray[0] = CreateBallSpawner(newVector2(700, 200), 100.0f, 100.0f, 120, 2.0f, true, 400, 15, Spawner);
    AddCircleportalpair(&screen_array[Level_4], Createcircleportalpair(newVector2(500, 500), newVector2(1000, 500), CP_Color_Create(255, 255, 255, 255), NULL, 50, 500));
    //AddRotatedboxportalpair(&screen_array[Level_4], Createrotatedboxportalpair(CreateBoxGameObject(newVector2(1000, 400), 50, 50, 0, 0, NULL), CreateBoxGameObject(newVector2(500, 400), 50, 50, 0, 135, NULL)));
    screen_array[Level_4].BallSpawnerArrayLengthCounter++;

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

    //Create Test Room screen
    //Fill up array with test circles
    for (int i = 0; i < 200; i++) {
        float scale = CP_Random_RangeFloat(10, 15);
        CircleGameObject tempc = CreateCircleGameObject(newVector2(CP_Random_RangeFloat(50, 1800), CP_Random_RangeFloat(50, 1000)), newVector2(CP_Random_RangeFloat(-100, 100), CP_Random_RangeFloat(-100, 100)), 0, CP_Color_Create(CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), CP_Random_RangeInt(0, 255), 255), scale, false, scale, 1);
        AddBall(&screen_array[Test_Room], tempc);
        AddBall(&screen_array[Level_1], tempc);
        AddBall(&screen_array[Level_2], tempc);
        AddBall(&screen_array[Level_3], tempc);
        AddBall(&screen_array[Level_10], tempc); // Using level 10 room to test score container. 
    }

    // Testing for Scoring Containers 
    ScoringContainerObject tempinit = createScoringContainer(newVector2(500, 500), 0, 200, 200, 100); 
    AddScoreContainer(&screen_array[Level_10], tempinit); 
}

void Initialize_Sprites(void) {
    TestDoge = CP_Image_Load("./Sprites/MahLe.jpg");
    DigipenLogo = CP_Image_Load("./Assets/DigiPen_WHITE.png");
    //initialize base button background
    basebuttonbackground.border = CP_Image_Load("./Assets/Buttons/ButtonDeactivated.png");
    basebuttonbackground.borderalpha = 255;
    basebuttonbackground.hoverbg = CP_Image_Load("./Assets/Buttons/ButtonHovered.png");
    basebuttonbackground.hoveralpha = 0;
    basebuttonbackground.clickedbg = CP_Image_Load("./Assets/Buttons/ButtonDown.png");
    basebuttonbackground.clickedalpha = 0;

    //initialise background for spawner
    Spawner = CP_Image_Load("./Assets/Spawner.png");
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

void game_exit(void)
{

}




