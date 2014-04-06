#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "rtsmini.h"

WINDOW *mainwin;

GameData game_data;

void win_startup( void )
{
  int offsety, offsetx;

  initscr( );
  cbreak( );
  noecho( );
  keypad( stdscr, TRUE );
  nodelay( stdscr, TRUE );

  offsetx = ( COLS - NCOLS ) / 2;
  offsety = ( LINES - NLINES ) / 2;

  mainwin = newwin( NLINES, NCOLS, offsety, offsetx );

  wborder( mainwin, 0, 0, 0, 0, 0, 0, 0, 0 );

  wrefresh( mainwin );

  return;
}


void win_shutdown( void )
{
  delwin( mainwin );

  endwin( );

  return;
}


void init_game( void )
{

  // Clear the game data to zero
  memset( (void *)&game_data, 0, sizeof( game_data ) );

  // Initialize the level data
  game_data.level.iteration = 1;
  game_data.level.state = PRE_GAME;
  game_data.level.delay = 1000000; // 10 seconds

  // Initialize the resources
  game_data.resources.food = INITIAL_FOOD;
  game_data.resources.wood = INITIAL_WOOD;
  game_data.resources.gold = INITIAL_GOLD;

  return;
}


int main( int argc, char *argv[] )
{
  unsigned long long curTime = getTimestamp( );
  int wallclock = 0;

  win_startup( );

  init_game( );

  while (!game_data.endOfGame)
  {

    // Update the window.
    mvprintw( 10, 10, "Time %d\n", (wallclock/10) );
    refresh( );

    // Process the user input.

    // Update the enemy

    // Delay for 100ms for a 10Hz game loop.
    while (getTimestamp( ) < curTime + TIME_DELTA );
    curTime += TIME_DELTA;

    wallclock++;
  }

  win_shutdown( );

  return 0;
}
