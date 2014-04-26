#include <stdio.h>

#ifndef __RTS_MINI_H__
#define __RTS_MINI_H__

typedef unsigned int uint32;

#define NLINES		30
#define NCOLS		60

#define TIME_DELTA	(unsigned long long)100000

unsigned long long getTimestamp( void );

#define INITIAL_FOOD  (double)80
#define INITIAL_WOOD  (double)60
#define INITIAL_GOLD  (double)60

#define FOOD_MULT     (double)0.07
#define WOOD_MULT     (double)0.05
#define GOLD_MULT     (double)0.038

typedef struct Cost {
  double food;
  double wood;
  double gold;
  char *item;
} Cost;

Cost costs[11]= {
  {  0.0,  0.0,  0.0, "Unknown     " }, // ILGL
  { 20.0, 10.0,  0.0, "Farmer      " }, // WRKF
  { 20.0, 20.0,  0.0, "Wood Cutter " }, // WRKW
  { 30.0, 30.0,  0.0, "Gold Miner  " }, // WRKM
  { 45.0, 30.0, 15.0, "Archer      " }, // WRKA
  { 60.0,  0.0, 50.0, "Gunner      " }, // WRKG
  { 00.0,  0.0, 30.0, "Farming     " }, // SKLF
  { 00.0,  0.0, 30.0, "Wood Cutting" }, // SKLW
  { 00.0,  0.0, 50.0, "Mining      " }, // SKLM
  { 00.0,  0.0, 60.0, "Archery     " }, // SKLA
  { 00.0,  0.0, 60.0, "Gunnery     " }  // SKLG
};

typedef enum State { 
  INITIALIZATION=0,
  ALL_QUIET=1,
  PRE_GAME=2, 
  IN_GAME=3, 
  POST_GAME=4, 
  YOU_DIED=5,
  GAME_OVER=6,
  EXIT=7
} State;

const char *status[8]={
  "Use the arrow keys and ENTER to purchase.\0", // INITIALIZATION
  "All is quiet...  Prepare for the enemy.  \0", // ALL_QUIET
  "Rumbling...  An enemy approaches...      \0", // PRE_GAME
  "Your troops are attacking the enemy...   \0", // IN_GAME
  "Enemy is defeated.                       \0", // POST_GAME
  "Enemy reached the barricade.  You lose.  \0", // YOU_DIED
  "Final enemy is defeated.  You win.       \0", // GAME_OVER
  "                                         \0"  // EXIT
};

typedef struct Level {
  int iteration;
  State state;
  int delay;
} Level;

typedef struct Resources {
  double food;
  double wood;
  double gold;
} Resources;

typedef struct Skills {
  int farming;
  int wood_cutting;
  int gold_mining;
  int archery;
  int gunnery;
} Skills;

// A skill indicates the rate for the worker (gold/sec, range, etc.).

typedef struct Workers {
  int farmers;
  int wood_cutters;
  int gold_miners;
  int archers;
  int gunners;
} Workers;

typedef struct Enemy {
  double hp;
  double max_hp;
  int speed;
  double distance;
} Enemy;

typedef struct Damage {
  double archer_dps;
  double gunner_dps;
} Damage;

typedef struct GameData {
  int endOfGame;
  int wallclock;
  unsigned int curmenu;
  Level level;
  Resources resources;
  Skills skills;
  Workers workers;
  Damage damage;
  Enemy enemy;
} GameData;

typedef struct EnemyLevel {
  double hp;
  int speed;
} EnemyLevel;

#define MAX_LEVELS	10

EnemyLevel enemies[MAX_LEVELS] = {
  {  40.0,   2 },
  {  50.0,   3 },
  {  50.0,   4 },
  { 100.0,   3 },
  {  25.0,   9 },
  {  45.0,   8 },
  {  40.0,   9 },
  {  70.0,   8 },
  {  80.0,   7 },
  { 999.0,   1 }
};


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

