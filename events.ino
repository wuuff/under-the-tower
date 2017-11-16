#include "events.h"
#include "save.h"
#include "battle.h"
#include "dungeon.h"
#include "dialogue.h"

struct dialogue_event{
  uint8_t status_index;
  uint8_t status_value;
  uint8_t dungeon_id;
  uint8_t dungeon_level;
  uint16_t dialogue_index;
  uint8_t dialogue_len;
  uint8_t name_index;
  uint8_t name_array;
};

/* Boss events depend on dialogue occurring before
 * them, so they don't bother to check the dungeon.
 */
struct boss_event{
  uint8_t status_index;
  uint8_t status_value;
  uint8_t boss_id;
};

#define PLAYER_ARRAY 0
#define ENEMY_ARRAY 1

#define NUM_DIALOGUE_EVENTS 8
const dialogue_event dialogue_events[NUM_DIALOGUE_EVENTS] PROGMEM = {
  //CATPAW---SLAVER DIALOGUE
  {STATUS_MAIN,2,0,2,TXT_SLAVER,TXT_SLAVER_LEN,SLAVER,ENEMY_ARRAY},
  //CATPAW---GIRL THANKS
  {STATUS_MAIN,4,0,2,TXT_GIRL_THX,TXT_GIRL_THX_LEN,3,PLAYER_ARRAY},
  //CATPAW---SHADOW DIALOGUE
  {STATUS_MAIN,5,0,0,TXT_SDW_BATTLE,TXT_SDW_BATTLE_LEN,SHADOW,PLAYER_ARRAY},
  //CATPAW---SHADOW JOINS (SPECIAL CASE)
  {STATUS_MAIN,7,0,0,TXT_SDW_WIN,TXT_SDW_WIN_LEN,SHADOW,PLAYER_ARRAY},
  //FATHER'S RESIDENCE---GIRL WARNING
  {STATUS_MAIN,8,1,0,TXT_GIRL_FATHER,TXT_GIRL_FATHER_LEN,3,PLAYER_ARRAY},
  //FATHER'S RESIDENCE---ENEMY ALERT
  {STATUS_MAIN,9,1,2,TXT_ENEMY,TXT_ENEMY_LEN,THUG,ENEMY_ARRAY},
  //FATHER'S RESIDENCE---FATHER THANKS
  {STATUS_MAIN,11,1,2,TXT_FATHER,TXT_FATHER_LEN,4,PLAYER_ARRAY},
  //LARGE SHIP---ENEMY ALERT
  {STATUS_MAIN,13,4,3,TXT_ENEMY,TXT_ENEMY_LEN,CAPTAIN,ENEMY_ARRAY},
};

#define NUM_BOSS_EVENTS 4
const boss_event boss_events[NUM_BOSS_EVENTS] PROGMEM = {
  //CATPAW---SLAVER BOSS
  {STATUS_MAIN,3,SLAVER},
  //CATPAW---SHADOW BOSS
  {STATUS_MAIN,6,EN_SHADOW},
  //FATHER'S RESIDENCE---THUG MINIBOSS
  {STATUS_MAIN,10,THUG},
  //LARGE SHIP---CAPTAIN BOSS
  {STATUS_MAIN,14,CAPTAIN},
};

void check_events(){
  uint8_t i,j;
  uint8_t* tmp;
  struct dialogue_event event;
  struct boss_event bevent;
  for( i = 0; i < NUM_DIALOGUE_EVENTS; i++ ){
    tmp = (uint8_t*)&dialogue_events[i];
    // Load our local variable with this entry
    for( j = 0; j < 9; j++ ){
      //Assume the byte layout in memory to read bytes
      *(((uint8_t*)&event)+j) = pgm_read_byte(tmp + j);
    }
    // If this event is active (correct status value, dungeon, and dungeon floor)
    if( game_status[event.status_index] == event.status_value && dungeonid == event.dungeon_id && dungeon_level == event.dungeon_level ){
      //Special case: shadow joins party
      if( game_status[event.status_index] == 7 ){
        party[SHADOW].level = 8;//Add shadow to party
      }
      game_status[event.status_index]++;
      if( event.name_array == PLAYER_ARRAY ){
        display_dialogue(event.dialogue_index,event.dialogue_len,event.name_index,player_names);
      }else{
        display_dialogue(event.dialogue_index,event.dialogue_len,event.name_index,enemy_names);
      }
      return;
    }
  }
  for( i = 0; i < NUM_BOSS_EVENTS; i++ ){
    tmp = (uint8_t*)&boss_events[i];
    // Load our local variable with this entry
    for( j = 0; j < 3; j++ ){
      //Assume the byte layout in memory to read bytes
      *(((uint8_t*)&bevent)+j) = pgm_read_byte(tmp + j);
    }
    // If this boss event is active (correct status value only)
    if( game_status[bevent.status_index] == bevent.status_value ){
      game_status[bevent.status_index]++;
      meta_mode = bevent.boss_id;//Boss id stored in meta_mode
      mode = COMBAT;
      return;
    }
  }
}

