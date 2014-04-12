#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <assert.h>
#include "rtsmini.h"

WINDOW *mainwin;

static GameData game_data;

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
  game_data.level.state = INITIALIZATION;
  game_data.level.delay = 10; // 10 seconds

  // Initialize the resources
  game_data.resources.food = INITIAL_FOOD;
  game_data.resources.wood = INITIAL_WOOD;
  game_data.resources.gold = INITIAL_GOLD;

  // Initialize the workers
  game_data.workers.farmers      = 1;
  game_data.workers.wood_cutters = 1;
  game_data.workers.gold_miners  = 1;

  // Initialize the skills
  game_data.skills.farming = 1;
  game_data.skills.wood_cutting = 1;
  game_data.skills.gold_mining = 1;
  game_data.skills.archery = 1;
  game_data.skills.gunnery = 1;

  // Initialize DPS for soldiers
  game_data.damage.archer_dps = 1.0;
  game_data.damage.gunner_dps = 2.0;

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
               (int)costs[game_data.curmenu].food, 
               (int)costs[game_data.curmenu].wood, 
               (int)costs[game_data.curmenu].gold,
               (int)costs[game_data.curmenu].item );

  if ( game_data.level.state == IN_GAME )
  {
    mvwprintw( mainwin, 20,  4, "Enemy:" );
    mvwprintw( mainwin, 22,  6, "Level   %2d", game_data.level.iteration );
    mvwprintw( mainwin, 23,  6, "HP     %3d", (int)game_data.enemy.hp );
    mvwprintw( mainwin, 22, 21, "Speed      %2d", game_data.enemy.speed );
    mvwprintw( mainwin, 23, 21, "Distance  %3d", (int)game_data.enemy.distance );
  }
  else
  {
    mvwprintw( mainwin, 20,  4, "      " );
    mvwprintw( mainwin, 22,  6, "           ");
    mvwprintw( mainwin, 23,  6, "          ");
    mvwprintw( mainwin, 22, 21, "              ");
    mvwprintw( mainwin, 23, 21, "             ");
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
        costs[SKLF].gold *= 1.1;
        game_data.skills.farming++;
        break;
      case SKLW:
        game_data.skills.wood_cutting++;
        costs[SKLW].gold *= 1.1;
        break;
      case SKLM:
        game_data.skills.gold_mining++;
        costs[SKLM].gold *= 1.1;
        break;
      case SKLA:
        game_data.skills.archery++;
        costs[SKLA].gold *= 1.1;
        costs[WRKA].food *= 1.1;
        costs[WRKA].wood *= 1.1;
        costs[WRKA].gold *= 1.1;
        break;
      case SKLG:
        game_data.skills.gunnery++;
        costs[SKLG].gold *= 1.1;
        costs[WRKG].food *= 1.1;
        costs[WRKG].wood *= 1.1;
        costs[WRKG].gold *= 1.1;
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
  game_data.resources.food += ( (double)game_data.workers.farmers * 
                                ((double)game_data.skills.farming * FOOD_MULT ) );
  game_data.resources.wood += ( (double)game_data.workers.wood_cutters *
                                ((double)game_data.skills.farming * WOOD_MULT ) );
  game_data.resources.gold += ( (double)game_data.workers.gold_miners *
                                ((double)game_data.skills.farming * GOLD_MULT ) );

  return;
}


void update_enemy( void )
{
  double damage = 0.0;

  if ( game_data.level.state == IN_GAME )
  {
    // Archers attack
    damage = ((double)game_data.workers.archers * game_data.damage.archer_dps) / 10.0;    

    // Gunners attack
    damage += (((double)game_data.workers.gunners * game_data.damage.gunner_dps) / 10.0);

    // Enemy damage
    game_data.enemy.hp -= damage;
    if (game_data.enemy.hp < 0.0) game_data.enemy.hp = 0;

    // Test the enemy
    if ( game_data.enemy.hp <= 0 )
    {
      if ( game_data.level.iteration++ == MAX_LEVELS )
      {
        game_data.level.state = GAME_OVER;
        game_data.level.delay = 5;
      }
      else 
      {
        game_data.level.state = POST_GAME;
        game_data.level.delay = 10;
      }
    }
    else
    {
      // Decrease enemy distance
      game_data.enemy.distance -= ( ( (double)game_data.enemy.speed ) / 10.0 );

      // Test the distance
      if ( game_data.enemy.distance <= 0.0 ) 
      {
        game_data.level.state = YOU_DIED;
        game_data.level.delay = 10;
      }
    }
  }
  return;
}


void init_enemy( void )
{
  // Iteration starts at 1, so we subtract one for the enemies array
  game_data.enemy.hp = enemies[game_data.level.iteration-1].hp;
  game_data.enemy.speed = enemies[game_data.level.iteration-1].speed;
  game_data.enemy.distance = 100.0;

  return;
}


void update_game_state( )
{
  if (game_data.level.delay > 0)
  {
    mvwprintw( mainwin, 18, 4, "%s (%d)", 
                status[game_data.level.state], (game_data.level.delay-1) );

    // Wait for the delay to expire
    if ( --game_data.level.delay == 0 )
    {
      // Transition to the next state
      switch( game_data.level.state )
      {
        case INITIALIZATION:
          game_data.level.state = ALL_QUIET;
          game_data.level.delay = 5;
          break;

        case ALL_QUIET:
          game_data.level.state = PRE_GAME;
          game_data.level.delay = 5;
          break;

        case PRE_GAME:
          game_data.level.state = IN_GAME;
          game_data.level.delay = 0;
          mvwprintw( mainwin, 18, 4, "%s      ", status[game_data.level.state] );
          init_enemy( );
          break;

        case IN_GAME:
          break;

        case POST_GAME:
          if ( game_data.level.iteration++ == MAX_LEVELS )
          {
            game_data.level.state = GAME_OVER;
            game_data.level.delay = 5;
          }
          else 
          {
            game_data.level.state = ALL_QUIET;
            game_data.level.delay = 5;
          }
          break;

        case YOU_DIED:
          game_data.level.state = EXIT;
          break;

        case GAME_OVER:
          game_data.level.state = EXIT;
          break;

        case EXIT:
          break;

      }

    }

  }

  return;
}


int main( int argc, char *argv[] )
{
  unsigned long long curTime = getTimestamp( );

  win_startup( );

  init_game( );

  while ( game_data.level.state != EXIT )
  {
    process_user_input( );

    update_resources( );

    update_enemy( );

    update_screen( );

    // Delay for 100ms for a 10Hz game loop.
    while ( getTimestamp( ) < curTime + TIME_DELTA );
    curTime += TIME_DELTA;

    game_data.wallclock++;

    if ( ( game_data.wallclock % 10 ) == 0 ) 
    {
      update_game_state( );
    }

  }

  win_shutdown( );

  return 0;
}
