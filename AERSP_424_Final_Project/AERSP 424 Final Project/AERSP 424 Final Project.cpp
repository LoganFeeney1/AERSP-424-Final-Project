/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AERSP 424 Final Project.cpp
Block Hopper
Lacy Harrington, Maxwell Bohn, Sana Yousef, Logan Feeney
3/17/24
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// HEADER FILES
#include <raylib.h>
#include <iostream>
#include <thread>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// USE OF STRUCTURE TO CREATE COMPOUND DATA TYPE CALLED "PICTURE"
struct picture
{
    Rectangle pic;      // defines a rectangle around an image to later draw
    Vector2 position;    // defines where to place the image on the popup window
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCTION CALL TO CHECK IF HOPPER IS ON THE GROUND
bool check_hopper_on_ground(picture check, int window_height, int spacer)
{
    return check.position.y >= window_height - check.pic.height - spacer;		// return true if hopper is on ground or false if hopper is in air
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ANY ADDITIONAL FUNCTIONS CAN GO HERE



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// START OF MAIN FUNCTION
int main()
{
    int win_dimension[2];        // array for popup window dimensions
    win_dimension[0] = 720;      // window width
    win_dimension[1] = 480;      // window height

    InitWindow(win_dimension[0], win_dimension[1], "Block Hopper");                 // create popup window

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD HOPPER CHARACTER
    Texture2D hopper = LoadTexture("images/Block_Hopper_Character.png");            // load hopper using raylib Texture2D data type

    const int spacer{ 93 };                                                         // spacer used to place hopper in the right position in the window                                                               

    picture hopper_info;										                    // Declare new variable of "picture" data type called "hopper_info" and initializing all of hopper_info's member variables below
    hopper_info.pic.x = 0;                                                          // identifies starting x-coord of hopper (works for pic member)
    hopper_info.pic.y = 0;                                                          // identifies starting y-coord of hopper (works for pic member)
    hopper_info.pic.width = hopper.width;                                           // identifies the full width of hopper (works for pic member)
    hopper_info.pic.height = hopper.height;                                         // identifies the full height of hopper (works for pic member)
    hopper_info.position.x = win_dimension[0] / 3 - hopper_info.pic.width / 2;      // places hopper perfectly in the middle of the screen (works for position member)
    hopper_info.position.y = win_dimension[1] - hopper_info.pic.height - spacer;    // places hopper on the bottom of the window with a spacer (works for position member)

    // JUMP CONDITIONS FOR HOPPER
    int velocity{};					                                                // sets vertical velocity equal to zero so hopper does not move when not jumping (pixels/second)
    const int jump_vel{ -791 };		                                                // velocity that hopper starts moving upward with when the spacebar is hit (pixels/second)
    const int gravity{ 2000 };		                                                // acceleration due to gravity (pixels/second)/second ---> pulls hopper back down after jumping
    bool in_air{};					                                                // boolean value initialized to false that determines if hopper is in the air

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD SPIKE
    //Texture2D spike = LoadTexture("images/Block_Hopper_Character.png");            // load spike using raylib Texture2D data type



    // BLANK SPACE
    


    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD BACKGROUND
    Texture2D background = LoadTexture("images/Background.png");                    // load background using raylib Texture2D data type
    float bg_move{};                                                                // variable declaration to allow the background to move

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // INITIALIZE COLLISION VARIABLE
    bool collision{};                       // initialize to false and as soon as collision goes true in the while loop, it stays true and game ends

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //  FRAME RATE SETTINGS
    const int fps = 60;		                // initialize the target frames per second
    SetTargetFPS(fps);		                // set target fps for popup window

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //  SETTING UP GAME
    while (!WindowShouldClose())            // boolean raylib function that turns true when the popup window "x" or "esc" is pressed. Conditions will then go "not true" and while loop ends
    {
        BeginDrawing();                     // raylib function to start drawing in the command window
        ClearBackground(WHITE);             // set white background to avoid double buffering

        const float dt{ GetFrameTime() };   // documents time between each frame every while loop iteration to avoind movement and frame coupling issue

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // SET UP THE BACKGROUND
        const int bg_move_speed{ 50 };                              // declare and initialize background move speed
        bg_move -= bg_move_speed * dt;                              // define how background moves (value linearly grows with every iteration)

        // BACKGROUND 1
        Vector2 bg_1_position{};                                    // variable declaration to define the position of background 1 (initialized to zero)
        bg_1_position.x = bg_move;                                  // assign x-position to background movement
        DrawTextureEx(background, bg_1_position, 0.0, 1.0, WHITE);  // draw background 1

        // BACKGROUND 2
        Vector2 bg_2_position{};                                    // variable declaration to define the position of background 2 (initialized to zero)
        bg_2_position.x = bg_move + background.width;               // assign x-position to background movement (shifted by one background width)
        DrawTextureEx(background, bg_2_position, 0.0, 1.0, WHITE);  // draw background 2

        if (bg_move <= -background.width)                           // when the first background has shifted one complete width, reset position
        {
            bg_move = 0;                                            // reset position
        }

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // GETTING TO KNOW HOPPER

        // GROUND CHECK
        if ( check_hopper_on_ground(hopper_info, win_dimension[1], spacer) )                // if hopper is on the ground
        {
            velocity = 0;										                            // set vertical velocity to zero
            hopper_info.position.y = win_dimension[1] - hopper_info.pic.height - spacer;	// set hopper to the floor
            in_air = false;									                                // hopper is not in the air
        }
        else                                                                                // if hopper is jumping
        {
            velocity += gravity * dt;							                            // apply gravity to hopper's vertical velocity until hopper returns to the ground
            in_air = true;										                            // hopper is in the air
        }

        // MAKE HOPPER JUMP
        if (IsKeyPressed(KEY_SPACE) && !in_air)                                             // if hopper is "not in the air" and the user hits the space bar
        {
            velocity = jump_vel;								                            // change hoppers vertical velocity to the initial jump velocity
        }

        // POSITION UPDATE
        hopper_info.position.y += velocity * dt;                                            // new vertical position = old position + (velocity * change in time)

        // DRAW HOPPER
        DrawTextureRec(hopper, hopper_info.pic, hopper_info.position, WHITE);               // draw hopper

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // GETTING TO KNOW SPIKE



        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // COLLISION - HOPPER VS. SPIKE ROUND 1



        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        EndDrawing();           // all done
    }

    UnloadTexture(hopper);      // unload a loaded texture from Texture2D
    UnloadTexture(background);  // unload a loaded texture from Texture2D
    CloseWindow();              // shuts everything down properly

    return 0;
}
