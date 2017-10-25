#include "dialogue.h"

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

void fill_dialogue_buffer(uint8_t index){
  byte offset = 0;
  while( 1 ){
    byte tmp;
    for(byte i = 0; i < 16; i++){
      tmp = pgm_read_byte(((byte*)&dialogue[index])+i);
      if( tmp != '\0' ){
        combat_message[offset] = tmp;
        offset++;
      }else{
        break;
      }
    }
    index++;
    tmp = pgm_read_byte(((byte*)&dialogue[index]));
    if( tmp != '\n' ){
      combat_message[offset] = '\0';
      return;
    }
  }
}

uint8_t dialogue_index;
uint8_t dialogue_remaining;

void step_dialogue(){
  worldframe++;
  worldframe%=8;
  gb.display.setColor(WHITE);
  gb.display.fillRect(0,SCREEN_HEIGHT/2-1,SCREEN_WIDTH,SCREEN_HEIGHT/2+1);
  gb.display.setColor(BLACK);
  gb.display.drawRect(0,SCREEN_HEIGHT/2-1,SCREEN_WIDTH,SCREEN_HEIGHT/2+1);
  
  gb.display.cursorX = 4;
  gb.display.cursorY = SCREEN_HEIGHT/2;
  fill_dialogue_buffer(dialogue_index);
  gb.display.print(combat_message);

  if(gb.buttons.pressed(BTN_A)){
    gb.sound.playOK();
    if( dialogue_remaining == 0 ){
      // If dialogue is done, return to world mode
      mode = WORLD;
    }else{
      // If dialogue is not done, move to next chunk of text
      dialogue_remaining--;
      //Skip lines until we reach one that has no newline
      do{
        dialogue_index++;
      }while( '\n' == pgm_read_byte(((byte*)&dialogue[dialogue_index])) );
    }
  }
}

