#include <stdio.h>
#include <sys/time.h>

unsigned long long getTimestamp( void )
{
  struct timeval tv;
  unsigned long long t1;

  gettimeofday( &tv, NULL );

  t1 = (tv.tv_sec * 1000000) + tv.tv_usec;

  return t1;
}

