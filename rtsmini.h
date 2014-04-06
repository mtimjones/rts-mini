#include <stdio.h>

#ifndef __RTS_MINI_H__
#define __RTS_MINI_H__

typedef unsigned int uint32;

#define NLINES		30
#define NCOLS		60

#define TIME_DELTA	(unsigned long long)100000

unsigned long long getTimestamp( void );

typedef enum State { PRE_GAME, IN_GAME, POST_GAME, END_GAME } State;

typedef struct Level {
  uint32 iteration;
  State state;
  int delay;
} Level;

typedef struct Resources {
  uint32 food;
  uint32 wood;
  uint32 gold;
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
  uint32 level;
  uint32 hp;
  uint32 speed;
  uint32 distance;
} Enemy;

typedef struct GameData {
  Level level;
  Resources resources;
  Skills skills;
  Workers workers;
  Enemy enemy;
} GameData;

#endif // __RTS_MINI_H__

