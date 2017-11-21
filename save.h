//Main quest status
#define STATUS_MAIN 0
#define STATUS_BAR 1
#define STATUS_MUTINY 2
#define STATUS_RATS 3
#define STATUS_SNOBS 4
#define STATUS_WARES 5

//All game status variables
extern int8_t game_status[];

extern void save_game();
extern void restore_game();
