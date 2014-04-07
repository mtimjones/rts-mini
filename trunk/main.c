#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <assert.h>
#include "rtsmini.h"

WINDOW *mainwin;

GameData game_data;

void win_startup( void )
{
  int offsety, offsetx;

  initscr( );
  cbreak( );
  noecho( );
  curs_set(0);
  nonl();

  offsetx = ( COLS - NCOLS ) / 2;
  offsety = ( LINES - NLINES ) / 2;

  mainwin = newwin( NLINES, NCOLS, offsety, offsetx );
  nodelay( mainwin, TRUE );
  keypad( mainwin, TRUE );

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
  extern void emit_cursor_position( );

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
  game_data.workers.farmers      = 1;
  game_data.workers.wood_cutters = 1;
  game_data.workers.gold_miners  = 1;

  // Initialize the menu position
  game_data.curmenu = WRKF;
  emit_cursor_position( );

  return;
}


void emit_cursor_position( void )
{
  int row, col1, col2;

  row  = cursors[game_data.curmenu].row;
  col1 = cursors[game_data.curmenu].col1;
  col2 = cursors[game_data.curmenu].col2;

  mvwaddch( mainwin, row, col1, '[' );
  mvwaddch( mainwin, row, col2, ']' );

  return;
}


void clear_cursor_position( void )
{
  int row, col1, col2;

  row  = cursors[game_data.curmenu].row;
  col1 = cursors[game_data.curmenu].col1;
  col2 = cursors[game_data.curmenu].col2;

  mvwaddch( mainwin, row, col1, ' ' );
  mvwaddch( mainwin, row, col2, ' ' );

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

  mvwprintw( mainwin, 16,  4, "Cost %3d/%3d/%3d (F/W/G) for %s",
               costs[game_data.curmenu].food, 
               costs[game_data.curmenu].wood, 
               costs[game_data.curmenu].gold,
               costs[game_data.curmenu].item );

  if (game_data.enemy.active)
  {
    mvwprintw( mainwin, 19,  4, "Enemy:" );
    mvwprintw( mainwin, 21,  6, "Level   %2d", game_data.enemy.level );
    mvwprintw( mainwin, 22,  6, "HP     %3d", game_data.enemy.hp );
    mvwprintw( mainwin, 21, 21, "Speed      %2d", game_data.enemy.speed );
    mvwprintw( mainwin, 22, 21, "Distance  %3d", game_data.enemy.distance );
  }

  wrefresh( mainwin );

  return;
}


void purchase_item( )
{
  if ( ( game_data.resources.food >= costs[game_data.curmenu].food ) &&
       ( game_data.resources.wood >= costs[game_data.curmenu].wood ) &&
       ( game_data.resources.gold >= costs[game_data.curmenu].gold ) )
  {
    game_data.resources.food -= costs[game_data.curmenu].food;
    game_data.resources.wood -= costs[game_data.curmenu].wood;
    game_data.resources.gold -= costs[game_data.curmenu].gold;

    switch( game_data.curmenu )
    {
      case WRKF:
        game_data.workers.farmers++;
        break;
      case WRKW:
        game_data.workers.wood_cutters++;
        break;
      case WRKM:
        game_data.workers.gold_miners++;
        break;
      case WRKA:
        game_data.workers.archers++;
        break;
      case WRKG:
        game_data.workers.gunners++;
        break;
      case SKLF:
// Need to multiply cost by 1.1 for each level purchased.
// for each level, the gold cost of the unit increases accordinly (1.1).
        break;
      case SKLW:
        break;
      case SKLM:
        break;
      case SKLA:
        break;
      case SKLG:
        break;
      default:
        assert(0);
        break;
    }

  }

  return;
}


void process_user_input( )
{
  int c;
  unsigned int next;

  c = wgetch( mainwin );

  if (c == ERR) return;

  clear_cursor_position( );

  switch( c )
  {
    case KEY_DOWN:
      next = GET_DOWN( game_data.curmenu );
      if (next != ILGL) game_data.curmenu = next;
      break;

    case KEY_UP:
      next = GET_UP( game_data.curmenu );
      if (next != ILGL) game_data.curmenu = next;
      break;

    case KEY_LEFT:
      next = GET_LEFT( game_data.curmenu );
      if (next != ILGL) game_data.curmenu = next;
      break;

    case KEY_RIGHT:
      next = GET_RIGHT( game_data.curmenu );
      if (next != ILGL) game_data.curmenu = next;
      break;

    case 0x0d:
      purchase_item( );
      break;
  }

  emit_cursor_position( );

  return;
}


void update_resources( )
{
  // Need to update for skills...

  game_data.resources.food += ( (double)game_data.workers.farmers * FOOD_MULT );
  game_data.resources.wood += ( (double)game_data.workers.wood_cutters * WOOD_MULT );
  game_data.resources.gold += ( (double)game_data.workers.gold_miners * GOLD_MULT );


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

    process_user_input( );

    // Update the enemy( );

    update_resources( );

    // Delay for 100ms for a 10Hz game loop.
    while (getTimestamp( ) < curTime + TIME_DELTA );
    curTime += TIME_DELTA;

    game_data.wallclock++;
  }

  win_shutdown( );

  return 0;
}
