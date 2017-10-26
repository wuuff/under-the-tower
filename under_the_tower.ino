#include <SPI.h>
//Use for PROGMEM
#include <avr/pgmspace.h>
#include "overworld.h"
#include "save.h"
#include "dialogue.h"
#include "battle.h"
#include "dungeon.h"
#include <Gamebuino.h>
Gamebuino gb;

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.titleScreen(F("Test"));
  gb.battery.show = false;
  gb.pickRandomSeed();//For random numbers, later

  /*for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 16; j++){
      map_cache[i][j] = pgm_read_byte(&world[i*64+j]);
    }
  }*/
}

#define WORLD 0
#define COMBAT 1
#define DUNGEON 2
#define DIALOGUE 3
//Transitions to world or dungeon modes
#define TO_WORLD 4
#define TO_COMBAT 5
#define TO_DUNGEON 6

#define TRANSITION_DIFF 4

byte mode = WORLD;

void step_transition(){
  uint8_t i;
  for(i = 0; i < SCREEN_HEIGHT/2+1 - abs(transition); i++ ){
    gb.display.drawFastVLine(i,0,SCREEN_HEIGHT);
    gb.display.drawFastVLine(SCREEN_WIDTH-i,0,SCREEN_HEIGHT);
    gb.display.drawFastHLine(0,i,SCREEN_WIDTH);
    gb.display.drawFastHLine(0,SCREEN_HEIGHT-i,SCREEN_WIDTH);
  }
  transition+=4;
  //If we are halfway into the transition, start drawing the next scene
  //And stop saving the last frame of the previous scene
  if( transition >= 0 ){
    gb.display.persistence = false;
  }
  //If we are done with the transition, move to the ordinary non-transition mode
  if( transition > SCREEN_HEIGHT/2 ){
    mode -= TRANSITION_DIFF;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(gb.update()){
    switch( mode ){
      case TO_WORLD:
        step_transition();
      case WORLD:
        if( transition >= 0 ){
          draw_world();
          step_world();
          if( game_status[STATUS_SHADOW1] == 0 && dudex < 24*8 ){
            game_status[STATUS_SHADOW1] = 1;
            meta_mode = WORLD;//So the dialogue displays the world
            dialogue_index = 19;
            dialogue_remaining = 5;
            mode = DIALOGUE;
          }
        }
        break;
      case TO_DUNGEON:
        step_transition();
      case DUNGEON:
        if( transition >= 0 ){
          draw_dungeon();
          step_dungeon();
          //A really hacky way to implement the bosses
          //CATPAW---SLAVER DIALOGUE
          if( game_status[STATUS_SLAVER] == 0 && dungeonid == 0 && dungeon_level == 1 ){
            game_status[STATUS_SLAVER] = 1;
            meta_mode = TO_DUNGEON;
            dialogue_index = 36;
            dialogue_remaining = 0;
            mode = DIALOGUE;
          }
          //CATPAW---SLAVER BOSS
          else if( game_status[STATUS_SLAVER] == 1 ){
            game_status[STATUS_SLAVER] = 2;
            meta_mode = SLAVER;//Boss id stored in meta_mode
            mode = COMBAT;
          }
          //CATPAW---GIRL THANKS
          else if( game_status[STATUS_SLAVER] == 2 ){
            game_status[STATUS_SLAVER] = 3;
            meta_mode = DUNGEON;
            dialogue_index = 39;
            dialogue_remaining = 2;
            mode = DIALOGUE;
          }
        }
        break;
      case TO_COMBAT:
        if( transition >= 0 ){
          do_combat();//Draw first to avoid the text overdrawing the transition
        }
        step_transition();
        break;
      case COMBAT:
        if( transition >= 0 ){
          do_combat();
        }
        break;
      case DIALOGUE:
        if( meta_mode == WORLD )
          draw_world();
        else
          draw_dungeon();
        step_dialogue();
    }

    if(gb.buttons.pressed(BTN_C)){
      gb.titleScreen(F("Paused"));
      gb.battery.show = false;
    }
  }
}
