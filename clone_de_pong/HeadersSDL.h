/*
    HEADERS SDL
*/

#ifdef WIN32 // Windows Operating System
    #include "SDL.h"
    #include "SDL_image.h"
    #include "SDL_mixer.h"
    #include "SDL_ttf.h"
#else        // *nix Operating System
    #include "SDL/SDL.h"
    #include "SDL/SDL_image.h"
    #include "SDL/SDL_mixer.h"
    #include "SDL/SDL_ttf.h"
#endif

#include "Structures.cpp"
#include "Timer.h"

#include <vector>
#include <string>

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

// The surfaces of gameplay
SDL_Surface *screen = NULL;
SDL_Surface *ball = NULL;
SDL_Surface *palette = NULL;

// The surfaces of opening
SDL_Surface *background = NULL;
SDL_Surface *messageKiddo1 = NULL;
SDL_Surface *messageSoft2 = NULL;
SDL_Surface *messageOpen = NULL;
SDL_Surface *messageTitle = NULL;
SDL_Surface *pressEnter = NULL;
SDL_Surface *instructionsSurface = NULL;

// The surfaces of Main menu
SDL_Surface *startGame_selected = NULL;
SDL_Surface *startGame_unselected = NULL;
SDL_Surface *quitGame_selected = NULL;
SDL_Surface *quitGame_unselected = NULL;
SDL_Surface *instructions_selected = NULL;
SDL_Surface *instructions_unselected = NULL;

// The font of opening
TTF_Font *fontKiddo = NULL;
TTF_Font *fontTitle = NULL;
TTF_Font *fontOpen = NULL;

// The colors of the fonts
SDL_Color textKiddoColor = { 0, 0, 255 };
SDL_Color textSoftColor = { 255, 0, 0 };
SDL_Color textOpenColor = { 255, 255, 255 };
SDL_Color textTitleColor = { 255, 255, 255 };

// Walls to mark the field of game
SDL_Rect middleWall;
SDL_Rect leftMark;
SDL_Rect rightMark;

// The event structure
SDL_Event event;


//The musics that will be played
Mix_Music *musicKipong = NULL;
Mix_Music *musicOpening = NULL;

// The sound effects that will be used
Mix_Chunk *ballbounce = NULL;
Mix_Chunk *ballhit = NULL;
Mix_Chunk *enter = NULL;

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */



/*
    CLASSES
*/

class Palette
{
    private:

    // Offset, width and height
    Box box;
    // Starting position of x offset
    Box startPosition;

    // The velocity of the palette
    float yVel, xVel;

    // Player 1 or player 2
    int player;

    public:

    // Constructor
    Palette();

    // Start the palette position and set player
    Palette( float x, float y, int playerControl );

    // Takes the key presses and adjusts the palette's velocity
    void handle_input();

    // Moves the palette
    void move( Uint32 deltaTicks, std::vector<SDL_Rect> rectsWall, Circle c );

    // Show the palette on the screen
    void show();

    // Return the palette's box
    Box get_box();

    // Start the palette position
    void start( int positionX, int positionY, int playerControl );
};

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

// The Ball class
class Ball
{
    private:
    // The offsets of the ball
    Circle c;

    // The velocity of the ball
    float xVel, yVel;

    public:

    // Constructor
    Ball();

    Ball( int x, int y );

    // Changes movement
    void changeMovement();

    // Start the ball movement
    void start();

    // Moves the ball
    void move( Uint32 deltaTicks, std::vector<SDL_Rect> rectsWal, Box leftBox, Box rightBox, float delta_vel_x, float delta_vel_y );

    // Shows the ball on the screen
    void show();

    // Get the circle of ball
    Circle get_circle();

    // Set the ball's position
    void set_position( int x, int y );
};

class Walls
{
    private:
    // The walls which delimitate the Pong
    //SDL_Rect leftWall;
    //SDL_Rect rightWall;
    SDL_Rect topWall;
    SDL_Rect bottomWall;

    // Vector to store the walls
    std::vector<SDL_Rect> box;

    public:

    // Set the walls coordenates
    void set_walls();

    // Shows the walls
    void show_walls();

    // Fill the vector with walls
    void put_walls_in_box();

    // Retrieve the walls box
    std::vector<SDL_Rect> get_box();
};

class MainMenu
{
    private:

    // The front surface alpha value
    //int alpha;

    // The items of menu
    int start, quit, instructions;

    // The frame rate regulator of main menu
    //Timer fps;

    public:

    void load_items();

    void show();

    int get_start_state();

    int get_quit_state();

    int get_instructions_state();

    void set_start_state( int state );

    void set_quit_state( int state );

    void set_instructions_state( int state );
};

// Manage the opening of game
class Opening
{
    private:
    // The frame rate regulator of opening
    Timer fps;

    // The Main Menu
    MainMenu main;

    // The front surface alpha value
    int alpha, alpha_press_enter;

    // The offsets of background
    int bgX, bgY;

    // Twinkle the message to press enter
    bool twinkle;

    // The twinkle state
    int twink_state;

    public:
    // The construtor
    Opening();

    // Apply the background image
    void apply_background();

    void handle_events();

    void set_start_state( int state );

    void set_quit_state( int state );

    void set_instructions_state( int state );
};

class Kipong
{
    private:
    // Ball of the game KiPong
    Ball ball;

    // Create the walls
    Walls walls;

    // Create the palettes
    Palette lPalette;
    Palette rPalette;

    // Palette movement's timers
    Timer deltaBall;
    Timer deltaPalette;

    public:

    Kipong();

    void start();

    void handle_events();

    void show();

    void match_result();
};



void clean_up()
{
    // Free the surfaces
    SDL_FreeSurface( ball );
    SDL_FreeSurface( palette );
    SDL_FreeSurface( background );
    SDL_FreeSurface( messageKiddo1 );
    SDL_FreeSurface( messageSoft2 );
    SDL_FreeSurface( messageOpen );
    SDL_FreeSurface( messageTitle );
    SDL_FreeSurface( startGame_selected );
    SDL_FreeSurface( startGame_unselected );
    SDL_FreeSurface( quitGame_selected );
    SDL_FreeSurface( quitGame_unselected );
    SDL_FreeSurface( instructions_selected );
    SDL_FreeSurface( instructions_unselected );
    SDL_FreeSurface( instructionsSurface );

    // Close the fonts that was used
    TTF_CloseFont( fontKiddo );
    TTF_CloseFont( fontOpen );
    TTF_CloseFont( fontTitle );

    // Quit SDL_ttf
    TTF_Quit();

    // Free the sound effects
    Mix_FreeChunk( ballbounce );
    Mix_FreeChunk( ballhit );
    Mix_FreeChunk( enter );

    // Free the musics
    Mix_FreeMusic( musicKipong );
    Mix_FreeMusic( musicOpening );

    //Quit SDL_mixer
    Mix_CloseAudio();

    SDL_Quit();
}
