/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

#include "Classes.h"
#include "Constants.cpp"

#include <cmath>
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>

//File stream for the game log.
std::ofstream logger( "log.txt" );

void log( std::string file, std::string message = "" )
{
    //Write message to file
    logger << file << ":" << message << std::endl;
    //Flush the buffer
    logger.flush();
}

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

// Return a text message
SDL_Surface* load_text( std::string filename, TTF_Font* font, SDL_Color color )
{
    // The message that will be used
    SDL_Surface* message = NULL;

    // Setting the font, text and color
    message = TTF_RenderText_Solid( font, filename.c_str(), color );

    // Retrieve the message
    return message;
}

// Return a font
TTF_Font* load_font( std::string filename, int size )
{
    // The font that will be used
    TTF_Font* font = NULL;

    // Setting the name and size of font
    font = TTF_OpenFont( filename.c_str(), size );

    // Retrieve the font
    return font;

}

SDL_Surface *load_image( std::string filename, int r = 0, int g = 0xFF, int b = 0xFF )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, r, g, b );

            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool load_files()
{
    // Load ball image with transparent black background
    ball = load_image( "Images/ball.png", 0, 0, 0 );

    // Load the left palette
    palette = load_image( "Images/palette.png" );

    // Open the fonts
    //fontKiddo = TTF_OpenFont( "Fonts/kiddo.ttf", 48 );
    fontKiddo = load_font( "Fonts/kiddo.ttf", 48 );
    //fontOpen = TTF_OpenFont( "Fonts/open.ttf", 24 );
    fontOpen = load_font( "Fonts/open.ttf", 24 );
    //fontTitle = TTF_OpenFont( "Fonts/title.ttf", 90 );
    fontTitle = load_font( "Fonts/title.ttf", 90 );
    fontMenu = load_font( "Fonts/kiddo.ttf", 30 );

    // Load the background
    background = load_image( "Images/background.png" );

    // Load the instructions
    instructionsSurface = load_image( "Images/instructions.png" );

    if( fontKiddo == NULL || fontOpen == NULL || fontTitle == NULL || background == NULL || instructionsSurface == NULL )
    {
        return false;
    }

    // If there was a problem in loading the ball
    if( ball == NULL || palette == NULL )
    {
        return false;
    }

    // Load the number of a match
    round0 = load_image( "Images/round0.png", 0, 0, 0 );
    round1 = load_image( "Images/round1.png", 0, 0, 0 );
    round2 = load_image( "Images/round2.png", 0, 0, 0 );
    round3 = load_image( "Images/round3.png", 0, 0, 0 );
    round4 = load_image( "Images/round4.png", 0, 0, 0 );
    round5 = load_image( "Images/round5.png", 0, 0, 0 );
    round6 = load_image( "Images/round6.png", 0, 0, 0 );
    round7 = load_image( "Images/round7.png", 0, 0, 0 );
    round8 = load_image( "Images/round8.png", 0, 0, 0 );
    round9 = load_image( "Images/round9.png", 0, 0, 0 );

    if( round1 == NULL || round2 == NULL || round3 == NULL || round4 == NULL || round5 == NULL ||
        round6 == NULL || round7 == NULL || round8 == NULL || round9 == NULL )
    {
        return false;
    }

    // Load the credits
    credits = load_image( "Images/ending.png" );

    if( credits == NULL )
    {
        return false;
    }

    // Load the musics
    musicKipong = Mix_LoadMUS( "Music/kipong.mp3" );
    musicOpening = Mix_LoadMUS( "Music/opening.mp3" );

    // If there was a problem in loading the musics
    if( musicKipong == NULL || musicOpening == NULL )
    {
        return false;
    }

    // Load the sound effects
    ballbounce = Mix_LoadWAV( "SE/ballbounce.wav" );
    ballhit = Mix_LoadWAV( "SE/ballhit.wav" );
    enter = Mix_LoadWAV( "SE/enter.wav" );

    // If there was a problem in loading the sound effects
    if( ballbounce == NULL || ballhit == NULL || enter == NULL )
    {
        return false;
    }

    return true;
}

void apply_opening_texts()
{
    apply_surface( KIDDO_1_X, KIDDO_1_Y, messageKiddo1, screen );
    apply_surface( KIDDO_2_X, KIDDO_2_Y, messageSoft2, screen );
    apply_surface( 75, 530, messageOpen, screen );
}

bool openFonts()
{
    messageKiddo1 = load_text( "Hicaro", fontKiddo, textKiddoColor );
    messageSoft2 = load_text( "Santos", fontKiddo, textSoftColor );
    messageOpen = load_text( "Trabalho Final De Graduacao. UNIFEI, 2013.", fontOpen, textOpenColor );
    messageTitle = load_text( "PongCCO", fontTitle, textTitleColor );
    pressEnter = load_text( "Pressione Enter", fontKiddo, textOpenColor );
    roundMatch = load_text( "Partida", fontKiddo, textSoftColor );

    // Main Menu
    versusPlayer_selected = load_text( "Dois Jogadores", fontMenu, textSoftColor );
    quitGame_selected = load_text( "Sair", fontMenu, textSoftColor );
    instructions_selected = load_text( "Instrucoes", fontMenu, textSoftColor );
    versusCPU_selected = load_text( "Versus Maquina", fontMenu, textSoftColor );
    easyMode_selected = load_text( "Modo Facil", fontMenu, textSoftColor );
    normalMode_selected = load_text( "Modo Normal", fontMenu, textSoftColor );
    hardMode_selected = load_text( "Modo Dificil", fontMenu, textSoftColor );

    versusPlayer_unselected = load_text( "Dois Jogadores", fontMenu, textOpenColor );
    quitGame_unselected = load_text( "Sair", fontMenu, textOpenColor );
    instructions_unselected = load_text( "Instrucoes", fontMenu, textOpenColor );
    versusCPU_unselected = load_text( "Versus Maquina", fontMenu, textOpenColor );
    easyMode_unselected = load_text( "Modo Facil", fontMenu, textOpenColor );
    normalMode_unselected = load_text( "Modo Normal", fontMenu, textOpenColor );
    hardMode_unselected = load_text( "Modo Dificil", fontMenu, textOpenColor );

    if( messageKiddo1 == NULL || messageSoft2 == NULL || messageOpen == NULL || messageTitle == NULL ||
        pressEnter == NULL || versusPlayer_selected == NULL || versusPlayer_unselected == NULL ||
        quitGame_selected == NULL || quitGame_unselected == NULL || instructions_selected == NULL ||
        instructions_unselected == NULL || roundMatch == NULL || versusCPU_selected == NULL ||
        versusCPU_unselected == NULL || hardMode_selected == NULL || hardMode_unselected == NULL )
    {
        return false;
    }

    // Load the players and win message
    player1 = load_text( "Jogador 1", fontKiddo, textKiddoColor );
    player2 = load_text( "Jogador 2", fontKiddo, textKiddoColor );
    win = load_text( "Venceu!", fontTitle, textSoftColor );

    if( player1 == NULL || player2 == NULL || win == NULL )
    {
        return false;
    }

    return true;
}

bool init()
{
    // Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    // Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    // If there was an error setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    // Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    // Set the Window Caption
    SDL_WM_SetCaption( "PongCCO 1.0 by Hicaro Santos", NULL );

    return true;
}

// Setting walls
void set_walls_of_field()
{
    middleWall.x = 397;
    middleWall.y = 0;
    middleWall.w = 6;
    middleWall.h = WALL_VERTICAL_HEIGHT;

    leftMark.x = 200;
    leftMark.y = 297;
    leftMark.w = 6;
    leftMark.h = 6;

    rightMark.x = 594;
    rightMark.y = 297;
    rightMark.w = 6;
    rightMark.h = 6;
}

// Ball's velocity
void adjustDeltaVelocity()
{
    // If X's velocity variation makes the ball too slow
    if( deltaVelX < 1 )
    {
        deltaVelX = 1;
    }
    // If X's velocity variation makes the ball too fast
    else if( deltaVelX > 8 )
    {
        deltaVelX = 8;
    }

    // If Y's velocity variation makes the ball too slow
    if( deltaVelY < 1 )
    {
        deltaVelY = 1;
    }
    // If Y's velocity variation makes the ball too fast
    else if( deltaVelY > 8 )
    {
        deltaVelY = 8;
    }
}

void speedDropX()
{
    deltaVelX -= BALL_SPEED_DROP_X;
}

void speedUpX()
{
    deltaVelX += BALL_SPEED_UP_X;
}

void speedBoostX()
{
    deltaVelX += BALL_SPEED_BOOST_X;
}

void speedDropY()
{
    deltaVelY -= BALL_SPEED_DROP_Y;
}

void speedUpY()
{
    deltaVelY += BALL_SPEED_UP_Y;
}

void speedBoostY()
{
    deltaVelY += BALL_SPEED_BOOST_Y;
}

double distance( float x1, float x2, float y1, float y2 )
{
    // Return the distance between two points
    return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}

bool check_colisions_palette_ball( Box box, Circle &A )
{
    // Closest point the B boxes
    float cX, cY;

    // Find the closes x offset
    if( A.x < box.x )
    {
        cX = box.x;
    }
    else if( A.x > box.x + box.w )
    {
        cX = box.x + box.w;
    }
    else
    {
        cX = A.x;
    }

    // Find the closest y offset
    if( A.y < box.y )
    {
        cY = box.y;
    }
    else if( A.y > box.y + box.h )
    {
        cY = box.y + box.h;
    }
    else
    {
        cY = A.y;
    }

    // If the closes point is inside the circle
    if( distance( A.x, cX, A.y, cY) < A.r )
    {
        // Up the ball's velocity
        speedUpX();

        return true;
    }

    return false;
}

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

bool check_collisions_palette( Circle &A, Box box )
{
    // Closest point the B boxes
    float cX, cY;

    // Find the closes x offset
    if( A.x < box.x )
    {
        cX = box.x;
    }
    else if( A.x > box.x + box.w )
    {
        cX = box.x + box.w;
    }
    else
    {
        cX = A.x;
    }

    // Find the closest y offset
    if( A.y < box.y )
    {
        cY = box.y;
    }
    else if( A.y > box.y + box.h )
    {
        cY = box.y + box.h;
    }
    else
    {
        cY = A.y;
    }

    // If the closes point is inside the circle
    if( distance( A.x, cX, A.y, cY) < A.r )
    {
        // If vertical collision and horizontal collision ( corner of palette )
        if( ( A.y < cY || A.y > cY ) && ( A.x < cX || A.x > cX ) )
        {
            if( cornerPalette == false )
            {
                if( ( A.x + A.r > cX && A.x + A.r < cX + box.w ) || ( A.x - A.r < cX && A.x - A.r > cX - box.w ) )
                {
                    changeVerticalMovement = !changeVerticalMovement;

                    // Up the ball's speed
                    speedBoostY();
                    speedUpY();
                }
                changeHorizontalMovement = !changeHorizontalMovement;

                // Up the ball's speed
                //speedUpX();

                cornerPalette = true;
            }
        }
        // If only vertical collision
        else if( ( A.y < cY ) || ( A.y > cY ) )
        {
            changeVerticalMovement = !changeVerticalMovement;

            // Up the ball's speed
            speedUpY();
            speedUpY();
        }
        // Vertical collision
        else
        {
            changeHorizontalMovement = !changeHorizontalMovement;

            if( A.y <= box.y + box.h / 5 )
            {
                speedBoostY();
                //speedUpX();
            }
            else if( A.y <= box.y + 2 * ( box.h / 5 ) )
            {
                speedUpY();
                //speedUpX();
            }
            else if( A.y <= box.y + 3 * ( box.h / 5 ) )
            {
                // Up the ball's speed
                speedUpX();
                speedUpY();
                //speedUpX();
            }
            else if( A.y <= box.y + 4 * ( box.h / 5 ) )
            {
                speedUpY();
                //speedUpX();
            }
            else
            {
                speedUpY();
                //speedUpX();
            }
        }

        // Player the ball bounce
        Mix_PlayChannel( -1, ballbounce, 0 );

        return true;
    }

    return false;
}

bool check_colisions_palette_walls( Box box, std::vector<SDL_Rect> &B )
{
    // The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

    // Calculates the side of rect A
    leftA = box.x;
    rightA = box.x + box.w;
    topA = box.y;
    bottomA = box.y + box.h;

    // Go through rect B
    for( unsigned int Bbox = 0; Bbox < B.size(); Bbox ++ )
    {
        // Calculate the sides of rect B
        leftB = B[ Bbox ].x;
        rightB = B[ Bbox ].x + B[ Bbox ].w;
        topB = B[ Bbox ].y;
        bottomB = B[ Bbox ].y + B[ Bbox ].h;

        // If no sides of A are outside of B
        if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
        {
            return true;
        }
    }

    return false;
}

bool check_colisions( Circle &A, std::vector<SDL_Rect> &B )
{
    // Closest point the B boxes
    float cX, cY;

    // Go through the B boxes
    for( unsigned int Bbox = 0; Bbox < B.size(); Bbox++ )
    {
        // Find the closes x offset
        if( A.x < B[ Bbox ].x )
        {
            cX = B[ Bbox ].x;
        }
        else if( A.x > B[ Bbox ].x + B[ Bbox ].w )
        {
            cX = B[ Bbox ].x + B[ Bbox ].w;
        }
        else
        {
            cX = A.x;
        }

        // Find the closest y offset
        if( A.y < B[ Bbox ].y )
        {
            cY = B[ Bbox ].y;
        }
        else if( A.y > B[ Bbox ].y + B[ Bbox ].h )
        {
            cY = B[ Bbox ].y + B[ Bbox ].h;
        }
        else
        {
            cY = A.y;
        }

        // If the closes point is inside the circle
        if( distance( A.x, cX, A.y, cY) < A.r )
        {
            // If vertical collision and horizontal collision
            if( ( A.y < cY || A.y > cY ) && ( A.x < cX || A.x > cX ) )
            {
                changeVerticalMovement = !changeVerticalMovement;
                changeHorizontalMovement = !changeHorizontalMovement;

                // Drop the ball's speed
                speedDropY();
                speedDropX();
            }
            // If only vertical collision
            else if( ( A.y < cY || A.y > cY ) )
            {
                changeVerticalMovement = !changeVerticalMovement;
            }
            // Horizontal collision
            else
            {
                changeHorizontalMovement = !changeHorizontalMovement;
                speedDropX();
            }

            // Player the ball bounce
            Mix_PlayChannel( -1, ballhit, 0 );

            return true;
        }
    }

    return false;
}

void Walls::set_walls()
{
    topWall.x = 0;
    topWall.y = 0;
    topWall.w = WALL_HORIZONTAL_WIDTH;
    topWall.h = WALL_HORIZONTAL_HEIGHT;

    bottomWall.x = 0;
    bottomWall.y = SCREEN_HEIGHT - WALL_HORIZONTAL_HEIGHT;
    bottomWall.w = WALL_HORIZONTAL_WIDTH;
    bottomWall.h = WALL_HORIZONTAL_HEIGHT;
}

void Walls::show_walls()
{
    // A wall to mark the middle
    SDL_FillRect( screen, &middleWall, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    SDL_FillRect( screen, &leftMark, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    SDL_FillRect( screen, &rightMark, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    for( unsigned int i = 0; i < box.size(); i++ )
    {
        // Fill walls white
        SDL_FillRect( screen, &box.at(i), SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    }
}

void Walls::put_walls_in_box()
{
    box.push_back( topWall );
    box.push_back( bottomWall );
}

std::vector<SDL_Rect> Walls::get_box()
{
    return box;
}

Ball::Ball()
{
    c.r = BALL_WIDTH / 2;

    // Initialize the velocity
    xVel = 0;
    yVel = 0;
}

float Ball::get_xVel_ball()
{
    return xVel;
}

float Ball::get_yVel_Ball()
{
    return yVel;
}

int Ball::get_x_ball()
{
    return c.x;
}

int Ball::get_y_ball()
{
    return c.y;
}

Ball::Ball( int x, int y )
{
    // Initialize the offsets and dimensions
    c.x = x;
    c.y = y;
    c.r = BALL_WIDTH / 2;

    // Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Ball::set_position( int x, int y )
{
    c.x = x;
    c.y = y;
}

void Ball::changeMovement()
{
    if( changeHorizontalMovement == true )
    {
         xVel *= -1;
         changeHorizontalMovement = false;
    }

    if( changeVerticalMovement == true )
    {
        yVel *= -1;
        changeVerticalMovement = false;
    }
}

void Ball::move( Uint32 deltaTicks, std::vector<SDL_Rect> rectsWal, Box leftBox, Box rightBox, float delta_vel_x = 1, float delta_vel_y = 1 )
{
    // If there is no collision with palette corner
    if( cornerPalette == false )
    {
        // Move the ball left or right
        c.x += ( xVel * delta_vel_x ) * ( deltaTicks / 1000.f );
    }
    else
    {
        cornerPalette = false;
    }

    // If a point was made, end of match
    if( ( c.x + BALL_WIDTH / 2 < 0 ) || ( c.x - BALL_WIDTH / 2 > SCREEN_WIDTH ) )
    {
        endOfMatch = true;

        if( c.x + BALL_WIDTH / 2 < 0 )
        {
            p2Points++;
            if( p2Points == 5 )
            {
                playerWin = true;
            }
        }
        else
        {
            p1Points++;
            if( p1Points == 5 )
            {
                playerWin = true;
            }
        }

        // Increase the round number
        roundNumber = roundNumber + 1;

        return;
    }

    // If the ball went too far to the left or right or has collided with the other shapes
    if( ( check_colisions( c, rectsWal ) ) || ( check_collisions_palette( c, leftBox ) ) || ( check_collisions_palette( c, rightBox ) ) )
    {
        // Move back
        c.x -= ( xVel * delta_vel_x ) * ( deltaTicks / 1000.f );
    }

    // Move the ball up or down
    c.y += ( yVel * delta_vel_y ) * ( deltaTicks / 1000.f );

    // If the ball went too far to up or down or has collided with the other shapes
    if( ( check_colisions( c, rectsWal ) ) || ( check_collisions_palette(  c, leftBox ) ) || ( check_collisions_palette( c, rightBox ) ) )
    {
       // Move back
       c.y -= ( yVel * delta_vel_y ) * ( deltaTicks / 1000.f );
    }
}

Circle Ball::get_circle()
{
    return c;
}

void Ball::show()
{
    // Show the ball
    apply_surface( c.x - c.r, c.y - c.r, ball, screen );
}

void Ball::start()
{
    // Reset delta velocity
    deltaVelX = 1;
    deltaVelY = 1;

    int random_start_x = rand();
    int random_start_y = rand();

    if( random_start_x % 2 == 0 )
    {
        xVel = BALL_VEL_X;
    }
    else
    {
        xVel = -BALL_VEL_X;
    }

    if( random_start_y % 2 == 0 )
    {
        yVel = BALL_VEL_Y;
    }
    else
    {
        yVel = -BALL_VEL_Y;
    }
}

Palette::Palette( float x, float y, int playerControl )
{
    box.x = x;
    box.y = y;
    box.w = PALETTE_WIDHT;
    box.h = PALETTE_HEIGHT;

    player = playerControl;

    // Initialize the velocity
    yVel = 0;
    xVel = 0;
}

void Palette::move( Uint32 deltaTicks, std::vector<SDL_Rect> rectsWall, Circle c )
{
    // Move the palette left or right
    box.x += xVel * 2 * ( deltaTicks / 1000.f );

    if(check_colisions_palette_ball( get_box(), c ) )
    {
        box.x -= xVel * 2 * ( deltaTicks / 1000.f );
    }

    if( player == PALETTE_PLAYER1 )
    {
        if( box.x + box.w > startPosition.x + PALETTE_LIMIT_HORIZONTAL )
        {
            box.x = startPosition.x + PALETTE_LIMIT_HORIZONTAL - box.w;
        }
        else if( box.x < startPosition.x - PALETTE_LIMIT_HORIZONTAL / 2 )
        {
            box.x = startPosition.x - PALETTE_LIMIT_HORIZONTAL / 2;
        }
    }
    if( player == PALETTE_PLAYER2 )
    {
        if( box.x + box.w > startPosition.x + PALETTE_LIMIT_HORIZONTAL / 2 )
        {
            box.x = startPosition.x + PALETTE_LIMIT_HORIZONTAL / 2 - box.w;
        }
        else if( box.x < startPosition.x - PALETTE_LIMIT_HORIZONTAL )
        {
            box.x = startPosition.x - PALETTE_LIMIT_HORIZONTAL;
        }
    }

    // Move the palette up or down
    box.y += yVel * ( deltaTicks / 1000.f );

    if( box.y < 0 || box.y + box.h > SCREEN_HEIGHT || check_colisions_palette_walls( get_box(), rectsWall ) || check_colisions_palette_ball( get_box(), c ) )
    {
        // Move back
        box.y -= yVel * ( deltaTicks / 1000.f );
    }
}

void Palette::start( int positionX, int positionY, int playerControl )
{
    box.x = positionX;
    box.y = positionY;

    // Velocity of palette set up to zero
    xVel = yVel = 0;

    player = playerControl;

    startPosition.x = positionX;
}

void Palette::easyPalette( int cx, int cy )
{
    if( cy == ( box.y + PALETTE_HEIGHT  / 2 ) )
    {
        yVel = 0;
    }
    else
    {
        if( cy > box.y + PALETTE_HEIGHT )
        {
            yVel = 0;

            yVel += PALETTE_VEL;
        }
        else if( cy < box.y )
        {
            yVel = 0;

            yVel -= PALETTE_VEL;
        }
    }
}

void Palette::normalPalette( int cx, int cy )
{
    if( cy == ( box.y + PALETTE_HEIGHT  / 2 ) )
    {
        yVel = 0;
    }
    else
    {
        if( cy > ( box.y + PALETTE_HEIGHT  / 2 ) )
        {
            yVel = 0;

            yVel += PALETTE_VEL;
        }
        else if( cy < ( box.y + PALETTE_HEIGHT / 2 ) )
        {
            yVel = 0;

            yVel -= PALETTE_VEL;
        }
    }
}

void Palette::hardPalette( int cx, int cy )
{
    if( cy == ( box.y + PALETTE_HEIGHT  / 2 ) )
    {
        yVel = 0;
    }
    else
    {
        if( cy > ( box.y + PALETTE_HEIGHT  / 2 ) )
        {
            yVel = 0;

            yVel += PALETTE_VEL;
        }
        else if( cy < ( box.y + PALETTE_HEIGHT / 2 ) )
        {
            yVel = 0;

            yVel -= PALETTE_VEL;
        }
    }

    if( cx < SCREEN_WIDTH / 2 )
    {
        xVel = 0;
        xVel += PALETTE_VEL;
    }
    else if( cx > 600 )
    {
        xVel = 0;
        xVel -= PALETTE_VEL;
    }
}

void Palette::handle_input( int cx, int cy )
{
    // Easy
    if( level == 1 )
    {
        easyPalette( cx, cy );
    }
    // Normal
    else if( level == 2 )
    {
        normalPalette( cx, cy );
    }
    // Hard
    else if( level == 3 )
    {
        hardPalette( cx, cy );
    }
}

void Palette::handle_input()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);

    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        // Player 1?
        if( player == PALETTE_PLAYER1 )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_w: yVel -= PALETTE_VEL; break;
                case SDLK_s: yVel += PALETTE_VEL; break;
                case SDLK_d: xVel += PALETTE_VEL; break;
                case SDLK_a: xVel -= PALETTE_VEL; break;
                // Return to title
                case SDLK_ESCAPE: roundNumber = 1; // Restart the number of round
                                  p1Points = p2Points = 0; // Zero points for both players
                                  xRound = 0; // The x offset of round message
                                  openingGame = true;
                                  level_of_game = false;
                                  Mix_HaltMusic();
                                  break;
                default: break;
            }
        }
        // Player 2?
        else if( player == PALETTE_PLAYER2 )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_UP: yVel -= PALETTE_VEL; break;
                case SDLK_DOWN: yVel += PALETTE_VEL; break;
                case SDLK_LEFT: xVel -= PALETTE_VEL; break;
                case SDLK_RIGHT: xVel += PALETTE_VEL; break;
                // Return to title
                case SDLK_ESCAPE: roundNumber = 1; // Restart the number of round
                                  p1Points = p2Points = 0; // Zero points for both players
                                  xRound = 0; // The x offset of round message
                                  openingGame = true;
                                  level_of_game = false;
                                  Mix_HaltMusic();
                                  break;
                default: break;
            }
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        // Player 1?
        if( player == PALETTE_PLAYER1 )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_w:
                             if( yVel < 0 && !keystate[ SDLK_s ] )
                             {
                                 yVel = 0;
                             }
                             else if( yVel < 0 && keystate[ SDLK_s ] )
                             {
                                 yVel = 0;
                                 yVel += PALETTE_VEL;
                             }
                             else if( yVel == 0 && keystate[ SDLK_s ] )
                             {
                                 yVel += PALETTE_VEL;
                             }
                             break;
                case SDLK_s:
                             if( yVel > 0 && !keystate[ SDLK_w ] )
                             {
                                 yVel = 0;
                             }
                             else if( yVel > 0 && keystate[ SDLK_w ] )
                             {
                                 yVel = 0;
                                 yVel -= PALETTE_VEL;
                             }
                             else if( yVel == 0 && keystate[ SDLK_w ] )
                             {
                                 yVel -= PALETTE_VEL;
                             }
                             break;
                case SDLK_a:
                             if( xVel < 0 && !keystate[ SDLK_d ] )
                             {
                                 xVel = 0;
                             }
                             else if( xVel < 0 && keystate[ SDLK_d ] )
                             {
                                 xVel = 0;
                                 xVel += PALETTE_VEL;
                             }
                             else if( xVel == 0 && keystate[ SDLK_d ] )
                             {
                                 xVel += PALETTE_VEL;
                             }
                             break;
                case SDLK_d:
                             if( xVel > 0 && !keystate[ SDLK_a ] )
                             {
                                 xVel = 0;
                             }
                             else if( xVel > 0 && keystate[ SDLK_a ] )
                             {
                                 xVel = 0;
                                 xVel -= PALETTE_VEL;
                             }
                             else if( xVel == 0 && keystate[ SDLK_a ] )
                             {
                                 xVel -= PALETTE_VEL;
                             }
                             break;
                // Return to title
                case SDLK_ESCAPE: roundNumber = 1; // Restart the number of round
                                  p1Points = p2Points = 0; // Zero points for both players
                                  xRound = 0; // The x offset of round message
                                  openingGame = true;
                                  level_of_game = false;
                                  Mix_HaltMusic();
                                  break;
                default: break;
            }
        }
        // Player 2?
        else if( player == PALETTE_PLAYER2 )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_UP:
                             if( yVel < 0 && !keystate[ SDLK_DOWN ] )
                             {
                                 yVel = 0;
                             }
                             else if( yVel < 0 && keystate[ SDLK_DOWN ] )
                             {
                                 yVel = 0;
                                 yVel += PALETTE_VEL;
                             }
                             else if( yVel == 0 && keystate[ SDLK_DOWN ] )
                             {
                                 yVel += PALETTE_VEL;
                             }
                             break;
                case SDLK_DOWN:
                             if( yVel > 0 && !keystate[ SDLK_UP ] )
                             {
                                 yVel = 0;
                             }
                             else if( yVel > 0 && keystate[ SDLK_UP ] )
                             {
                                 yVel = 0;
                                 yVel -= PALETTE_VEL;
                             }
                             else if( yVel == 0 && keystate[ SDLK_UP ] )
                             {
                                 yVel -= PALETTE_VEL;
                             }
                             break;
                case SDLK_LEFT:
                             if( xVel < 0 && !keystate[ SDLK_RIGHT ] )
                             {
                                 xVel = 0;
                             }
                             else if( xVel < 0 && keystate[ SDLK_RIGHT ] )
                             {
                                 xVel = 0;
                                 xVel += PALETTE_VEL;
                             }
                             else if( xVel == 0 && keystate[ SDLK_RIGHT ] )
                             {
                                 xVel += PALETTE_VEL;
                             }
                             break;
                case SDLK_RIGHT:
                             if( xVel > 0 && !keystate[ SDLK_LEFT ] )
                             {
                                 xVel = 0;
                             }
                             else if( xVel > 0 && keystate[ SDLK_LEFT ] )
                             {
                                 xVel = 0;
                                 xVel -= PALETTE_VEL;
                             }
                             else if( xVel == 0 && keystate[ SDLK_LEFT ] )
                             {
                                 xVel -= PALETTE_VEL;
                             }
                             break;
                // Return to title
                case SDLK_ESCAPE: roundNumber = 1; // Restart the number of round
                                  p1Points = p2Points = 0; // Zero points for both players
                                  xRound = 0; // The x offset of round message
                                  openingGame = true;
                                  level_of_game = false;
                                  Mix_HaltMusic();
                                  break;
                default: break;
            }
        }
    }
}

void Palette::show()
{
    // Fill palette white
    apply_surface( box.x, box.y, palette, screen );
}

Box Palette::get_box()
{
    return box;
}

Opening::Opening()
{
    main.load_items();

    bgX = bgY = 0;

    alpha = SDL_ALPHA_TRANSPARENT;

    alpha_press_enter = SDL_ALPHA_TRANSPARENT;

    twinkle = false;

    twink_state = TWINKLE_FADE_IN;
}

void Kipong::show_credits()
{
    apply_surface( 0, 0, credits, screen );

    if( SDL_Flip( screen ) == -1 )
    {
        return;
    }

    SDL_Delay( 3000 );
}

void Opening::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        if( mainMenuOption == false )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_RETURN: mainMenuOption = true;
                                  Mix_PlayChannel( -1, enter, 0 );

                                  break;
                case SDLK_ESCAPE: quit = true; break;
                default: break;
            }
        }
        else if( level_of_game == true )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_RETURN: // Player the enter sound
                                  Mix_PlayChannel( -1, enter, 0 );

                                  if( main.get_easyMode_state() == EASYMODE_SELECTED )
                                  {
                                      level = 1;
                                  }
                                  else if( main.get_normalMode_state() == NORMALMODE_SELECTED )
                                  {
                                      level = 2;
                                  }
                                  else if( main.get_hardMode_state() == HARDMODE_SELECTED )
                                  {
                                      level = 3;
                                  }
                                  // Stop the opening music
                                  Mix_HaltMusic();
                                  openingGame = false;

                                  roundNumber = 1;

                                  endOfMatch = true;

                                  break;
                case SDLK_UP: if( main.get_easyMode_state() == EASYMODE_SELECTED )
                              {
                                  set_easyMode_state( EASYMODE_UNSELECTED );
                                  set_normalMode_state( NORMALMODE_UNSELECTED );
                                  set_hardMode_state( HARDMODE_SELECTED );
                              }
                              else if( main.get_normalMode_state() == NORMALMODE_SELECTED )
                              {
                                  set_easyMode_state( EASYMODE_SELECTED );
                                  set_normalMode_state( NORMALMODE_UNSELECTED );
                                  set_hardMode_state( HARDMODE_UNSELECTED );
                              }
                              else if( main.get_hardMode_state() == HARDMODE_SELECTED )
                              {
                                  set_easyMode_state( EASYMODE_UNSELECTED );
                                  set_normalMode_state( NORMALMODE_SELECTED );
                                  set_hardMode_state( HARDMODE_UNSELECTED );
                              }
                              break;
                case SDLK_DOWN: if( main.get_easyMode_state() == EASYMODE_SELECTED )
                                {
                                    set_easyMode_state( EASYMODE_UNSELECTED );
                                    set_normalMode_state( NORMALMODE_SELECTED );
                                    set_hardMode_state( HARDMODE_UNSELECTED );
                                }
                                else if( main.get_normalMode_state() == NORMALMODE_SELECTED )
                                {
                                    set_easyMode_state( EASYMODE_UNSELECTED );
                                    set_normalMode_state( NORMALMODE_UNSELECTED );
                                    set_hardMode_state( HARDMODE_SELECTED );
                                }
                                else if( main.get_hardMode_state() == HARDMODE_SELECTED )
                                {
                                    set_easyMode_state( EASYMODE_SELECTED );
                                    set_normalMode_state( NORMALMODE_UNSELECTED );
                                    set_hardMode_state( HARDMODE_UNSELECTED );
                                }
                                break;
                case SDLK_ESCAPE: level_of_game = false; break;
                default: break;
            }
        }
        else
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_RETURN: // Player the enter sound
                                  Mix_PlayChannel( -1, enter, 0 );

                                  if( main.get_start_state() == START_SELECTED )
                                  {
                                       // Versus Player
                                       level = 0;

                                       // Stop the opening music
                                       Mix_HaltMusic();
                                       openingGame = false;

                                       roundNumber = 1;

                                       endOfMatch = true;
                                  }
                                  else if( main.get_versusCPU_state() == VERSUSCPU_SELECTED )
                                  {
                                      level_of_game = true;
                                  }
                                  else if( main.get_instructions_state() == INSTRUCTIONS_SELECTED )
                                  {
                                      instructions_of_game = true;
                                      apply_surface( 0, 0, instructionsSurface, screen );
                                  }
                                  else if( main.get_quit_state() == QUIT_SELECTED )
                                  {
                                      quit = true;
                                  }

                                  break;
                case SDLK_UP: if( main.get_start_state() == START_SELECTED )
                              {
                                  set_quit_state( QUIT_SELECTED );
                                  set_start_state( START_UNSELECTED );
                                  set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                  set_versusCPU_state( VERSUSCPU_UNSELECTED );
                              }
                              else if( main.get_versusCPU_state() == VERSUSCPU_SELECTED )
                              {
                                  set_quit_state( QUIT_UNSELECTED );
                                  set_start_state( START_SELECTED );
                                  set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                  set_versusCPU_state( VERSUSCPU_UNSELECTED );
                              }
                              else if( main.get_instructions_state() == INSTRUCTIONS_SELECTED )
                              {
                                  set_quit_state( QUIT_UNSELECTED );
                                  set_start_state( START_UNSELECTED );
                                  set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                  set_versusCPU_state( VERSUSCPU_SELECTED );
                              }
                              else if( main.get_quit_state() == QUIT_SELECTED )
                              {
                                  set_quit_state( QUIT_UNSELECTED );
                                  set_start_state( START_UNSELECTED );
                                  set_instructions_state( INSTRUCTIONS_SELECTED );
                                  set_versusCPU_state( VERSUSCPU_UNSELECTED );
                              }

                              break;
                case SDLK_DOWN: if( main.get_start_state() == START_SELECTED )
                                {
                                    set_quit_state( QUIT_UNSELECTED );
                                    set_start_state( START_UNSELECTED );
                                    set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                    set_versusCPU_state( VERSUSCPU_SELECTED );
                                }
                                else if( main.get_versusCPU_state() == VERSUSCPU_SELECTED )
                                {
                                    set_quit_state( QUIT_UNSELECTED );
                                    set_start_state( START_UNSELECTED );
                                    set_instructions_state( INSTRUCTIONS_SELECTED );
                                    set_versusCPU_state( VERSUSCPU_UNSELECTED );
                                }
                                else if( main.get_instructions_state() == INSTRUCTIONS_SELECTED )
                                {
                                    set_quit_state( QUIT_SELECTED );
                                    set_start_state( START_UNSELECTED );
                                    set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                    set_versusCPU_state( VERSUSCPU_UNSELECTED );
                                }
                                else if( main.get_quit_state() == QUIT_SELECTED )
                                {
                                    set_quit_state( QUIT_UNSELECTED );
                                    set_start_state( START_SELECTED );
                                    set_instructions_state( INSTRUCTIONS_UNSELECTED );
                                    set_versusCPU_state( VERSUSCPU_UNSELECTED );
                                }
                                break;
                case SDLK_ESCAPE: roundNumber = 1; // Restart the number of round
                                  p1Points = p2Points = 0; // Zero points for both players
                                  if( main.get_instructions_state() == INSTRUCTIONS_SELECTED )
                                  {
                                      instructions_of_game = false;
                                  }
                                  else
                                  {
                                      mainMenuOption = false;
                                  }
                                  break;
                default: break;
            }
        }
    }


}

void Opening::set_start_state( int state )
{
    main.set_start_state( state );
}

void Opening::set_quit_state( int state )
{
    main.set_quit_state( state );
}

void Opening::set_instructions_state( int state )
{
    main.set_instructions_state( state );
}

void Opening::set_versusCPU_state( int state )
{
    main.set_versusCPU_state( state );
}

void Opening::set_easyMode_state( int state )
{
    main.set_easyMode_state( state );
}

void Opening::set_normalMode_state( int state )
{
    main.set_normalMode_state( state );
}

void Opening::set_hardMode_state( int state )
{
    main.set_hardMode_state( state );
}

void Opening::apply_background()
{
    if( mainMenuOption == true )
    {
        main.show();
    }
    else
    {
        // Start the frame rate
        fps.start();

        // Scroll background
        bgX -= 2;

        // If the background has gone too far
        if( bgX <= -background->w )
        {
            // Reset the offset
            bgX = 0;
        }

        // Show the background
        apply_surface( bgX, bgY, background, screen );
        apply_surface( bgX + background->w, bgY, background, screen );

        // Show the opening's texts
        apply_opening_texts();

        if( alpha < SDL_ALPHA_OPAQUE )
        {
            alpha += 5;
        }
        else
        {
            alpha = SDL_ALPHA_OPAQUE;

            twinkle = true;
        }

        SDL_SetAlpha( messageTitle, SDL_SRCALPHA, alpha);
        apply_surface( 170, 220, messageTitle, screen );

        // Twinkle the message of press enter
        if( twinkle == true )
        {
            // Apply the press enter message
            SDL_SetAlpha( pressEnter, SDL_SRCALPHA, alpha_press_enter );
            apply_surface( 265, 400, pressEnter, screen );

            if( twink_state == TWINKLE_FADE_OUT )
            {
                if( alpha_press_enter > SDL_ALPHA_TRANSPARENT )
                {
                    alpha_press_enter -= 10;

                    if( alpha_press_enter <= SDL_ALPHA_TRANSPARENT )
                    {
                        twink_state = TWINKLE_FADE_IN;
                        alpha_press_enter = SDL_ALPHA_TRANSPARENT;
                    }
                }
            }

            if( twink_state == TWINKLE_FADE_IN )
            {
                if( alpha_press_enter < SDL_ALPHA_OPAQUE )
                {
                    alpha_press_enter += 10;

                    if( alpha_press_enter >= SDL_ALPHA_OPAQUE )
                    {
                        alpha_press_enter = SDL_ALPHA_OPAQUE;
                        twink_state = TWINKLE_FADE_OUT;
                    }
                }
            }
        }

        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            // Sleep the remaining time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }
}

Kipong::Kipong()
{
    // Adjust the ball's velocity
    adjustDeltaVelocity();

    // set walls to mark the field
    set_walls_of_field();

    // Ball of the game KiPong
    ball.set_position( BALL_START_X_POSITION, BALL_START_Y_POSITION );

    ball.start();

    // Create the walls
    walls.set_walls();
    walls.put_walls_in_box();

    lPalette = new Palette( PALETTE_PLAYER1_START_X_POSITION, PALETTE_PLAYER1_START_Y_POSITION, PALETTE_PLAYER1 );
    rPalette = new Palette( PALETTE_PLAYER2_START_X_POSITION, PALETTE_PLAYER2_START_Y_POSITION, PALETTE_PLAYER2 );

    // Start ball's delta timer and palettes's delta timer
    deltaBall.start();
    deltaPalette.start();
}

void Kipong::handle_AI()
{
    if( level > 0 )
    {
        rPalette->handle_input( ball.get_x_ball(), ball.get_y_ball() );
    }
}

void Kipong::handle_events()
{
    lPalette->handle_input();

    // Multplayer?
    if( level == 0 )
    {
        rPalette->handle_input();
    }
}

void Kipong::start()
{
    if( endOfMatch == false )
    {
        // Move the palettes
        lPalette->move( deltaPalette.get_ticks(), walls.get_box(), ball.get_circle() );
        rPalette->move( deltaPalette.get_ticks(), walls.get_box(), ball.get_circle() );

        // Move the ball
        ball.move( deltaBall.get_ticks(), walls.get_box(), lPalette->get_box(), rPalette->get_box(), deltaVelX, deltaVelY );

        // Changes the ball's direction
        ball.changeMovement();

        // Restart delta timer
        deltaBall.start();
        deltaPalette.start();
    }
}

void Kipong::show()
{
    if( endOfMatch == false )
    {
        // Show walls
        walls.show_walls();

        // Show the ball
        ball.show();

        // Show palettes
        lPalette->show();
        rPalette->show();
    }
}

void apply_surface_round()
{
    switch( roundNumber )
    {
        case 1: apply_surface( xRound + 130, 290, round1, screen ); break;
        case 2: apply_surface( xRound + 130, 290, round2, screen ); break;
        case 3: apply_surface( xRound + 130, 290, round3, screen ); break;
        case 4: apply_surface( xRound + 130, 290, round4, screen ); break;
        case 5: apply_surface( xRound + 130, 290, round5, screen ); break;
        case 6: apply_surface( xRound + 130, 290, round6, screen ); break;
        case 7: apply_surface( xRound + 130, 290, round7, screen ); break;
        case 8: apply_surface( xRound + 130, 290, round8, screen ); break;
        case 9: apply_surface( xRound + 130, 290, round9, screen ); break;
        case 10: roundNumber = 1;
                 apply_surface( xRound + 130, 290, round1, screen );
                 break;
        default: break;
    }
}

void apply_surface_player_points( int points, int x )
{
    switch( points )
    {
        case 0: apply_surface( x + 160, 90, round0, screen ); break;
        case 1: apply_surface( x + 160, 90, round1, screen ); break;
        case 2: apply_surface( x + 160, 90, round2, screen ); break;
        case 3: apply_surface( x + 160, 90, round3, screen ); break;
        case 4: apply_surface( x + 160, 90, round4, screen ); break;
        case 5: break;
        default: break;
    }
}

void apply_player_surface( int numberPlayer, int playerPoints )
{
    if( numberPlayer == PLAYER1_NUMBER )
    {
        apply_surface( 100, 100, player1, screen );
        apply_surface_player_points( playerPoints, 100 );
    }
    else if( numberPlayer == PLAYER2_NUMBER )
    {
        apply_surface( 400, 100, player2, screen );
        apply_surface_player_points( playerPoints, 400 );
    }
}

void show_player_winner()
{
    if( p1Points == 5 )
    {
        apply_surface( 100, 300, player1, screen );
    }
    else if( p2Points == 5 )
    {
        apply_surface( 100, 300, player2, screen );
    }

    // Apply the win message
    apply_surface( 300, 300, win, screen );

    if( SDL_Flip( screen ) == -1 )
    {
        return;
    }

    p1Points = p2Points = 0;

    SDL_Delay( 3000 );
}

void Kipong::match_result()
{
    // If is the end of match
    if( endOfMatch == true )
    {
        // If one player won
        if( playerWin == true )
        {
            show_player_winner();

            Mix_HaltMusic();

            openingGame = true;
            level_of_game = false;

            playerWin = false;

            // Restart status of game
            xRound = 0;

            endOfMatch = false;
            SDL_Delay( 800 );

            // Restart delta timer
            deltaBall.start();
            deltaPalette.start();

            // Restart ball
            ball.start();
            ball.set_position( BALL_START_X_POSITION, BALL_START_Y_POSITION );
            // Restart palettes
            lPalette->start( PALETTE_PLAYER1_START_X_POSITION, PALETTE_PLAYER1_START_Y_POSITION, PALETTE_PLAYER1 );
            rPalette->start( PALETTE_PLAYER2_START_X_POSITION, PALETTE_PLAYER2_START_Y_POSITION, PALETTE_PLAYER2 );
        }
        // None player have win yet
        else
        {
            // Makes the round message move to right
            if( xRound < 300 )
            {
                Timer fps;

                fps.start();

                // Show the round message
                apply_surface( xRound, 300, roundMatch, screen );
                apply_surface_round();

                // Show the players's score
                apply_player_surface( PLAYER1_NUMBER, p1Points );
                apply_player_surface( PLAYER2_NUMBER, p2Points );

                if( xRound >= 300 )
                {
                    if( SDL_Flip( screen ) == -1 )
                    {
                        return;
                    }

                    SDL_Delay( 2000 );
                }

                xRound += 10;

                if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
                {
                    // Sleep the remaining time
                    SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
                }
            }
            else
            {
                xRound = 0;

                endOfMatch = false;
                SDL_Delay( 800 );

                // Restart delta timer
                deltaBall.start();
                deltaPalette.start();

                // Restart ball
                ball.start();
                ball.set_position( BALL_START_X_POSITION, BALL_START_Y_POSITION );
                // Restart palettes
                lPalette->start( PALETTE_PLAYER1_START_X_POSITION, PALETTE_PLAYER1_START_Y_POSITION, PALETTE_PLAYER1 );
                rPalette->start( PALETTE_PLAYER2_START_X_POSITION, PALETTE_PLAYER2_START_Y_POSITION, PALETTE_PLAYER2 );
            }
        }
    }
}

void MainMenu::show()
{
    if( level_of_game == true )
    {
        // Show the background
        apply_surface( 0, 0, background, screen );
        // Show the game's title
        SDL_SetAlpha( messageTitle, SDL_SRCALPHA, SDL_ALPHA_OPAQUE );
        apply_surface( 200, 220, messageTitle, screen );

        // Show the logo and Copyrights
        apply_opening_texts();

        // Easy Mode
        if( easyMode == EASYMODE_SELECTED )
        {
            apply_surface( 320, 330, easyMode_selected, screen );
        }
        else if( easyMode == EASYMODE_UNSELECTED )
        {
            apply_surface( 320, 330, easyMode_unselected, screen );
        }
        // Normal Mode
        if( normalMode == NORMALMODE_SELECTED )
        {
            apply_surface( 320, 375, normalMode_selected, screen );
        }
        else if( normalMode == NORMALMODE_UNSELECTED )
        {
            apply_surface( 320, 375, normalMode_unselected, screen );
        }

        // Hard Mode
        if( hardMode == HARDMODE_SELECTED )
        {
            apply_surface( 320, 420, hardMode_selected, screen );
        }
        else if( hardMode == HARDMODE_UNSELECTED )
        {
            apply_surface( 320, 420, hardMode_unselected, screen );
        }
    }
    else if( instructions_of_game == false )
    {
        // Show the background
        apply_surface( 0, 0, background, screen );
        // Show the game's title
        SDL_SetAlpha( messageTitle, SDL_SRCALPHA, SDL_ALPHA_OPAQUE );
        apply_surface( 200, 220, messageTitle, screen );

        // Show the logo and Copyrights
        apply_opening_texts();

        // Show the menu's options
        if( start == START_SELECTED )
        {
            apply_surface( 320, 330, versusPlayer_selected, screen );
        }
        else if( start == START_UNSELECTED )
        {
            apply_surface( 320, 330, versusPlayer_unselected, screen );
        }

        if( versusCPU == VERSUSCPU_SELECTED )
        {
            apply_surface( 325, 375, versusCPU_selected, screen );
        }
        else if( versusCPU == VERSUSCPU_UNSELECTED )
        {
            apply_surface( 325, 375, versusCPU_unselected, screen );
        }

        // Show the game's instructions
        if( instructions == INSTRUCTIONS_SELECTED )
        {
            apply_surface( 325, 410, instructions_selected, screen );
        }
        else
        {
            apply_surface( 325, 410, instructions_unselected, screen );
        }

        // Show the menu's options
        if( quit == QUIT_SELECTED )
        {
            apply_surface( 360, 450, quitGame_selected, screen );
        }
        else if( quit == QUIT_UNSELECTED )
        {
            apply_surface( 360, 450, quitGame_unselected, screen );
        }
    }
}

void MainMenu::load_items()
{
    start = START_SELECTED;
    quit = QUIT_UNSELECTED;
    instructions = INSTRUCTIONS_UNSELECTED;
    versusCPU = VERSUSCPU_UNSELECTED;

    easyMode = EASYMODE_SELECTED;
    normalMode = NORMALMODE_UNSELECTED;
    hardMode = HARDMODE_UNSELECTED;
}

int MainMenu::get_start_state()
{
    return start;
}

int MainMenu::get_quit_state()
{
    return quit;
}

int MainMenu::get_instructions_state()
{
    return instructions;
}

int MainMenu::get_versusCPU_state()
{
    return versusCPU;
}

int MainMenu::get_easyMode_state()
{
    return easyMode;
}

int MainMenu::get_normalMode_state()
{
    return normalMode;
}

int MainMenu::get_hardMode_state()
{
    return hardMode;
}

void MainMenu::set_start_state( int state )
{
    start = state;
}

void MainMenu::set_quit_state( int state )
{
    quit = state;
}

void MainMenu::set_instructions_state( int state )
{
    instructions = state;
}

void MainMenu::set_versusCPU_state( int state )
{
    versusCPU = state;
}

void MainMenu::set_easyMode_state( int state )
{
    easyMode = state;
}

void MainMenu::set_normalMode_state( int state )
{
    normalMode = state;
}

void MainMenu::set_hardMode_state( int state )
{
    hardMode = state;
}
