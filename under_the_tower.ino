#include <SPI.h>
//Use for PROGMEM
#include <avr/pgmspace.h>
#include "overworld.h"
#include <Gamebuino.h>
Gamebuino gb;

const char dialogue[][16] PROGMEM = {
//"123456789012345",
"THIS CITY IS",
"\n DYING",
"GATES SEALED",
"\n DUE TO",
"\n PLAGUE",
"ITS RULERS SAFE",
"\n HIGH ABOVE IN",
"\n THE TOWER",
"WHILE THE POOR",
"\n DIE BELOW",
"ON MUD FLATS",
"\n BY THE RIVER",
"\n THEY SCAVENGE",
"THEY ARE KNOWN",
"\n AS MUDLARKS",
"THIS IS ABOUT",
"\n ONE OF THESE",
"\n THAT LIVES",
"UNDER THE TOWER",

"I HAVE OBSERVED",
"\n FROM AFAR",
"\n YOUR SKILL",
"I HAVE A TASK",
"\n THAT WILL PAY",
"\n A HUNDREDFOLD",
"BUT FIRST YOU",
"\n MUST PROVE I",
"\n CHOSE WISELY",
"SEEK THE CATPAW",
"\n A PLACE OF",
"\n DECADENCE",
"A GIRL IS HELD",
"\n BOUND WITHIN",
"FREE HER AND",
"\n BRING HER TO",
"\n HER FATHER",

"YOUR KIND IS",
"\n NOT ALLOWED",
"\n IN THE CATPAW",

"YOU LOOK YOUNG",
"\n TO BE A",
"\n CUSTOMER HERE",

"A KID LIKE YOU",
"\n IS TOO YOUNG",
"\n TO BE HERE"
};

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
//Transitions to world or dungeon modes
#define TO_WORLD 3
#define TO_COMBAT 4
#define TO_DUNGEON 5

#define TRANSITION_DIFF 3

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
        }
        break;
      case TO_DUNGEON:
        step_transition();
      case DUNGEON:
        if( transition >= 0 ){
          draw_dungeon();
          step_dungeon();
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
    }

    if(gb.buttons.pressed(BTN_C)){
      gb.titleScreen(F("Paused"));
      gb.battery.show = false;
    }
  }
}
