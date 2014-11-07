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
SDL_Surface *versusPlayer_selected = NULL;
SDL_Surface *versusPlayer_unselected = NULL;
SDL_Surface *quitGame_selected = NULL;
SDL_Surface *quitGame_unselected = NULL;
SDL_Surface *instructions_selected = NULL;
SDL_Surface *instructions_unselected = NULL;
// Options of Main menu
SDL_Surface *versusCPU_selected = NULL;
SDL_Surface *versusCPU_unselected = NULL;

// Levels of game
SDL_Surface *easyMode_selected = NULL;
SDL_Surface *easyMode_unselected = NULL;
SDL_Surface *normalMode_selected = NULL;
SDL_Surface *normalMode_unselected = NULL;
SDL_Surface *hardMode_selected = NULL;
SDL_Surface *hardMode_unselected = NULL;

// The credits screen
SDL_Surface *credits = NULL;

// The players
SDL_Surface *player1 = NULL;
SDL_Surface *player2 = NULL;
SDL_Surface *win = NULL;

// The round of match
SDL_Surface *roundMatch = NULL;

// The numbers of the round of match
SDL_Surface *round0 = NULL;
SDL_Surface *round1 = NULL;
SDL_Surface *round2 = NULL;
SDL_Surface *round3 = NULL;
SDL_Surface *round4 = NULL;
SDL_Surface *round5 = NULL;
SDL_Surface *round6 = NULL;
SDL_Surface *round7 = NULL;
SDL_Surface *round8 = NULL;
SDL_Surface *round9 = NULL;

// The font of opening
TTF_Font *fontKiddo = NULL;
TTF_Font *fontTitle = NULL;
TTF_Font *fontOpen = NULL;
// The fonts of options
TTF_Font *fontMenu = NULL;

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

// Level of the game
int level = 0; /* 0 - Versus Player
                  1 - Easy
                  2 - Normal
                  3 - Hard */

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

// The number of the round
int roundNumber = 1;

// Points of both players
int p1Points = 0, p2Points = 0;

// The x offset of the round message
int xRound = 0;

// Leave game
bool quit = false;

// Levels of game
bool players2 = true; // Default 2 players
bool easy = false;
bool normal = false;
bool hard = false;

// Control of the palette collision in corner
bool cornerPalette = false;

// Controls of direction of ball's movements
bool changeHorizontalMovement = false;
bool changeVerticalMovement = false;

// Control of the match
bool endOfMatch = false;
// Opening
bool openingGame = true;
// Main menu
bool mainMenuOption = false;
// Instructions of game
bool instructions_of_game = false;
// Level of game
bool level_of_game = false;
// Show round message
bool showRound = false;

bool playerWin = false;

// Variations in ball's velocity
float deltaVelX = 1;
float deltaVelY = 1;

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

    // adjusts the palette's velocity based in ball's position ( Artificial Intelligence )
    void handle_input( int cx, int cy );

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

    void easyPalette( int cx, int cy );

    void normalPalette( int cx, int cy );

    void hardPalette( int cx, int cy );
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

    // Get the x offset of ball
    int get_x_ball();

    // Get the y offset of ball
    int get_y_ball();

    // Get the horizontal velocity of ball
    float get_xVel_ball();

    // Get the vertical velocity of ball
    float get_yVel_Ball();
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
    int start, quit, instructions, versusCPU;
    int easyMode, normalMode, hardMode;

    // The frame rate regulator of main menu
    //Timer fps;

    public:

    void load_items();

    void show();

    int get_start_state();

    int get_quit_state();

    int get_instructions_state();

    int get_versusCPU_state();

    int get_easyMode_state();

    int get_normalMode_state();

    int get_hardMode_state();

    void set_start_state( int state );

    void set_quit_state( int state );

    void set_instructions_state( int state );

    void set_versusCPU_state( int state );

    void set_easyMode_state( int state );

    void set_normalMode_state( int state );

    void set_hardMode_state( int state );
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

    void set_versusCPU_state( int state );

    void set_easyMode_state( int state );

    void set_normalMode_state( int state );

    void set_hardMode_state( int state );
};

class Kipong
{
    private:
    // Ball of the game KiPong
    Ball ball;

    // Create the walls
    Walls walls;

    // Create the palettes
    Palette *lPalette;
    Palette *rPalette;

    // Palette movement's timers
    Timer deltaBall;
    Timer deltaPalette;

    public:

    Kipong();

    void start();

    void handle_events();

    void show();

    void match_result();

    void show_credits();

    // Handle with artificial intelligence
    void handle_AI();
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
    SDL_FreeSurface( versusPlayer_selected );
    SDL_FreeSurface( versusPlayer_unselected );
    SDL_FreeSurface( quitGame_selected );
    SDL_FreeSurface( quitGame_unselected );
    SDL_FreeSurface( instructions_selected );
    SDL_FreeSurface( instructions_unselected );
    SDL_FreeSurface( instructionsSurface );
    SDL_FreeSurface( versusCPU_selected );
    SDL_FreeSurface( versusCPU_unselected );

    SDL_FreeSurface( roundMatch );
    SDL_FreeSurface( round1 );
    SDL_FreeSurface( round2 );
    SDL_FreeSurface( round3 );
    SDL_FreeSurface( round4 );
    SDL_FreeSurface( round5 );
    SDL_FreeSurface( round6 );
    SDL_FreeSurface( round7 );
    SDL_FreeSurface( round8 );
    SDL_FreeSurface( round9 );

    // Release the players messages and credits
    SDL_FreeSurface( credits );
    SDL_FreeSurface( player1 );
    SDL_FreeSurface( player2 );
    SDL_FreeSurface( win );

    // Free the levels
    SDL_FreeSurface( easyMode_selected );
    SDL_FreeSurface( easyMode_unselected );
    SDL_FreeSurface( normalMode_selected );
    SDL_FreeSurface( normalMode_unselected );
    SDL_FreeSurface( hardMode_selected );
    SDL_FreeSurface( hardMode_unselected );

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

