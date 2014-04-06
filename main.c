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

  // Initialize the workers
  game_data.workers.farmers      = 2;
  game_data.workers.wood_cutters = 1;
  game_data.workers.gold_miners  = 1;

  return;
}


void update_screen( void )
{
  // Update the window.
  mvwprintw( mainwin,  2,  3, "RTS Defense" );

  mvwprintw( mainwin,  4,  6, "Time  %03d", (game_data.wallclock/10) );
  mvwprintw( mainwin,  5,  6, "Level %3d", (game_data.level.iteration) );

  mvwprintw( mainwin,  4, 19, "<<status>>" );

  mvwprintw( mainwin,  7,  4, "Resources:" );
  mvwprintw( mainwin,  9,  6, "Food %4d", (int)game_data.resources.food );
  mvwprintw( mainwin, 10,  6, "Wood %4d", (int)game_data.resources.wood );
  mvwprintw( mainwin, 11,  6, "Gold %4d", (int)game_data.resources.gold );

  mvwprintw( mainwin,  7, 19, "Workers:" );
  mvwprintw( mainwin,  9, 21, "Farmers      %3d", game_data.workers.farmers );
  mvwprintw( mainwin, 10, 21, "Wood Cutters %3d", game_data.workers.wood_cutters );
  mvwprintw( mainwin, 11, 21, "Gold Miners  %3d", game_data.workers.gold_miners );

  mvwprintw( mainwin, 13, 21, "Archers      %3d", game_data.workers.archers );
  mvwprintw( mainwin, 14, 21, "Gunners      %3d", game_data.workers.gunners );

  mvwprintw( mainwin,  7, 42, "Skills:" );
  mvwprintw( mainwin,  9, 44, "Farming %3d", game_data.skills.farming );
  mvwprintw( mainwin, 10, 44, "Cutting %3d", game_data.skills.wood_cutting );
  mvwprintw( mainwin, 11, 44, "Mining  %3d", game_data.skills.gold_mining );

  mvwprintw( mainwin, 13, 44, "Archery %3d", game_data.skills.archery );
  mvwprintw( mainwin, 14, 44, "Gunnery %3d", game_data.skills.gunnery );

  if (game_data.enemy.active)
  {
    mvwprintw( mainwin, 16,  4, "Enemy:" );
    mvwprintw( mainwin, 18,  6, "Level   %2d", game_data.enemy.level );
    mvwprintw( mainwin, 19,  6, "HP     %3d", game_data.enemy.hp );
    mvwprintw( mainwin, 18, 21, "Speed      %2d", game_data.enemy.speed );
    mvwprintw( mainwin, 19, 21, "Distance  %3d", game_data.enemy.distance );
  }

  wrefresh( mainwin );

  return;
}


void update_resources( )
{
  // Need to update for skills...

  game_data.resources.food += ( (double)game_data.workers.farmers * 0.05 );
  game_data.resources.wood += ( (double)game_data.workers.wood_cutters * 0.05 );
  game_data.resources.gold += ( (double)game_data.workers.gold_miners * 0.05 );


  return;
}


int main( int argc, char *argv[] )
{
  unsigned long long curTime = getTimestamp( );

  win_startup( );

  init_game( );

  while (!game_data.endOfGame)
  {

    update_screen( );

    // Process the user input.

    // Update the enemy

    // Update the resources
    update_resources( );

    // Delay for 100ms for a 10Hz game loop.
    while (getTimestamp( ) < curTime + TIME_DELTA );
    curTime += TIME_DELTA;

    game_data.wallclock++;
  }

  win_shutdown( );

  return 0;
}
