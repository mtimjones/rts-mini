#include <stdio.h>

#ifndef __RTS_MINI_H__
#define __RTS_MINI_H__

typedef unsigned int uint32;

#define NLINES		30
#define NCOLS		60

#define TIME_DELTA	(unsigned long long)100000

unsigned long long getTimestamp( void );

#define INITIAL_FOOD  (double)80
#define INITIAL_WOOD  (double)40
#define INITIAL_GOLD  (double)10

#define FOOD_MULT     (double)0.05
#define WOOD_MULT     (double)0.04
#define GOLD_MULT     (double)0.02

typedef struct Cost {
  int food;
  int wood;
  int gold;
  char *item;
} Cost;

Cost costs[11]= {
  {  0,  0,  0, "Unknown     " }, // ILGL
  { 30, 20,  0, "Farmer      " }, // WRKF
  { 40, 10,  0, "Wood Cutter " }, // WRKW
  { 40, 30,  0, "Gold Miner  " }, // WRKM
  { 75, 50, 25, "Archer      " }, // WRKA
  { 90,  0, 50, "Gunner      " }, // WRKG
  {  0,  0,  0, "Farming     " }, // SKLF
  {  0,  0,  0, "Wood Cutting" }, // SKLW
  {  0,  0,  0, "Mining      " }, // SKLM
  {  0,  0,  0, "Archery     " }, // SKLA
  {  0,  0,  0, "Gunnery     " }  // SKLG
};

typedef enum State { PRE_GAME, IN_GAME, POST_GAME, END_GAME } State;

typedef struct Level {
  uint32 iteration;
  State state;
  int delay;
} Level;

typedef struct Resources {
  double food;
  double wood;
  double gold;
} Resources;

typedef struct Skills {
  uint32 farming;
  uint32 wood_cutting;
  uint32 gold_mining;
  uint32 archery;
  uint32 gunnery;
} Skills;

// A skill indicates the rate for the worker (gold/sec, range, etc.).

typedef struct Workers {
  uint32 farmers;
  uint32 wood_cutters;
  uint32 gold_miners;
  uint32 archers;
  uint32 gunners;
} Workers;

typedef struct Enemy {
  uint32 active;
  uint32 level;
  uint32 hp;
  uint32 speed;
  uint32 distance;
} Enemy;

typedef struct GameData {
  int endOfGame;
  int wallclock;
  unsigned int curmenu;
  Level level;
  Resources resources;
  Skills skills;
  Workers workers;
  Enemy enemy;
} GameData;


// Key Map

#define UP( x )	    ( x << 0  )
#define DOWN( x )   ( x << 8  )
#define LEFT( x )   ( x << 16 )
#define RIGHT( x )  ( x << 24 )

#define GET_UP( x )    ( ( moves[ x ] & 0xff ) )
#define GET_DOWN( x )  ( ( moves[ x ] >> 8 ) & 0xff )
#define GET_LEFT( x )  ( ( moves[ x ] >> 16 ) & 0xff )
#define GET_RIGHT( x ) ( ( moves[ x ] >> 24 ) & 0xff )

#define ILGL	0

#define WRKF    1
#define WRKW    2
#define WRKM    3
#define WRKA    4
#define WRKG    5

#define SKLF    6
#define SKLW    7
#define SKLM    8
#define SKLA    9
#define SKLG    10

const uint32 moves[11] = {
  0,                                                    // ILGL
  ( UP(ILGL) | DOWN(WRKW) | LEFT(ILGL) | RIGHT(SKLF) ), // WKRF
  ( UP(WRKF) | DOWN(WRKM) | LEFT(ILGL) | RIGHT(SKLW) ), // WKRW
  ( UP(WRKW) | DOWN(WRKA) | LEFT(ILGL) | RIGHT(SKLM) ), // WKRM
  ( UP(WRKM) | DOWN(WRKG) | LEFT(ILGL) | RIGHT(SKLA) ), // WKRA
  ( UP(WRKA) | DOWN(ILGL) | LEFT(ILGL) | RIGHT(SKLG) ), // WRKG
  ( UP(ILGL) | DOWN(SKLW) | LEFT(WRKF) | RIGHT(ILGL) ), // SKLF
  ( UP(SKLF) | DOWN(SKLM) | LEFT(WRKW) | RIGHT(ILGL) ), // SKLW
  ( UP(SKLW) | DOWN(SKLA) | LEFT(WRKM) | RIGHT(ILGL) ), // SKLM
  ( UP(SKLM) | DOWN(SKLG) | LEFT(WRKA) | RIGHT(ILGL) ), // SKLA
  ( UP(SKLA) | DOWN(ILGL) | LEFT(WRKG) | RIGHT(ILGL) )  // SKLG
};

typedef struct Position {
  int row;
  int col1;
  int col2;
} Position;

Position cursors[11] = {
  {  0,  0,  0 }, // ILGL
  {  9, 20, 37 }, // WRKF
  { 10, 20, 37 }, // WRKW
  { 11, 20, 37 }, // WRKM
  { 13, 20, 37 }, // WRKA
  { 14, 20, 37 }, // WRKG
  {  9, 43, 55 }, // SKLF
  { 10, 43, 55 }, // SKLW
  { 11, 43, 55 }, // SKLM
  { 13, 43, 55 }, // SKLA
  { 14, 43, 55 }  // SKLG
};

#endif // __RTS_MINI_H__

