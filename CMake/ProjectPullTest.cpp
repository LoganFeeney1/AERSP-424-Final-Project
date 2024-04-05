/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AERSP 424 Final Project.cpp
Block airplane
Lacy Harrington, Maxwell Bohn, Sana Yousef, Logan Feeney
3/17/24
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// HEADER FILES
#include <raylib.h>
#include <iostream>
#include <thread>
//#include "code.h"        // our written library

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// USE OF STRUCTURE TO CREATE COMPOUND DATA TYPE CALLED "PICTURE"
struct picture
{
    Rectangle pic;      // defines a rectangle around an image to later draw
    Vector2 position;   // defines where to place the image on the popup window
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// GLOBAL VARIABLES
unsigned int bird_time_delay_spacer{ 200 };     // spacer that creates a time delay for the user before they need to start jumping
unsigned int bird_vert_spacer{ 300 };           // spacer that places the bird in the corrct vertical position
unsigned int tree_time_delay_spacer{ 600 };     // spacer that creates a time delay for the user before they need to start jumping
unsigned int tree_vert_spacer{ 100 };            // spacer used to place tree in the correct vertical position in popup window 
int vel_across_screen{ -300 };                  // speed at which all objects will move across the screen
float padding{ 0 };					        // buffer between the defined rectangle boundary and the images boundary inside rectangle

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCTION CALL TO CHECK IF airplane IS ON THE GROUND
bool check_airplane_on_ground(picture check, int window_height, int spacer)
{
    return check.position.y >= window_height - check.pic.height - spacer;		// return true if airplane is on ground or false if airplane is in air
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

    InitWindow(win_dimension[0], win_dimension[1], "Airplane Game!");               // create popup window

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD AIRPLANE
    Texture2D airplane = LoadTexture("images/airplane.png");                        // load airplane using raylib Texture2D data type

    const int ap_spacer{ 72 };                                                      // spacer used to place airplane in the correct vertical position in popup window                                                               

    picture airplane_info;										                    // Declare new variable of "picture" data type called "airplane_info" and initializing all of airplane_info's member variables below
    airplane_info.pic.x = 0;                                                        // identifies starting x-coord of airplane (works for pic member)
    airplane_info.pic.y = 0;                                                        // identifies starting y-coord of airplane (works for pic member)
    airplane_info.pic.width = airplane.width;                                       // identifies the full width of airplane (works for pic member)
    airplane_info.pic.height = airplane.height;                                     // identifies the full height of airplane (works for pic member)
    airplane_info.position.x = win_dimension[0] / 3 - airplane_info.pic.width / 2;  // places airplane perfectly in the middle of the screen (works for position member)
    airplane_info.position.y = win_dimension[1] - airplane_info.pic.height - ap_spacer;// places airplane on the bottom of the window with a spacer (works for position member)

    // JUMP CONDITIONS FOR airplane
    int ap_velocity{};					                                                // sets vertical velocity equal to zero so airplane does not move when not jumping (pixels/second)
    const int jump_vel{ -791 };		                                                // velocity that airplane starts moving upward with when the spacebar is hit (pixels/second)
    const int gravity{ 2000 };		                                                // acceleration due to gravity (pixels/second)/second ---> pulls airplane back down after jumping
    bool in_air{};					                                                // boolean value initialized to false that determines if airplane is in the air

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD BIRD
    Texture2D bird = LoadTexture("images/hopper.png");            // load bird using raylib Texture2D data type

    const int num_of_bird{ 3 };								// number of birds that will appear in the game
    const int bird_spacing{ 600 };								// spacing between each bird

    picture BIRDS[num_of_bird]{};							    // create array of data type "picture" to store "size_of_birds" number of birds

    for (int i = 0; i < num_of_bird; i++)                      // for loop to define bird image along with position on the popup window
    {
        BIRDS[i].pic.x = 0;									                            // create x-coord variable for bird
        BIRDS[i].pic.y = 0;									                            // create y-coord variable for bird
        BIRDS[i].pic.width = bird.width;				                                // create width variable for bird
        BIRDS[i].pic.height = bird.height;				                                // create height variable for bird
        BIRDS[i].position.x = win_dimension[0] + i * bird_spacing;	// create x-coord variable to place bird on screen
        BIRDS[i].position.y = win_dimension[1] - bird_vert_spacer;	                    // create y-coord variable to place bird on screen
    }

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD TREE
    Texture2D tree = LoadTexture("images/hopper.png");          // load tree using raylib Texture2D data type

    const int num_of_tree{ 3 };								    // number of trees that will appear in the game
    const int tree_spacing{ 600 };								// spacing between each tree

    picture TREES[num_of_tree]{};							    // create array of data type "picture" to store "size_of_trees" number of trees

    for (int i = 0; i < num_of_bird; i++)                       // for loop to define bird image along with position on the popup window
    {
        TREES[i].pic.x = 0;									            // create x-coord variable for bird
        TREES[i].pic.y = 0;									            // create y-coord variable for bird
        TREES[i].pic.width = tree.width;				                // create width variable for bird
        TREES[i].pic.height = tree.height;				                // create height variable for bird
        TREES[i].position.x += win_dimension[0] + i * tree_spacing;	    // create x-coord variable to place bird on screen
        TREES[i].position.y = win_dimension[1] - tree_vert_spacer;	    // create y-coord variable to place bird on screen
    }


    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // FINISH LINE CONDITION
    float finishline{ TREES[num_of_tree - 1].position.x };		// finish line variable that will occur immediately after the final tree
    const int buffer{ 400 };									// space after finish line when game ends

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // LOAD BACKGROUND
    Texture2D background = LoadTexture("images/runway_background.png");             // load background using raylib Texture2D data type
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
        const int bg_move_speed{ 70 };                              // declare and initialize background move speed
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
        // AIRPLANE

        // GROUND CHECK
        if (check_airplane_on_ground(airplane_info, win_dimension[1], ap_spacer))             // if airplane is on the ground
        {
            ap_velocity = 0;										                                // set vertical velocity to zero
            airplane_info.position.y = win_dimension[1] - airplane_info.pic.height - ap_spacer; // set airplane to the floor
            in_air = false;									                                    // airplane is not in the air
        }
        else                                                                                    // if airplane is jumping
        {
            ap_velocity += gravity * dt;							                            // apply gravity to airplane's vertical velocity until airplane returns to the ground
            in_air = true;										                                // airplane is in the air
        }

        // MAKE AIRPLANE JUMP
        if (IsKeyPressed(KEY_SPACE) && !in_air)                                                 // if airplane is "not in the air" and the user hits the space bar
        {
            ap_velocity = jump_vel;								                                // change airplanes vertical velocity to the initial jump velocity
        }

        // POSITION UPDATE
        airplane_info.position.y += ap_velocity * dt;                                           // new vertical position = old position + (velocity * change in time)

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // BIRD

        // POSITION UPDATE
        for (int i = 0; i < num_of_bird; i++)
        {
            BIRDS[i].position.x += vel_across_screen * dt;	    // new horizontal position = old position + (velocity * change in time)
        }

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // TREE

        // POSITION UPDATE
        for (int i = 0; i < num_of_tree; i++)
        {
            TREES[i].position.x += vel_across_screen * dt;	    // new horizontal position = old position + (velocity * change in time)
        }

        // FINISH LINE CONDITION WILL MOVE WITH THE FINAL TREE
        finishline += vel_across_screen * dt;                   // new horizontal position = old position + (velocity * change in time)

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // COLLISION - AIRPLANE VS. EVERYONE

        // CHECK COLLISION WITH BIRD
        for (picture bird : BIRDS)							    // Range based for loop. For each element in BIRDS array, creating a temp variable called bird and the loop executes for each element in the array
        {
            // define rectangle around bird
            Rectangle bird_rectangle
            {
                bird.position.x + padding,
                bird.position.y + padding,
                bird.pic.width - 2 * padding,
                bird.pic.height - 2 * padding
            };
            // define rectangle around airplane
            Rectangle ap_rectangle
            {
                airplane_info.position.x,
                airplane_info.position.y,
                airplane_info.pic.width,
                airplane_info.pic.height
            };
            if (CheckCollisionRecs(bird_rectangle, ap_rectangle))   // raylib function to define collision
            {
                collision = true;
            }
        }

        // CHECK COLLISION WITH TREE
        for (picture tree : TREES)							    // Range based for loop. For each element in BIRDS array, creating a temp variable called bird and the loop executes for each element in the array
        {
            // define rectangle around tree
            Rectangle tree_rectangle
            {
                tree.position.x + padding,
                tree.position.y + padding,
                tree.pic.width - 2 * padding,
                tree.pic.height - 2 * padding
            };
            // define rectangle around airplane
            Rectangle ap_rectangle
            {
                airplane_info.position.x,
                airplane_info.position.y,
                airplane_info.pic.width,
                airplane_info.pic.height
            };
            if (CheckCollisionRecs(tree_rectangle, ap_rectangle))   // raylib function to define collision
            {
                collision = true;
            }
        }

        // IF COLLISION IS TRUE, PLAYERS LOSSES GAME
        if (collision)
        {
            DrawText("You Lose!", win_dimension[0] / 3, win_dimension[1] / 2, 50, WHITE);
        }
        // IF COLLISION IS NOT TRUE AND PLAYER HAS CROSSED THE FINISH LINE, PLAYER WINS
        else if (airplane_info.position.x >= finishline + buffer)
        {
            // wind the game
            DrawText("You Win!", win_dimension[0] / 3, win_dimension[1] / 2, 50, WHITE);

            // DRAW AIRPLANE
            DrawTextureRec(airplane, airplane_info.pic, airplane_info.position, WHITE); // draw airplane
        }
        else
        {
            // DRAW BIRDS
            for (int i = 0; i < num_of_tree; i++)
            {
                DrawTextureRec(bird, BIRDS[i].pic, BIRDS[i].position, WHITE);           // draw birds
            }

            // DRAW TREES
            for (int i = 0; i < num_of_tree; i++)
            {
                DrawTextureRec(tree, TREES[i].pic, TREES[i].position, WHITE);           // draw trees  
            }

            // DRAW AIRPLANE
            DrawTextureRec(airplane, airplane_info.pic, airplane_info.position, WHITE); // draw airplane
        }

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        EndDrawing();           // all done
    }

    UnloadTexture(tree);        // unload a loaded texture from Texture2D
    UnloadTexture(bird);        // unload a loaded texture from Texture2D
    UnloadTexture(airplane);    // unload a loaded texture from Texture2D
    UnloadTexture(background);  // unload a loaded texture from Texture2D
    CloseWindow();              // shuts everything down properly

    //thankYou();             // call library function

    return 0;
}

