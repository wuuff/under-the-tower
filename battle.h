#define RAT 0
#define SCAMP 1
#define RUFFIAN 2
#define THUG 3
#define BIG_RAT 4
#define PATRON 5
#define BOUNCER 6
#define SLAVER 7
#define SEA_RAT 8
#define SWABBIE 9
#define SAILOR 10
#define SKIPPER 11
#define CAPTAIN 12
#define BAD_RAT 13
#define WATCHER 14
#define BRUISER 15
#define MUSCLER 16
//#define OVERMAN 17 //Eliminate OVERMAN because we got rid of his dungeon
#define DOCTOR 17//18
#define PATIENT 18//19
#define SUBJECT 19//20
#define MUTANT 20//21
#define MADMAN 21//22
#define WOW_RAT 22//23
#define SNOB 23//24
#define RICHMAN 24//25
#define MAX_RAT 25//26
#define GUARD 26//27
#define GOLEM 27//28
#define OFFICER 28//29
#define LEADER 29//30
#define CRAB 30//31
#define EN_SHADOW 31//32

#define MUDLARK 0
#define SHADOW 1
#define NURSE 2

extern uint8_t meta_mode;
extern const char player_names[][8];
extern const char enemy_names[][8];
extern void copy_to_buffer(byte index, const char arr[][8]);
