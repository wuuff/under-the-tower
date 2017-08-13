#ifdef NOTDEFINED
//Scamp: Level 1
//Ruffian: Level 2
//Scoundrel: Level 4
//Thief (or maybe Shadow) recruits you on his quest

const byte enemy_levels[] PROGMEM = {
1,
1,
2,
4 
};

const byte enemy_speeds[] PROGMEM = {
2,
3,
2,
2  
};

const byte enemy_imgs[] PROGMEM = {
0,
1,
0,
0,
1,
0,
2
};

const char enemy_names[][8] PROGMEM = {
"SCAMP",
"RAT",
"RUFFIAN",
"THUG",
"BIG RAT",
"PATRON",
"BOUNCER"
};

const byte enemybmps[] PROGMEM = {8,16, // Scoundrel
B01000000,
B01111000,
B00101100,
B00100100,
B00101010,
B00011101,
B00100001,
B00100101,
B00100101,
B01010110,
B10010100,
B00011100,
B00010100,
B00110010,
B11100010,
B10000011,
8,16,                   // Rat
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000001,
B01010010,
B01111001,
B11111110,
B01111100,
8,16,                   // Bouncer
B00000000,
B00000000,
B00011000,
B00011000,
B00111100,
B01111110,
B11111111,
B11110111,
B11111001,
B10001110,
B01111110,
B01111110,
B01100110,
B01100110,
B00100100,
B01100110,
};

//Stats:
//Level (determines max health + base damage)
//Health
//Speed

const char player_names[][8] PROGMEM = {
"MUDLARK",
"SHADOW",
"NURSE"
};

uint8_t mudlark_level = 1;
uint8_t mudlark_health = 10;
uint8_t mudlark_speed = 2;
uint8_t mudlark_xp = 0;

uint8_t shadow_level = 0;
uint8_t shadow_health = 0;
uint8_t shadow_speed = 2;
uint8_t shadow_xp = 0;

uint8_t nurse_level = 0;
uint8_t nurse_health = 0;
uint8_t nurse_speed = 2;
uint8_t nurse_xp = 0;

//Abilities:
//Mudlark: Rally, scavenge
//Nurse: Heal one, heal all
//Thief: Sneak, Speed up

//TODO: More effects, more items
#define HEAL 0

const char item_names[][8] PROGMEM = {
  "FRUIT",  // 5 HP
  "BREAD",  // 10 HP
  "TONIC",  // 50% HP
  "TEA",    // 1 SP
  "LIQUOR", // 1 damage resist
  "MEAT"    // 1 damage increase        
};

#define INVENTORY_SIZE 6

unsigned char inventory[6] = {2,1,0,0,0,0};//Each element == how much of each item

void try_combat(){
  if( random(16) == 0 && random(16) == 0 ){
    mode = COMBAT;
  }
}

const char menu_text[][8] PROGMEM = {
  "FLAIL",
  "RALLY",
  "SCAVNGE",
  "OTHER",
  "VANISH",
  "STRIKE",
  "SPEED",
  "OTHER",
  "HP ONE",
  "HP ALL",
  "PROTECT",
  "OTHER",
  "ITEM",
  "RUN",
  "",//TODO: optimize these out
  "",
  };

const char combat_text[][8] PROGMEM = {
  " HIT ",
  "\n FOR ",
  " DAMAGE",
  " HEAL ",
  " FALLS",
  "YOU WIN",
  " XP GET",
  "TRAPPED"
};

#define MUDLARK_MENU 0
#define SHADOW_MENU 1
#define NURSE_MENU 2
#define SECONDARY_MENU 3
#define ENEMY_MENU 4
#define ALLY_MENU 5
#define ITEM_MENU 6
// More...

#define PRECOMBAT -1
#define MUDLARK 0
#define SHADOW 1
#define NURSE 2
#define ENEMY1 3
#define ENEMY2 4
#define ENEMY3 5
#define MESSAGE 6
#define VICTORY 7

char combat_mode = PRECOMBAT;
byte menu_selection = 0;
byte combat_selection = 0;
char combat_buffer[8] = "\0\0\0\0\0\0\0";
char combat_message[64];//Max of 64 characters in message, which should be plenty
int8_t combat_status[6] = {0,0,0,0,0,0};//Determines who moves next
uint8_t combat_xp = 0;

int8_t enemies[3];
uint8_t enemy_health[3];

uint8_t calculate_damage(uint8_t lvl){
  return lvl*10/8;
}

void gen_enemies(){
  combat_xp = 0;
  //Always generate center enemy
  enemies[1] = random(3);
  combat_xp+=pgm_read_byte(&enemy_levels[enemies[1]]);
  enemy_health[1] = pgm_read_byte(&enemy_levels[enemies[1]])*10/2;
  //50% chance of left enemy
  if( random(2) == 0 ){
    enemies[0] = random(3);
    combat_xp+=pgm_read_byte(&enemy_levels[enemies[0]]);
    enemy_health[0] = pgm_read_byte(&enemy_levels[enemies[0]])*10/2;
  }else{
    enemies[0] = -1;
  }
  //50% chance of right enemy
  if( random(2) == 0 ){
    enemies[2] = random(3);
    combat_xp+=pgm_read_byte(&enemy_levels[enemies[2]]);
    enemy_health[2] = pgm_read_byte(&enemy_levels[enemies[2]])*10/2;
  }else{
    enemies[2] = -1;
  }
}

void copy_to_buffer(byte index, const char arr[][8]){
  for(byte i = 0; i < 7; i++){
    combat_buffer[i] = pgm_read_byte(((byte*)&arr[index])+i);
  }
  //Final null byte was defined initially
}

uint8_t append_to_msg_buffer(uint8_t index, const char arr[][8], uint8_t offset){
  for(byte i = 0; i < 8; i++){
    byte tmp = pgm_read_byte(((byte*)&arr[index])+i);
    if( tmp != '\0' ){
      combat_message[offset+i] = tmp;
    }else{
      return offset+i;
    }
  }
}

#define PL2EN 0
#define EN2PL 1
#define EFALL 2
#define PWIN 3
#define PHEAL 4

//TODO: space optimize this
void copy_action_to_msg_buffer(uint8_t source, uint8_t dest, uint8_t amount, uint8_t type){
  uint8_t offset = 0;
  if( type == PL2EN || type == PHEAL )
    offset = append_to_msg_buffer( source, player_names, offset );
  else if( type == EN2PL )
    offset = append_to_msg_buffer( source, enemy_names, offset );
  else if( type == EFALL ){
    offset = append_to_msg_buffer( source, enemy_names, offset );
    offset = append_to_msg_buffer( 4, combat_text, offset );
    combat_message[offset] = '\0';
    return;
  }else if( type == PWIN ){
    offset = append_to_msg_buffer( 5, combat_text, offset );
    combat_message[offset++] = '\n';
    combat_message[offset++] = ' ';
    combat_message[offset++] = amount/100+'0';
    combat_message[offset++] = amount%100/10+'0';
    combat_message[offset++] = amount%100%10+'0';
    offset = append_to_msg_buffer( 6, combat_text, offset );
    combat_message[offset] = '\0';
    return;
  }
  if( type == PHEAL ){
    offset = append_to_msg_buffer( 3, combat_text, offset );
  }else{
    offset = append_to_msg_buffer( 0, combat_text, offset );
  }
  if( type == PL2EN )
    offset = append_to_msg_buffer( dest, enemy_names, offset );
  else if( type == EN2PL )
    offset = append_to_msg_buffer( dest, player_names, offset );
  offset = append_to_msg_buffer( 1, combat_text, offset );
  combat_message[offset++] = amount/100+'0';
  combat_message[offset++] = amount%100/10+'0';
  combat_message[offset++] = amount%100%10+'0';
  offset = append_to_msg_buffer( 2, combat_text, offset );
  combat_message[offset] = '\0';
}

void draw_menu(byte index){
  gb.display.cursorY = SCREEN_HEIGHT/2-6;
  for(byte i = 0; i < 4; i++){
    gb.display.cursorX = 1;
    gb.display.cursorY+=6;
    if( combat_selection == i ) gb.display.print(F("\20"));
    gb.display.cursorX = 4;
    if( index <= SECONDARY_MENU ){
      copy_to_buffer(index*4+i,menu_text);//Ordinary menus with fixed text
    }else if(index == ENEMY_MENU){//Dynamically generated menus with text for allies/enemies
      combat_buffer[0] = '\0';//If we don't copy anything in, print nothing
      //Menu for selecting an enemy to attack
      /*
          0 1 2 Result
        0 Y ? ? 0
        0 N Y ? 1
        0 N N Y 2
        1 Y Y ? 1
        1 N Y Y 2
        2 Y Y Y 2
      */
      byte yeses = 0;
      for( byte j = 0; j < 3; j++ )
      {
        if( enemies[j] != -1 ){
          yeses++;
          if( yeses > i ){
            copy_to_buffer(enemies[j],enemy_names);
            break;
          }
        }
      }
      /*
      if( i == 0 ){
        if( enemies[0] != -1 ){
          copy_to_buffer(enemies[0],enemy_names);
        }else if( enemies[1] != -1 ){
          copy_to_buffer(enemies[1],enemy_names);
        }else{
          copy_to_buffer(enemies[2],enemy_names);
        }
      }else if( i == 1 ){
        if( enemies[0] != -1 ){
          if( enemies[1] != -1 ){
            copy_to_buffer(enemies[1],enemy_names);
          }else if( enemies[2] !-){
            copy_to_buffer(enemies[1],enemy_names);
          }
        }else if( enemies[1] != -1 ){
          copy_to_buffer(enemies[2],enemy_names);
        }//Otherwise draw nothing
      }else if( i == 2 ){
        if( enemies[2] != -1 ){
          copy_to_buffer(enemies[2],enemy_names);
        }//Otherwise draw nothing
      }*/
    }else if( index == ITEM_MENU ){
      combat_buffer[0] = '\0';//If we don't copy anything in, print nothing
      uint8_t item = 0;
      for( uint8_t j = 0; j < INVENTORY_SIZE; j++ ){
        if( inventory[j] > 0 ) item++;
        if( item == i+1 ){
          copy_to_buffer(j,item_names);
          break;
        }
      }
    }else{
      //TODO: allies menu
    }
    gb.display.print(combat_buffer);
  }
}

void do_combat_step(){
  combat_status[MUDLARK] += mudlark_speed;
  if( shadow_level > 0 ) combat_status[SHADOW] += shadow_speed;
  if( nurse_level > 0 ) combat_status[NURSE] += nurse_speed;
  if( enemies[0] != -1 ) combat_status[ENEMY1] += pgm_read_byte(&enemy_speeds[enemies[0]]);
  if( enemies[1] != -1 ) combat_status[ENEMY2] += pgm_read_byte(&enemy_speeds[enemies[1]]);
  if( enemies[2] != -1 ) combat_status[ENEMY3] += pgm_read_byte(&enemy_speeds[enemies[2]]);

  uint8_t maxi = 0;
  for(uint8_t i = 0; i < 6; i++){
    if( combat_status[i] > combat_status[maxi] ) maxi = i;
  }
  combat_mode = maxi;
  menu_selection = maxi;//Only relevant for player characters?
  combat_status[maxi] = 0;//Reset that unit back to starting value
}

void do_combat(){
  if( combat_mode == PRECOMBAT ){
    gen_enemies();
    for(byte i = 0; i < 6; i++){
      combat_status[i] = -1;//Reset combat status
    }
    do_combat_step();
  }
  
  //Get enemy type
  //graphics hardcoded for now
  gb.display.cursorY = 0;

  if(enemies[0] != -1){
    gb.display.cursorX = 0;
    copy_to_buffer(enemies[0],enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/4-4,SCREEN_HEIGHT/8,&enemybmps[pgm_read_byte(&enemy_imgs[enemies[0]])*18]);
  }
  if(enemies[1] != -1){
    gb.display.cursorX = SCREEN_WIDTH/2-(3*4)-2;
    copy_to_buffer(enemies[1],enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/8,&enemybmps[pgm_read_byte(&enemy_imgs[enemies[1]])*18]);
  }
  if(enemies[2] != -1){
    gb.display.cursorX = SCREEN_WIDTH-(7*4);
    copy_to_buffer(enemies[2],enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/4*3-4,SCREEN_HEIGHT/8,&enemybmps[pgm_read_byte(&enemy_imgs[enemies[2]])*18]);
  }

  //If a message is displayed (dismissing the message steps combat forward)
  if( combat_mode == MESSAGE || combat_mode == VICTORY ){
     gb.display.cursorX = 4;
     gb.display.cursorY = SCREEN_HEIGHT/2;
     gb.display.print(combat_message);
     if(gb.buttons.pressed(BTN_A)){
        byte step_forward = 1;
        gb.sound.playTick();
        if( combat_mode == VICTORY ){
          combat_mode = PRECOMBAT;
          mode = WORLD;
          return;
        }
        //First check if all enemies have died
        if( enemies[0] == -1 && enemies[1] == -1 && enemies[2] == -1 ){
          step_forward = 0;
          combat_mode = VICTORY;
          copy_action_to_msg_buffer(0,0,combat_xp, PWIN);
          //Give mudlark xp (TODO: give other characters xp)
          mudlark_xp+=combat_xp;
          if( mudlark_xp >= mudlark_level*2 ){
            mudlark_xp -= (mudlark_level*2);// Keep previous xp
            mudlark_health += ((mudlark_level+1)*10)-(mudlark_level*10);// Give enough health that a full-health mudlark stays at full health
            mudlark_level++;//Level up!
          }
        }
        //Second do a check whether anyone has died
        for( byte i = 0; i < 3; i++ ){
          if( enemies[i] != -1 && enemy_health[i] == 0 ){
            copy_action_to_msg_buffer(enemies[i],0,0, EFALL);
            combat_status[ENEMY1+i] = -1;
            enemies[i] = -1;//This will break center enemy TODO: support absence of center enemy
            step_forward = 0;
            break;
          }
        }
        if( step_forward )
          do_combat_step();
     }
  }

  //If it is the player's turn
  else if( combat_mode == MUDLARK ){

    gb.display.drawRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+1, SCREEN_WIDTH/2-2, 3);
    gb.display.drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+2, SCREEN_WIDTH/2+((SCREEN_WIDTH/2-4)*mudlark_health)/(mudlark_level*10), SCREEN_HEIGHT/2+2);
    
    gb.display.cursorX = SCREEN_WIDTH-29;//7*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+6;
    gb.display.print(F("MUDLARK"));
    gb.display.cursorX = SCREEN_WIDTH-29;//7*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+12;
    gb.display.print(mudlark_health);
    gb.display.print(F("/"));
    gb.display.print(mudlark_level*10);

    //DEBUG
    //gb.display.print(F(" S"));
    //gb.display.print(mudlark_speed);
    gb.display.cursorX = SCREEN_WIDTH-29;//7*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+18;
    gb.display.print(F("XP"));
    gb.display.print(mudlark_xp);
    gb.display.print(F("L"));
    gb.display.print(mudlark_level);

    draw_menu(menu_selection);
    
    if(gb.buttons.pressed(BTN_UP)){
      gb.sound.playTick();
      combat_selection--;
    }else if(gb.buttons.pressed(BTN_DOWN)){
      gb.sound.playTick();
      combat_selection++;
    }else if(gb.buttons.pressed(BTN_A)){
      gb.sound.playOK();
      if(menu_selection == MUDLARK_MENU && combat_selection == 0){
        menu_selection = ENEMY_MENU;
        combat_selection = 0;
      }else if( menu_selection == MUDLARK_MENU && combat_selection == 3 ){
        menu_selection = SECONDARY_MENU;
        combat_selection = 0;
      }else if( menu_selection == SECONDARY_MENU ){
        if( combat_selection == 0 ){ // ITEM
          menu_selection = ITEM_MENU;
          combat_selection = 0;
        }else{ // 1 == RUN
          if( random(2) == 0 ){
            combat_mode = PRECOMBAT;
            mode = WORLD;
          }else{
            combat_message[append_to_msg_buffer( 7, combat_text, 0 )] = 0;
            combat_mode = MESSAGE;
          }
        }
      }else if( menu_selection == ENEMY_MENU ){
        uint8_t damage = calculate_damage(mudlark_level)*2;//Mudlark does double base damage
        if( combat_selection == 1 && enemies[0] == -1 ){
          combat_selection = 2;//Must be the third one
        }else{
          for( ; combat_selection < 3; combat_selection++ ){
            if( enemies[combat_selection] != -1 ){
              break;  // As soon as we find an existing enemy, that's the one that was selected
            }
          }
        }
        copy_action_to_msg_buffer(0,enemies[combat_selection],damage, PL2EN);
        if( damage > enemy_health[combat_selection] ){
          enemy_health[combat_selection] = 0;
        }else{
          enemy_health[combat_selection]-=damage;
        }
        combat_mode = MESSAGE;
      }else if( menu_selection == ITEM_MENU ){
        uint8_t item = 0;
        for( uint8_t i = 0; i < INVENTORY_SIZE; i++ ){
          if( inventory[i] != 0 ) item++;
          if( item == combat_selection+1 ){
            item = i;
            break;
          }
        }
        if( item == 0 ){
          mudlark_health+=5;
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,5, PHEAL);
        }else if( item == 1 ){
          mudlark_health+=10;
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,10, PHEAL);
        }
        combat_mode = MESSAGE;
      }
    }else if(gb.buttons.pressed(BTN_B)){
      gb.sound.playCancel();
      



      //This is for debugging  TODO: remove
      if( menu_selection == MUDLARK_MENU ){
        combat_mode = PRECOMBAT;
        mode = WORLD;
      }




      if( menu_selection == ENEMY_MENU || menu_selection == SECONDARY_MENU ){
        menu_selection = MUDLARK_MENU;
        combat_selection = 0;
      }else if( menu_selection == ITEM_MENU ){
        menu_selection = SECONDARY_MENU;
        combat_selection = 0;
      }
    }
    byte mod = 4;
    if( menu_selection == ENEMY_MENU ){
      mod--;
      for( byte i = 0; i < 3; i++ ){
        if( enemies[i] == -1 ){
          mod--;
        }
      }
    }else if( menu_selection == SECONDARY_MENU ){
      mod = 2;
    }else if( menu_selection == ITEM_MENU ){
      uint8_t item = 0;
      for( uint8_t i = 0; i < INVENTORY_SIZE; i++ ){
        if( inventory[i] != 0 ) item++;
      }
      if( item < 4 ) mod = item; 
    }
    combat_selection%=mod;
  }

  else if( combat_mode == SHADOW ){
    gb.display.cursorX = SCREEN_WIDTH-29;//7*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+6;
    gb.display.print(F("SHADOW?"));
    gb.display.cursorX = SCREEN_WIDTH-29;//7*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+12;
    gb.display.print(shadow_level);
  }

  //If it is an enemy's turn
  else if( combat_mode >= ENEMY1 && combat_mode <= ENEMY3 ){
    //if(gb.buttons.pressed(BTN_A)){
    //For now only target Mudlark.  TODO: random targeting
    uint8_t damage = calculate_damage(pgm_read_byte(&enemy_levels[enemies[combat_mode-ENEMY1]]));
    copy_action_to_msg_buffer(enemies[combat_mode-ENEMY1],0,damage, EN2PL);
    if( damage > mudlark_health ){
      mudlark_health = 0;
    }else{
      mudlark_health-=damage;
    }
    combat_mode = MESSAGE;
    //}
  }

  //DEBUG TODO: remove
  gb.display.cursorX = 0;
  gb.display.cursorY = 6;
  gb.display.print((char)(combat_mode+'0'));
  gb.display.print(' ');
  gb.display.print((char)(mudlark_level+'0'));
  gb.display.print(' ');
  gb.display.print(combat_xp);
  gb.display.cursorX = 0;
  gb.display.cursorY = 12;
  for( uint8_t i = 0; i < 6; i++ ){
    gb.display.print((char)(combat_status[i]+'1'));
  }
  gb.display.cursorX = 0;
  gb.display.cursorY = 18;
  for( uint8_t i = 0; i < 3; i++ ){
    gb.display.print((char)(enemies[i]+'1'));
  }

  gb.display.drawRect(0,SCREEN_HEIGHT/2-1,SCREEN_WIDTH,SCREEN_HEIGHT/2+1);
  
  
}
#endif