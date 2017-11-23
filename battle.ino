//Scamp: Level 1
//Ruffian: Level 2
//Scoundrel: Level 4
//Thief (or maybe Shadow) recruits you on his quest
#include "battle.h"
#include "dungeon.h"

uint8_t meta_mode = WORLD; //Mode that we came from, either WORLD or DUNGEON

struct enemy{
  int8_t lvl;
  uint8_t spd;
  uint8_t img;
  uint8_t nme;
};

struct enemy enemy_buffer[3];//Store data for enemies to battle


struct min_enemy{
  int8_t lvl;
  uint8_t spd;
  uint8_t img;
};

//This is not efficient, because there is always an
//entry for the enemy name even though every enemy has
//a unique name and I don't want two different enemies 
//with the same name.  However, the way I copy names into
//the buffer uses a generic approach that always copies from an
//array of 8-char strings in progmem, and I don't want to change all
//that code for a special case with the name!  Therefore, this
//approach is a bit inefficient because refactoring is a huge chore.
const struct min_enemy enemies[] PROGMEM = {
  {1,5,1}, //RAT
  {1,4,0}, //SCAMP
  {2,3,0}, //RUFFIAN
  {4,2,0}, //THUG
  {4,5,1}, //BIG RAT
  {5,4,0}, //PATRON
  {5,5,2}, //BOUNCER
  {10,5,0}, //SLAVER (boss)
  {8,6,1}, //SEA RAT
  {9,4,0}, //SWABBIE
  {11,3,0}, //SAILOR
  {10,5,0}, //SKIPPER
  {20,4,0}, //CAPTAIN (boss)
  {14,7,1}, //BAD RAT
  {15,5,0}, //WATCHER
  {19,3,2}, //BRUISER (boss/enemy)
  {24,2,2}, //MUSCLER (boss/enemy)
  //{25,3,2},  //Remove OVERMAN
  {25,4,0}, //DOCTOR
  {26,5,0}, //PATIENT
  {26,6,0}, //SUBJECT
  {26,7,0}, //MUTANT
  {32,8,0}, //MADMAN (boss)
  {30,7,1}, //WOW RAT
  {34,3,0}, //SNOB
  {37,2,0}, //RICHMAN
  {42,8,1}, //MAX RAT (boss)
  {40,4,0}, //GUARD
  {45,2,0}, //GOLEM
  {42,5,0}, //OFFICER
  {50,8,0}, //LEADER (boss)
  {1,2,3}, //CRAB
  {10,7,0}, //SHADOW (boss)
};

const char enemy_names[][8] PROGMEM = {
"RAT",
"SCAMP",
"RUFFIAN",
"THUG",
"BIG RAT",
"PATRON",
"BOUNCER",
"SLAVER",
"SEA RAT",
"SWABBIE",
"SAILOR",
"SKIPPER",
"CAPTAIN",
"BAD RAT",
"WATCHER",
"BRUISER",
"MUSCLER",
//"OVERMAN",
"DOCTOR",
"PATIENT",
"SUBJECT",
"MUTANT",
"MADMAN",
"WOW RAT",
"SNOB",
"RICHMAN",
"MAX RAT",
"GUARD",
"GOLEM",
"OFFICER",
"LEADER",
"CRAB",
"SHADOW"
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
8,16,                   // Crab
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B01000010,
B10000001,
B11000011,
B10000001,
B10111101,
B01000010,
B10101001,
B10000001,
B01111110,
B00100100,
};

const uint8_t world_spawns[4][4][3] PROGMEM = {
  {
    // Top far left (Rich District)
    {
      // Wow Rat, Snob, Richman
      WOW_RAT,SNOB,RICHMAN
    },
    // Top center left (Right Rich District)
    {
      WOW_RAT,SNOB,RICHMAN
    },
    // Top center right (Top Left Medical District
    {
      WOW_RAT,PATIENT,SUBJECT
    },
    // Top far right (Top Right Medical District)
    {
      WOW_RAT,SUBJECT,MUTANT
    }
  },
  {
    // Mid top far left (Red Light District)
    {
      RAT,PATRON,BOUNCER
    },
    // Mid top center left (TOWER DISTRICT)
    {
      BAD_RAT,BRUISER,MUSCLER //Problem here is that a tiny bit of this area is in a much earlier place
    },
    // Mid top center right (Bottom Left Medical District)
    {
      WATCHER,BRUISER,DOCTOR
    },
    // Mid top far right (Bottom Right Medical District)
    {
      MUSCLER,DOCTOR,PATIENT
    }
  },
  {
    // Mid bottom far left (Left Factory District)
    {
      // Rat, Ruffian, Thug
      RAT,RUFFIAN,THUG
    },
    // Mid bottom center left (Right Factory District)
    {
      // Big Rat, Ruffian, Thug
      BIG_RAT,RUFFIAN,THUG
    },
    // Mid bottom center right (Top Left Dock District)
    {
      // SEA_RAT, SKIPPER, WATCHER
      SEA_RAT,SKIPPER,WATCHER
    },
    // Mid bottom far right (Top Right Dock District)
    {
      // Sea Rat, Sailor, Skipper
      SEA_RAT,SAILOR,SKIPPER
    }
  },
  {
    // Bottom far left (Left Sewer District)
    {
      // Rat, Scamp, Thug
      RAT,SCAMP,RUFFIAN
    },
    // Bottom center left (Right Sewer District)
    {
      // Rat, Scamp, Ruffian
      RAT,CRAB,SCAMP
    },
    // Bottom center right (Bottom Left Dock District)
    {
      // Sea rat, Swabbie, Sailor
      SEA_RAT,SWABBIE,SAILOR
    },
    // Bottom far right (Bottom Right Dock District)
    {
      // Sea Rat, Sailor, Skipper
      SEA_RAT,SAILOR,SKIPPER
    }
  }
};

//Stats:
//Level (determines max health + base damage)
//Health
//Speed

const char player_names[][8] PROGMEM = {
"MUDLARK",
"SHADOW",
"NURSE",
"GIRL", //Not actually playable characters, but I needed the text somewhere....
"FATHER",
"",
"MAN",
};

struct character party[3] = {
  {1,20,4,0,0,0,0}, // Mudlark
  {0,160,3,0,0,0,0}, // Shadow
  {0,200,4,0,0,0,0} // Nurse
};

uint8_t shadow_stealth_bonus = 0;
int8_t nurse_protect_bonus = -1;// -1 means she is not protecting anyone

//Abilities:
//Mudlark: Rally, scavenge
//Nurse: Heal one, heal all
//Thief: Sneak, Speed up

//TODO: More effects, more items
#define HEAL 0

#define ITEM_FRUIT 0
#define ITEM_BREAD 1
#define ITEM_MEAT 2
#define ITEM_TONIC 3
#define ITEM_TEA 4
#define ITEM_LIQUOR 5

const char item_names[][8] PROGMEM = {
  "FRUIT",  // 10% HP
  "BREAD",  // 15% HP
  "MEAT",    // 1 damage increase 
  "TONIC",  // 50% HP
  "TEA",    // 1 SP
  "LIQUOR", // 1 damage resist       
};

#define INVENTORY_MAX 8

unsigned char inventory[6] = {2,1,0,0,0,1};//Each element == how much of each item

uint8_t next_combat = 32;

void try_combat(){
  next_combat--;
  if( next_combat == 0 ){
    meta_mode = mode; //Remember the mode we came from
    mode = TO_COMBAT;
    transition = -SCREEN_HEIGHT/2;
    gb.display.persistence = true;
    next_combat = random(192)+64;//32-256 steps
  }
}

const char menu_text[][8] PROGMEM = {
  "FLAIL",
  "RALLY",
  "SEARCH",
  "OTHER",
  "STRIKE",
  "VANISH", // Perhaps SNEAK or LURK?
  "HASTE",
  "OTHER",
  "HP ONE",
  "HP ALL",
  "PROTECT",
  "OTHER",
  "FOOD",
  "DRINK",
  "RUN",
  "",//TODO: optimize these out
  };

const char combat_text[][8] PROGMEM = {
  " HIT ",
  "\n FOR ",
  " DAMAGE",
  " HEAL ",
  " FALLS",
  "YOU WIN",
  " XP GET",
  "TRAPPED",
  " SPD UP",
  " FOUND\n",
  "\n GIVE ",
  "TO WHO?",
  "PROTECT",
  " DMG UP",
  " DEF UP",
  "ALL",
  "BAG MAX",
  " USES ",
};

#define MUDLARK_MENU 0
#define SHADOW_MENU 1
#define NURSE_MENU 2
#define SECONDARY_MENU 3
#define ENEMY_MENU 4
#define ALLY_MENU 5
#define FOOD_MENU 6
#define DRINK_MENU 7
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
#define POSTCOMBAT 8 // Used for allocating xp to allies
#define DEFEAT 9

char combat_mode = PRECOMBAT;
byte menu_selection = 0;
byte combat_selection = 0;
char combat_buffer[8] = "\0\0\0\0\0\0\0";
char combat_message[64];//Max of 64 characters in message, which should be plenty
int8_t combat_status[6] = {0,0,0,0,0,0};//Determines who moves next
uint8_t combat_xp = 0;

uint16_t enemy_health[3];

uint8_t calculate_damage(uint8_t lvl){
  return lvl*10/8;
}

void load_enemy_data(uint8_t index, uint8_t slot){
  uint8_t en_ind;
  const struct min_enemy* en;
  if( meta_mode == WORLD ){
    //Use dudex and dudey to determine which pool to spawn enemies from
    en_ind = pgm_read_byte(&(world_spawns[dudey/8/16][dudex/8/16][index]));
    en = &(enemies[en_ind]);
  }else if( meta_mode == DUNGEON ){
    //Use dungeon data to determine pool to spawn enemies from
    en_ind = pgm_read_byte(&(dungeons[dungeonid].spawns[index]));
    en = &(enemies[en_ind]);
  }else{
    //If meta_mode actually contains the boss data
    //It's always the center the first time it's called
    //if( slot == 1 ){ //If it's the center enemy
      en_ind = meta_mode;
      en = &(enemies[meta_mode]);
      meta_mode = DUNGEON;//This will be used to return to the dungeon now
    //}
    /*else{
      //If it's not the center enemy, we don't want it
      enemy_buffer[slot].lvl = -1;
      combat_xp++;//Compensate for loss in combat_xp
      return;//Skip setting enemy_buffer
    }*/
  }
  enemy_buffer[slot].lvl = pgm_read_byte(&(en->lvl));
  enemy_buffer[slot].spd = pgm_read_byte(&(en->spd));
  enemy_buffer[slot].img = pgm_read_byte(&(en->img));
  enemy_buffer[slot].nme = en_ind;
}

void gen_enemies(){
  uint8_t enemy_index = 0;
  combat_xp = 0;
  //Always generate center enemy
  enemy_index = random(3);
  load_enemy_data(enemy_index,1);
  combat_xp+=enemy_buffer[1].lvl * 2;
  enemy_health[1] = enemy_buffer[1].lvl*10;
  //50% chance of left enemy
  if( random(2) == 0 ){
    enemy_index = random(3);
    load_enemy_data(enemy_index,0);
    combat_xp+=enemy_buffer[0].lvl * 2;
    enemy_health[0] = enemy_buffer[0].lvl*10;
  }else{
    enemy_buffer[0].lvl = -1;
  }
  //50% chance of right enemy
  if( random(2) == 0 ){
    enemy_index = random(3);
    load_enemy_data(enemy_index,2);
    combat_xp+=enemy_buffer[2].lvl * 2;
    enemy_health[2] = enemy_buffer[2].lvl*10;
  }else{
    enemy_buffer[2].lvl = -1;
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
#define PSPEED 5
#define PITEM 6
#define PROTECT 7
#define PDAMAGE 8
#define PDEFENSE 9
#define HEALALL 10
#define PFALL 11

//TODO: space optimize this
void copy_action_to_msg_buffer(uint8_t source, uint8_t dest, uint8_t amount, uint8_t type){
  uint8_t offset = 0;
  if( type == PL2EN || type == PHEAL || type == PSPEED || type == PITEM || type == PDAMAGE || type == PDEFENSE )
    offset = append_to_msg_buffer( source, player_names, offset );
  else if( type == EN2PL || type == PROTECT ){
    if( type == PROTECT ){
      //NURSE PROTECT <PARTY MEMBER>
      //offset = append_to_msg_buffer( NURSE, player_names, offset );
      //combat_message[offset++] = ' ';
      offset = append_to_msg_buffer( 12, combat_text, offset );
      combat_message[offset++] = ' ';
      offset = append_to_msg_buffer( dest, player_names, offset );
      combat_message[offset++] = '\n';
      combat_message[offset++] = ' ';
      dest = NURSE;// Nurse takes the damage instead
    }
    offset = append_to_msg_buffer( source, enemy_names, offset );
  }else if( type == EFALL ){
    offset = append_to_msg_buffer( source, enemy_names, offset );
    offset = append_to_msg_buffer( 4, combat_text, offset );
    combat_message[offset] = '\0';
    return;
  }else if( type == PFALL ){
    offset = append_to_msg_buffer( source, player_names, offset );
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
    // If we have more than one party member, ask who to give xp to
    if( party[SHADOW].level != 0 || party[NURSE].level != 0 ){ 
      offset = append_to_msg_buffer( 10, combat_text, offset );
      offset = append_to_msg_buffer( 11, combat_text, offset );
    }
    combat_message[offset] = '\0';
    return;
  }else if( type == HEALALL ){
    offset = append_to_msg_buffer( 15, combat_text, offset );
  }
  if( type == PHEAL || type == HEALALL ){
    offset = append_to_msg_buffer( 3, combat_text, offset );
  }else if( type == PSPEED ){
    offset = append_to_msg_buffer( 8, combat_text, offset );
  }else if( type == PDAMAGE ){
    offset = append_to_msg_buffer( 13, combat_text, offset );
  }else if( type == PDEFENSE ){
    offset = append_to_msg_buffer( 14, combat_text, offset );
  }else if( type == PITEM ){
    offset = append_to_msg_buffer( 9, combat_text, offset );
    combat_message[offset++] = ' ';
    offset = append_to_msg_buffer( amount, item_names, offset );
    //Check if player has more than max of item in inventory.
    //If so, we must say that the mudlark consumes it immediately.
    if( inventory[amount] > INVENTORY_MAX ){
      combat_message[offset++] = '\n';
      combat_message[offset++] = ' ';
      offset = append_to_msg_buffer( 16, combat_text, offset );
      combat_message[offset++] = '\n';
      combat_message[offset++] = ' ';
      offset = append_to_msg_buffer( 0, player_names, offset ); //MUDLARK
      offset = append_to_msg_buffer( 17, combat_text, offset ); // USES 
      offset = append_to_msg_buffer( amount, item_names, offset );//<item>
    }
  }else{
    offset = append_to_msg_buffer( 0, combat_text, offset );
  }
  if( type == PL2EN )
    offset = append_to_msg_buffer( dest, enemy_names, offset );
  else if( type == EN2PL || type == PROTECT )
    offset = append_to_msg_buffer( dest, player_names, offset );
  if( type != PSPEED && type != PDAMAGE && type != PDEFENSE && type != PITEM ){
    offset = append_to_msg_buffer( 1, combat_text, offset );
    combat_message[offset++] = amount/100+'0';
    combat_message[offset++] = amount%100/10+'0';
    combat_message[offset++] = amount%100%10+'0';
    offset = append_to_msg_buffer( 2, combat_text, offset );
    //If shadow was struck and had an active bonus, remove it and say so
    if( type == EN2PL && dest == SHADOW && shadow_stealth_bonus > 0 )
    {
      shadow_stealth_bonus = 0;
      combat_message[offset++] = '\n';
      combat_message[offset++] = ' ';
      offset = append_to_msg_buffer( SHADOW, player_names, offset );
      offset = append_to_msg_buffer( 9, combat_text, offset );
    }
  }
  combat_message[offset] = '\0';
}

void draw_menu(byte index){
  gb.display.cursorY = SCREEN_HEIGHT/2-6;
  uint8_t offset = 0;
  for(byte i = 0; i < 4; i++){
    gb.display.cursorX = 1;
    gb.display.cursorY+=6;
    if( combat_selection == i ) gb.display.print(F("\20"));
    gb.display.cursorX = 4;
    combat_message[0] = '\0';//If we don't copy anything in, print nothing
    if( index <= SECONDARY_MENU ){
      offset = append_to_msg_buffer(index*4+i,menu_text,0);//Ordinary menus with fixed text
    }else if(index == ENEMY_MENU){//Dynamically generated menus with text for allies/enemies
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
        if( enemy_buffer[j].lvl != -1 ){
          yeses++;
          if( yeses > i ){
            offset = append_to_msg_buffer(enemy_buffer[j].nme,enemy_names,0);
            break;
          }
        }
      }
    }else if( index == FOOD_MENU || index == DRINK_MENU ){
      uint8_t item = 0;
      uint8_t j = index == FOOD_MENU ? 0 : 3;
      for( ; j < (index == FOOD_MENU ? INVENTORY_SIZE/2: INVENTORY_SIZE); j++ ){
        if( inventory[j] > 0 ) item++;
        if( item == i+1 ){
          offset = append_to_msg_buffer(j,item_names,0);
          combat_message[offset++] = 'x';
          combat_message[offset++] = inventory[j]/10+'0';
          combat_message[offset++] = inventory[j]%10+'0';
          break;
        }
      }
    }else{  //Allies menu (ALLY_MENU)
      if( i < 3 && party[i].level != 0 ){
        offset = append_to_msg_buffer(i,player_names,0);
      }
    }
    combat_message[offset] = '\0';
    gb.display.print(combat_message);
  }
}

void give_xp(){
  //Give xp to chosen character
  party[combat_selection].xp+=combat_xp;
  if( party[combat_selection].xp >= party[combat_selection].level*2 ){
    party[combat_selection].xp -= (party[combat_selection].level*2);// Keep previous xp
    party[combat_selection].health += ((party[combat_selection].level+1)*20)-(party[combat_selection].level*20);// Give enough health that full health stays at full health
    party[combat_selection].level++;//Level up!
  }
  combat_mode = PRECOMBAT;
  mode = meta_mode; //Switch back to previous mode, either dungeon or overworld
}

void do_combat_step(){
  uint8_t i;
  for( i = 0; i < 3; i++ ){
    // If a party member is present, add their speed
    if( party[i].level != 0 ){
      combat_status[i] += party[i].speed + party[i].bonus_speed;
    }
    // If an enemy is present, add their speed
    if( enemy_buffer[i].lvl != -1 ){
      combat_status[i+ENEMY1] += enemy_buffer[i].spd;
    }
  }

  uint8_t maxi = 0;
  for(i = 0; i < 6; i++){
    if( combat_status[i] > combat_status[maxi] ) maxi = i;
  }
  //If nurse was protecting anyone, reset this as her turn starts.
  if( maxi == NURSE ) nurse_protect_bonus = -1;
  combat_mode = maxi;
  combat_selection = 0;//Force the first item to be selected by default
  menu_selection = maxi;//Only relevant for player characters?
  combat_status[maxi] = 0;//Reset that unit back to starting value
}

void do_combat(){
  if( combat_mode == PRECOMBAT ){
    gen_enemies();
    for(uint8_t i = 0; i < 6; i++){
      combat_status[i] = -1;//Reset combat status
      // Reset party members' temporary bonuses
      if( i < 3 ){
        party[i].bonus_speed = 0;
        party[i].bonus_damage = 0;
        party[i].bonus_defense = 0;
      }
    }
    shadow_stealth_bonus = 0;
    nurse_protect_bonus = -1;
    do_combat_step();
  }
  
  //Get enemy type
  //graphics hardcoded for now
  gb.display.cursorY = 0;

  if(enemy_buffer[0].lvl != -1){
    gb.display.cursorX = 0;
    copy_to_buffer(enemy_buffer[0].nme,enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/4-4,SCREEN_HEIGHT/8,&enemybmps[enemy_buffer[0].img*18]);
  }
  if(enemy_buffer[1].lvl != -1){
    gb.display.cursorX = SCREEN_WIDTH/2-(3*4)-2;
    copy_to_buffer(enemy_buffer[1].nme,enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/8,&enemybmps[enemy_buffer[1].img*18]);
  }
  if(enemy_buffer[2].lvl != -1){
    gb.display.cursorX = SCREEN_WIDTH-(7*4);
    copy_to_buffer(enemy_buffer[2].nme,enemy_names);
    gb.display.print(combat_buffer);
    gb.display.drawBitmap(SCREEN_WIDTH/4*3-4,SCREEN_HEIGHT/8,&enemybmps[enemy_buffer[2].img*18]);
  }

  //If a message is displayed (dismissing the message steps combat forward)
  if( combat_mode == MESSAGE || combat_mode == VICTORY || combat_mode == DEFEAT ){
     gb.display.cursorX = 4;
     gb.display.cursorY = SCREEN_HEIGHT/2;
     gb.display.print(combat_message);
     if(gb.buttons.pressed(BTN_A)){
        byte step_forward = 1;
        gb.sound.playTick();
        if( combat_mode == VICTORY ){
          //If there is more than one party member, ask who to give xp to
          if( party[SHADOW].level > 0 ){
            combat_mode = POSTCOMBAT;
            menu_selection = ALLY_MENU;
          }else{
            combat_selection = 0;// Select mudlark to give xp to
            give_xp();
          }
          return;
        }
        if( combat_mode == DEFEAT ){
          combat_mode = PRECOMBAT;
          mode = GAME_OVER;
          return;
        }
        //First check if all enemies have died
        if( enemy_buffer[0].lvl == -1 && enemy_buffer[1].lvl == -1 && enemy_buffer[2].lvl == -1 ){
          step_forward = 0;
          combat_mode = VICTORY;
          copy_action_to_msg_buffer(0,0,combat_xp, PWIN);
        }
        //Second do a check whether anyone has died
        for( byte i = 0; i < 3; i++ ){
          if( enemy_buffer[i].lvl != -1 && enemy_health[i] == 0 ){
            copy_action_to_msg_buffer(enemy_buffer[i].nme,0,0, EFALL);
            combat_status[ENEMY1+i] = -1;
            enemy_buffer[i].lvl = -1;//This will break center enemy TODO: support absence of center enemy
            step_forward = 0;
            break;
          }
          //Loss condition---if any party member is dead
          if( party[i].health == 0 ){
            copy_action_to_msg_buffer(i,0,0,PFALL);
            step_forward = 0;
            combat_mode = DEFEAT;
            break;
          }
        }
        if( step_forward )
          do_combat_step();
     }
  }

  //If it is the player's turn (one of 3 party members or post-combat xp allocation)
  else if( (combat_mode >= MUDLARK && combat_mode <= NURSE) || combat_mode == POSTCOMBAT ){

    if( combat_mode >= MUDLARK && combat_mode <= NURSE ){
      gb.display.drawRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+1, SCREEN_WIDTH/2-2, 3);
      gb.display.drawLine(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+2, SCREEN_WIDTH/2+((SCREEN_WIDTH/2-4)*party[combat_mode].health)/(party[combat_mode].level*20), SCREEN_HEIGHT/2+2);
      
      gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
      gb.display.cursorY = SCREEN_HEIGHT/2+6;
      copy_to_buffer(combat_mode,player_names);
      gb.display.print(combat_buffer);
      gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
      gb.display.cursorY = SCREEN_HEIGHT/2+12;
      gb.display.print(party[combat_mode].health);
      gb.display.print(F("/"));
      gb.display.print(party[combat_mode].level*20);
  
      gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
      gb.display.cursorY = SCREEN_HEIGHT/2+18;
      gb.display.print(F("XP"));
      gb.display.print(party[combat_mode].xp);
      gb.display.print(F("L"));
      gb.display.print(party[combat_mode].level);
    }

    draw_menu(menu_selection);
    
    if(gb.buttons.pressed(BTN_UP)){
      gb.sound.playTick();
      combat_selection--;
    }else if(gb.buttons.pressed(BTN_DOWN)){
      gb.sound.playTick();
      combat_selection++;
    }else if(gb.buttons.pressed(BTN_A)){
      gb.sound.playOK();
      // Flail, Strike
      if( (menu_selection == MUDLARK_MENU || menu_selection == SHADOW_MENU) && combat_selection == 0){
        menu_selection = ENEMY_MENU;
        combat_selection = 0;
      }else if( menu_selection == NURSE_MENU && combat_selection == 0){
        //Nurse's HEAL ONE ability
        //Signal that we are going to heal, not protect, by resetting bonus (in case player entered protect menu and then cancelled)
        nurse_protect_bonus = -1;
        menu_selection = ALLY_MENU;
        combat_selection = 0;
      }else if( menu_selection == MUDLARK_MENU && combat_selection == 1){
        //Mudlark's RALLY ability; select an ally to boost damage of
        menu_selection = ALLY_MENU;
        combat_selection = 0;
      }else if( menu_selection == SHADOW_MENU && combat_selection == 1){
        //Go into stealth, get damage bonus unless hit
        shadow_stealth_bonus++;
        //Say that shadow is sneaking
        combat_message[append_to_msg_buffer( 5, menu_text, 0 )] = 0;
        combat_mode = MESSAGE;
      }else if( menu_selection == NURSE_MENU && combat_selection == 1 ){
        uint16_t healing = 2*party[NURSE].level; // Heal 10% of NURSE's health for everyone
        // Heal an extra amount by (5% of original healing + 1) * bonus_damage
        // Therefore, the nurse's damage bonus is used for healing since she has no attack. 
        healing += (healing/10/2 + 1)*party[NURSE].bonus_damage;
        for( uint8_t i = 0; i < 3; i++ ){
          party[i].health+=healing;
          if( party[i].health > party[i].level*20 ) party[i].health = party[i].level*20;//Cap off healing
        }
        copy_action_to_msg_buffer(0,0,healing, HEALALL);
        // TODO: Display how much all heal by, now that the heal amount is unified by the nurse's level
        combat_mode = MESSAGE;
      }else if( menu_selection == MUDLARK_MENU && combat_selection == 2 ){ //Scavenge
        if( random(100) < 30 ){ //Highest chance to get fruit
          inventory[ITEM_FRUIT]++;
          copy_action_to_msg_buffer(0,0,ITEM_FRUIT, PITEM);
          //If the inventory slot is full, reset to max and consume by mudlark immediately
          if( inventory[ITEM_FRUIT] > INVENTORY_MAX ){
            inventory[ITEM_FRUIT] = INVENTORY_MAX;
            party[MUDLARK].health+=2*party[MUDLARK].level;//Heal 10%
          }
        }else if( random(100) < 30 ){ //Second highest chance to get bread
          inventory[ITEM_BREAD]++;
          copy_action_to_msg_buffer(0,0,ITEM_BREAD, PITEM);
          //If the inventory slot is full, reset to max and consume by mudlark immediately
          if( inventory[ITEM_BREAD] > INVENTORY_MAX ){
            inventory[ITEM_BREAD] = INVENTORY_MAX;
            party[MUDLARK].health+=3*party[MUDLARK].level;//Heal 15%
          }
        }else{ //All the rest of the items have an equal chance
          uint8_t chosen = random(4) + 2;
          inventory[chosen]++;
          copy_action_to_msg_buffer(0,0, chosen, PITEM);
          //If the inventory slot is full, reset to max and consume by mudlark immediately
          if( inventory[chosen] > INVENTORY_MAX ){
            inventory[chosen] = INVENTORY_MAX;
            if( chosen == ITEM_MEAT ){
              party[MUDLARK].bonus_damage+=4; // Meat increases damage
            }else if( chosen == ITEM_TONIC ){
              party[MUDLARK].health+=10*party[MUDLARK].level;//Heal 50%
            }else if( chosen == ITEM_TEA ){
              party[MUDLARK].bonus_speed+=4;//Speed up
            }else if( chosen == ITEM_LIQUOR ){
              party[MUDLARK].bonus_defense+=2;// Liquor increases damage resistance
            }
          }
        }
        if( party[MUDLARK].health > party[MUDLARK].level*20 ) party[MUDLARK].health = party[MUDLARK].level*20;//Cap off healing
        combat_mode = MESSAGE;
      }else if( menu_selection == SHADOW_MENU && combat_selection == 2 ){ //Shadow's speed boost
        party[SHADOW].bonus_speed+=3;//Speed up by 3!
        copy_action_to_msg_buffer(SHADOW,0,1, PSPEED);
        combat_mode = MESSAGE;
      }else if( menu_selection == NURSE_MENU && combat_selection == 2 ){ //Nurse's protect
        nurse_protect_bonus = 4;// Signal that we are going to protect, not heal
        menu_selection = ALLY_MENU;
        combat_selection = 0;
      }else if( (menu_selection >= MUDLARK_MENU && menu_selection <= NURSE_MENU ) && combat_selection == 3 ){
        menu_selection = SECONDARY_MENU;
        combat_selection = 0;
      }else if( menu_selection == SECONDARY_MENU ){
        if( combat_selection == 0 ){ // FOOD
          //If the player has any food to eat, only then show the menu
          if( inventory[0] > 0 || inventory[1] > 0 || inventory[2] > 0 ){
            menu_selection = FOOD_MENU;
            combat_selection = 0;
          }
        }else if( combat_selection == 1 ){ // DRINK
          //If the player has any drinks to drink, only then show the menu
          if( inventory[3] > 0 || inventory[4] > 0 || inventory[5] > 0 ){
            menu_selection = DRINK_MENU;
            combat_selection = 0;
          }
        }else{ // 1 == RUN
          if( random(2) == 0 ){
            combat_mode = PRECOMBAT;
            mode = meta_mode;
          }else{
            combat_message[append_to_msg_buffer( 7, combat_text, 0 )] = 0;
            combat_mode = MESSAGE;
          }
        }
      }else if( menu_selection == ENEMY_MENU ){
        uint16_t damage = calculate_damage(party[combat_mode].level);// Base damage
        if( combat_mode == MUDLARK ) damage*=2;//Mudlark does double base damage
        if( combat_mode == SHADOW ) damage*=(shadow_stealth_bonus+1);//Rapidly increase stealth bonus
        // Increase damage by (5% of original damage + 1) * bonus_damage
        damage += (damage/10/2 + 1)*party[combat_mode].bonus_damage;
        if( combat_selection == 1 && enemy_buffer[0].lvl == -1 ){
          combat_selection = 2;//Must be the third one
        }else{
          for( ; combat_selection < 3; combat_selection++ ){
            if( enemy_buffer[combat_selection].lvl != -1 ){
              break;  // As soon as we find an existing enemy, that's the one that was selected
            }
          }
        }
        copy_action_to_msg_buffer(combat_mode,enemy_buffer[combat_selection].nme,damage, PL2EN);
        if( damage > enemy_health[combat_selection] ){
          enemy_health[combat_selection] = 0;
        }else{
          enemy_health[combat_selection]-=damage;
        }
        combat_mode = MESSAGE;
      }else if( menu_selection == ALLY_MENU ){
        //Assume mudlark is always slot 0, shadow is always slot 1, and nurse is always slot 2
        //TODO: This assumption will cause problems if we change in the future to have, say, the nurse
        //but not the shadow in the party.
        // Check if this is post-combat xp allocation or nurse ability
        if( combat_mode == POSTCOMBAT ){
          give_xp();
          return;
        }else if( combat_status[MUDLARK] == 0 ){ // If it is the mudlark's turn
          party[combat_selection].bonus_damage+=2;
          copy_action_to_msg_buffer(combat_selection,0,1, PDAMAGE);
          combat_mode = MESSAGE;
        }else if( nurse_protect_bonus == -1 ){ // If we are not trying to select who to protect, and instead heal
          //Nurse's HEAL ONE ability
          //Remember, this assumes that we are not missing a character---if I add the ability to 
          //revive fallen party members (vs an instant game over) then this will need to change...
          uint16_t healing = 4*party[NURSE].level;// Heal 20% of NURSE's health
          // Heal an extra amount by (5% of original healing + 1) * bonus_damage
          // Therefore, the nurse's damage bonus is used for healing since she has no attack. 
          healing += (healing/10/2 + 1)*party[NURSE].bonus_damage;
          party[combat_selection].health += healing;
          if( party[combat_selection].health > party[combat_selection].level*20 ){
            //Cap off health
            party[combat_selection].health = party[combat_selection].level*20;
          }
          copy_action_to_msg_buffer(combat_selection,0,healing, PHEAL);
          combat_mode = MESSAGE;
        }else{ // Protect selected party member
          nurse_protect_bonus = combat_selection;
          uint8_t offset;
          offset = append_to_msg_buffer( 12, combat_text, 0 );
          combat_message[offset++] = ' ';
          offset = append_to_msg_buffer( combat_selection, player_names, offset );
          combat_message[offset++] = '\0';
          combat_mode = MESSAGE;
        }
      }else if( menu_selection == FOOD_MENU || menu_selection == DRINK_MENU ){
        uint8_t item = 0;
        for( uint8_t i = 0; i < INVENTORY_SIZE/2; i++ ){
          if( inventory[menu_selection == FOOD_MENU ? i : i + (INVENTORY_SIZE/2)] != 0 ) item++;
          if( item == combat_selection+1 ){
            item = menu_selection == FOOD_MENU ? i : i + (INVENTORY_SIZE/2);
            break;
          }
        }
        if( item == ITEM_FRUIT ){
          party[combat_mode].health+=2*party[combat_mode].level;//Heal 10%
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,2*party[combat_mode].level, PHEAL);
        }else if( item == ITEM_BREAD ){
          party[combat_mode].health+=3*party[combat_mode].level;//Heal 15%
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,3*party[combat_mode].level, PHEAL);
        }else if( item == ITEM_MEAT ){
          party[combat_mode].bonus_damage+=4; // Meat increases damage
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,1, PDAMAGE);
        }else if( item == ITEM_TONIC ){
          party[combat_mode].health+=10*party[combat_mode].level;//Heal 50%
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,10*party[combat_mode].level, PHEAL);
        }else if( item == ITEM_TEA ){
          party[combat_mode].bonus_speed+=4;//Speed up
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,1, PSPEED);
        }else if( item == ITEM_LIQUOR ){
          party[combat_mode].bonus_defense+=2;// Liquor increases damage resistance
          inventory[item]--;
          copy_action_to_msg_buffer(combat_mode,0,1, PDEFENSE);
        }
        if( party[combat_mode].health > party[combat_mode].level*20 ) party[combat_mode].health = party[combat_mode].level*20;//Cap off healing
        combat_mode = MESSAGE;
      }
    }else if(gb.buttons.pressed(BTN_B)){
      gb.sound.playCancel();
      



      //This is for debugging  TODO: remove
      /*if( menu_selection == MUDLARK_MENU ){
        combat_mode = PRECOMBAT;
        mode = meta_mode;
      }*/




      if( menu_selection == ENEMY_MENU || menu_selection == ALLY_MENU || menu_selection == SECONDARY_MENU ){
        //since the combat mode and menu selection values line up for the party
        //members, we can do this to switch back to the right primary menu
        //depending on which party member's turn it is
        menu_selection = combat_mode; 
        combat_selection = 0;
      }else if( menu_selection == FOOD_MENU || menu_selection == DRINK_MENU ){
        menu_selection = SECONDARY_MENU;
        combat_selection = 0;
      }
    }
    byte mod = 4;
    if( menu_selection == ENEMY_MENU ){
      mod--;
      for( byte i = 0; i < 3; i++ ){
        if( enemy_buffer[i].lvl == -1 ){
          mod--;
        }
      }
    }else if( menu_selection == ENEMY_MENU ){
      //Determine how many party members there are so we can
      //wrap the selection cursor around
      mod = 1;
      if( party[SHADOW].level != 0 ) mod++;
      if( party[NURSE].level != 0 ) mod++;
    }else if( menu_selection == SECONDARY_MENU ){
      mod = 3;
    }else if( menu_selection == FOOD_MENU || menu_selection == DRINK_MENU ){
      uint8_t item = 0;
      for( uint8_t i = 0; i < INVENTORY_SIZE/2; i++ ){
        if( inventory[menu_selection == FOOD_MENU ? i : i + (INVENTORY_SIZE/2)] != 0 ) item++;
      }
      if( item < 4 ) mod = item; 
    }
    combat_selection%=mod;
  }

  //If it is an enemy's turn
  else if( combat_mode >= ENEMY1 && combat_mode <= ENEMY3 ){
    int16_t damage = calculate_damage(enemy_buffer[combat_mode-ENEMY1].lvl);
    //Choose which member of the party to attack
    uint8_t member = party[NURSE].level > 0 ? random(3) : (party[SHADOW].level > 0 ? random(2) : 0 );
    if( nurse_protect_bonus == member ){
      //Damage is reduced by 10% of nurse's level if nurse protects
      damage = party[NURSE].level >= damage ? 1 : damage - party[NURSE].level;
      // Reduce damage by an extra amount of (5% of reduced damage + 1) * bonus_defense 
      damage -= (damage/10/2 + 1)*party[NURSE].bonus_defense;
      if( damage <= 0 ) damage = 1;
      copy_action_to_msg_buffer(enemy_buffer[combat_mode-ENEMY1].nme,member,damage, PROTECT);
      member = NURSE; //Nurse takes damage instead
    }else{
      // Reduce damage by (5% of original damage + 1) * bonus_defense 
      damage -= (damage/10/2 + 1)*party[member].bonus_defense;
      if( damage <= 0 ) damage = 1;
      copy_action_to_msg_buffer(enemy_buffer[combat_mode-ENEMY1].nme,member,damage, EN2PL);
    }
    
    if( damage > party[member].health ){
      party[member].health = 0;
    }else{
      party[member].health-=damage;
    }
    combat_mode = MESSAGE;
  }

  //DEBUG TODO: remove
  /*gb.display.cursorX = 0;
  gb.display.cursorY = 6;
  gb.display.println(enemy_buffer[1].nme);
  gb.display.println(enemy_buffer[1].lvl);
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
  }*/

  gb.display.drawRect(0,SCREEN_HEIGHT/2-1,SCREEN_WIDTH,SCREEN_HEIGHT/2+1);
  
  
}
