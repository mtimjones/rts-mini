#include <stdio.h>

#ifndef __RTS_MINI_H__
#define __RTS_MINI_H__

typedef unsigned int uint32;

#define NLINES		30
#define NCOLS		60

#define TIME_DELTA	(unsigned long long)100000

unsigned long long getTimestamp( void );

#define INITIAL_FOOD  (double)100
#define INITIAL_WOOD   (double)50
#define INITIAL_GOLD   (double)50

#define COST_OF_FARMER 25
#define COST_OF_CUTTER 30
#define COST_OF_MINER  50

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
  Level level;
  Resources resources;
  Skills skills;
  Workers workers;
  Enemy enemy;
} GameData;

#endif // __RTS_MINI_H__

