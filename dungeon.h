#ifndef DUNGEON_H
#define DUNGEON_H
extern uint8_t dungeonid;
extern uint8_t dungeon_level;

struct dungeon{
  uint8_t x;
  uint8_t y;
  //uint8_t type;
  uint8_t theme;
  uint8_t size;
  //uint8_t exit;
  uint8_t spawns[3];
};

#define NUM_DUNGEONS 18
extern const struct dungeon dungeons[];

#define NUM_DUN_TILES 4
#define NUM_COMMON_TILES 4

#define DUN_WALL (NUM_DUN_TILES+NUM_COMMON_TILES+1)

#endif
