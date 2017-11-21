#include <SPI.h>
//Use for PROGMEM
#include <avr/pgmspace.h>
#include "overworld.h"
#include "save.h"
#include "dialogue.h"
#include "battle.h"
#include "dungeon.h"
#include "events.h"
#include <Gamebuino.h>
Gamebuino gb;

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

#define WORLD 0
#define COMBAT 1
#define DUNGEON 2
#define DIALOGUE 3
//Transitions to world or dungeon modes
#define TO_WORLD 4
#define TO_COMBAT 5
#define TO_DUNGEON 6
//Menu mode
#define MAIN_MENU 7
#define PAUSE_MENU 8

#define TRANSITION_DIFF 4

byte mode = WORLD;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  //gb.titleScreen(F("Test"));
  //gb.battery.show = false;
  mode = MAIN_MENU;

  /*for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 16; j++){
      map_cache[i][j] = pgm_read_byte(&world[i*64+j]);
    }
  }*/
}

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
          if( game_status[STATUS_MAIN] == -1 ){
            game_status[STATUS_MAIN] = 0;
            display_dialogue(TXT_INTRO,TXT_INTRO_LEN,15,menu_text);
          }
          else if( game_status[STATUS_MAIN] == 0 && dudex < 7*8 ){
            game_status[STATUS_MAIN] = 1;
            display_dialogue(TXT_SDW_INTRO,TXT_SDW_INTRO_LEN,SHADOW,player_names);
          }else if( game_status[STATUS_MAIN] == 1 && dudey < 41*8 ){
            game_status[STATUS_MAIN] = 2;
            display_dialogue(TXT_SDW_CATPAW,TXT_SDW_CATPAW_LEN,SHADOW,player_names);
          }/*else if( game_status[STATUS_MAIN] == 12 && dudex > 55*8 && dudey < 45*8 ){
            game_status[STATUS_MAIN] = 13;
            display_dialogue(TXT_SDW_SHIP,TXT_SDW_SHIP_LEN,SHADOW,player_names);
          }*/
        }
        break;
      case TO_DUNGEON:
        step_transition();
      case DUNGEON:
        if( transition >= 0 ){
          draw_dungeon();
          step_dungeon();

          check_events();//Dialogue and boss battles encountered in dungeons
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
        break;
      case MAIN_MENU:
        gb.display.cursorX = SCREEN_WIDTH/2-7*4;
        gb.display.cursorY = 6;
        gb.display.println(F("UNDER THE TOWER"));
        gb.display.cursorX = SCREEN_WIDTH/2-2*4;
        gb.display.cursorY = SCREEN_HEIGHT/2;
        gb.display.println(F("NEW"));
        gb.display.cursorX = SCREEN_WIDTH/2-2*4;
        gb.display.println(F("LOAD"));
        gb.display.cursorX = SCREEN_WIDTH/2-2*4;
        gb.display.println(F("QUIT"));
        gb.display.cursorX = SCREEN_WIDTH/2-3*4;
        gb.display.cursorY = SCREEN_HEIGHT/2;
        if(menu_selection == 1){
          gb.display.cursorY += 6;          
        }
        if(menu_selection == 2){
          gb.display.cursorY += 12;          
        }
        gb.display.print(F("\20"));
        if(gb.buttons.pressed(BTN_UP)){
          menu_selection--;
          if( menu_selection == 255 ) menu_selection = 2;
        }
        else if(gb.buttons.pressed(BTN_DOWN)){
          menu_selection++;
          menu_selection%=3;
        }
        else if(gb.buttons.pressed(BTN_A)){
          gb.pickRandomSeed();//For random numbers, later
          if(menu_selection == 1){
            restore_game();
          }
          else if(menu_selection == 2){
            gb.changeGame();
          }
          else{
            mode = WORLD;
          }
        }
        break;
      case PAUSE_MENU:
        gb.display.cursorX = SCREEN_WIDTH/2-3*4;
        gb.display.cursorY = 6;
        gb.display.println(F("PAUSED"));
        gb.display.cursorX = SCREEN_WIDTH/2-2*4;
        gb.display.cursorY = SCREEN_HEIGHT/2;
        gb.display.println(F("BACK"));
        if( meta_mode == WORLD ){
          gb.display.cursorX = SCREEN_WIDTH/2-2*4;
          gb.display.println(F("SAVE"));
        }
        gb.display.cursorX = SCREEN_WIDTH/2-3*4;
        gb.display.cursorY = SCREEN_HEIGHT/2;
        if(menu_selection == 1){
          gb.display.cursorY += 6;          
        }
        gb.display.print(F("\20"));
        if(meta_mode == WORLD && gb.buttons.pressed(BTN_UP)){
          menu_selection--;
          if( menu_selection == 255 ) menu_selection = 1;
        }
        else if(meta_mode == WORLD && gb.buttons.pressed(BTN_DOWN)){
          menu_selection++;
          menu_selection%=2;
        }
        else if(gb.buttons.pressed(BTN_A)){
          if(menu_selection == 1){
            save_game();
          }
          mode = meta_mode;
        }
        break;
    }

    if(mode != PAUSE_MENU && mode != TO_COMBAT && mode != COMBAT && gb.buttons.pressed(BTN_C)){
      menu_selection = 0;
      meta_mode = mode;
      mode = PAUSE_MENU;
    }
  }
}
