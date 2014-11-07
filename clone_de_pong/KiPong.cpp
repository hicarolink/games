/*
  11/11/2011
  Kipong by Kiddo
*/

// Headers
/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */
#include "Utils.h"
#include <stdio.h>

/* -----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------- */

int main( int argc, char* argv[] )
{
    // The game class
    Kipong kipong;

    // The Kipong's opening
    Opening OpenKipong;

    if( init() == false )
    {
        return 1;
    }

    if( load_files() == false )
    {
        return 1;
    }

    if( openFonts() == false )
    {
        return 1;
    }

     // Makes the ball start at a radom direction
    srand((unsigned)time(0));

    while( quit == false )
    {
        if( openingGame == true )
        {
            //If there is no music playing
            if( Mix_PlayingMusic() == 0 )
            {
                //Play the opening music
                if( Mix_PlayMusic( musicOpening, -1 ) == -1 )
                {
                    return 1;
                }
            }

            // Player already choose the starting simbol
            while( SDL_PollEvent( &event ) )
            {
                OpenKipong.handle_events();

                if( event.type == SDL_QUIT )
                {
                    quit = true;
                }
            }

            OpenKipong.apply_background();
        }
        else
        {
            //If there is no music playing
            if( Mix_PlayingMusic() == 0 )
            {
                //Play the Kipong music
                if( Mix_PlayMusic( musicKipong, -1 ) == -1 )
                {
                    return 1;
                }
            }

            kipong.start();

            // Player already choose the starting simbol
            while( SDL_PollEvent( &event ) )
            {
                kipong.handle_events();

                if( event.type == SDL_QUIT )
                {
                    quit = true;
                }
            }

            kipong.handle_AI();

            // Fill the screen black
            SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 200, 0x00 ) );

            kipong.show();

            kipong.match_result();
        }

        // Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    kipong.show_credits();

    // Clean up
    clean_up();

    return 0;
}

