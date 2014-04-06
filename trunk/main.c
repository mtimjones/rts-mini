#include <stdio.h>
#include <curses.h>
#include "rtsmini.h"

WINDOW *mainwin;
int EndOfGame = 0;

void startup( void )
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


void shutdown( void )
{
  delwin( mainwin );

  endwin( );

  return;
}


int main( int argc, char *argv[] )
{
  unsigned long long curTime = getTimestamp( );

  startup( );

  while (!EndOfGame)
  {

    // Delay for 100ms for a 10Hz game loop.
    while (getTimestamp( ) < curTime + TIME_DELTA );
    curTime += TIME_DELTA;
  }

  shutdown( );

  return 0;
}
