//Scamp: Level 1
//Ruffian: Level 2
//Scoundrel: Level 4
//Thief (or maybe Shadow) recruits you on his quest

struct enemy{
  int8_t lvl;
  uint8_t spd;
  uint8_t img;
  uint8_t nme;
};

struct enemy enemy_buffer[3];//Store data for enemies to battle

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
#define OVERMAN 17
#define DOCTOR 18
#define PATIENT 19
#define SUBJECT 20
#define MUTANT 21
#define MADMAN 22
#define WOW_RAT 23
#define SNOB 24
#define RICHMAN 25
#define MAX_RAT 26
#define GUARD 27
#define GOLEM 28
#define OFFICER 29
#define LEADER 30
#define CRAB 31
#define EN_SHADOW 32

//This is not efficient, because there is always an
//entry for the enemy name even though every enemy has
//a unique name and I don't want two different enemies 
//with the same name.  However, the way I copy names into
//the buffer uses a generic approach that always copies from an
//array of 8-char strings in progmem, and I don't want to change all
//that code for a special case with the name!  Therefore, this
//approach is a bit inefficient because refactoring is a huge chore.
const struct enemy enemies[] PROGMEM = {
  {1,5,1,RAT},
  {1,4,0,SCAMP},
  {2,3,0,RUFFIAN},
  {4,2,0,THUG},
  {4,5,1,BIG_RAT},
  {5,4,0,PATRON},
  {5,5,2,BOUNCER},
  {8,5,0,SLAVER},
  {8,6,1,SEA_RAT},
  {9,4,0,SWABBIE},
  {11,3,0,SAILOR},
  {10,5,0,SKIPPER},
  {15,4,0,CAPTAIN},
  {14,7,1,BAD_RAT},
  {15,5,0,WATCHER},
  {18,3,2,BRUISER},
  {20,2,2,MUSCLER},
  {25,3,2,OVERMAN},
  {25,4,0,DOCTOR},
  {26,5,0,PATIENT},
  {26,6,0,SUBJECT},
  {26,7,0,MUTANT},
  {32,7,0,MADMAN},
  {30,7,1,WOW_RAT},
  {32,3,0,SNOB},
  {35,2,0,RICHMAN},
  {36,7,1,MAX_RAT},
  {38,4,0,GUARD},
  {42,2,0,GOLEM},
  {40,4,0,OFFICER},
  {50,7,0,LEADER},
  {1,2,3,CRAB},
  {8,7,0,EN_SHADOW},
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
"OVERMAN",
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

/*const byte enemy_levels[] PROGMEM = {
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
};*/

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
uint16_t mudlark_health = 10;
uint8_t mudlark_speed = 4;
uint8_t mudlark_xp = 0;
uint8_t mudlark_bonus_speed = 0;
uint8_t mudlark_bonus_damage = 0;
uint8_t mudlark_bonus_defense = 0;

uint8_t shadow_level = 0;
uint8_t shadow_health = 0;
uint8_t shadow_speed = 4;
uint8_t shadow_xp = 0;
uint8_t shadow_bonus_speed = 0;
uint8_t shadow_bonus_damage = 0;
uint8_t shadow_bonus_defense = 0;

uint8_t nurse_level = 0;
uint8_t nurse_health = 0;
uint8_t nurse_speed = 4;
uint8_t nurse_xp = 0;
uint8_t nurse_bonus_speed = 0;
uint8_t nurse_bonus_damage = 0;
uint8_t nurse_bonus_defense = 0;

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

#define INVENTORY_SIZE 6

unsigned char inventory[6] = {2,1,0,1,16,0};//Each element == how much of each item

uint8_t next_combat = 10;

void try_combat(){
  next_combat--;
  if( next_combat == 0 ){
    mode = TO_COMBAT;
    transition = -SCREEN_HEIGHT/2;
    gb.display.persistence = true;
    next_combat = random(90)+10;//10-100 steps
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
  " FINDS\n",
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

char combat_mode = PRECOMBAT;
byte menu_selection = 0;
byte combat_selection = 0;
char combat_buffer[8] = "\0\0\0\0\0\0\0";
char combat_message[64];//Max of 64 characters in message, which should be plenty
int8_t combat_status[6] = {0,0,0,0,0,0};//Determines who moves next
uint8_t combat_xp = 0;

//int8_t enemies[3];
uint8_t enemy_health[3];

uint8_t calculate_damage(uint8_t lvl){
  return lvl*10/8;
}

void load_enemy_data(uint8_t index, uint8_t slot){
  //Right now only support overworld enemy generation
  //TODO: add support for dungeon spawn tables

  //Use dudex and dudey to determine which pool to spawn enemies from
  uint8_t en_ind = pgm_read_byte(&(world_spawns[dudey/8/16][dudex/8/16][index]));
  const struct enemy* en = &(enemies[en_ind]);
  enemy_buffer[slot].lvl = pgm_read_byte(&(en->lvl));
  enemy_buffer[slot].spd = pgm_read_byte(&(en->spd));
  enemy_buffer[slot].img = pgm_read_byte(&(en->img));
  enemy_buffer[slot].nme = pgm_read_byte(&(en->nme));
}

void gen_enemies(){
  uint8_t enemy_index = 0;
  combat_xp = 0;
  //Always generate center enemy
  enemy_index = random(3);
  load_enemy_data(enemy_index,1);
  combat_xp+=enemy_buffer[1].lvl;
  enemy_health[1] = enemy_buffer[1].lvl*10/2;
  //50% chance of left enemy
  if( random(2) == 0 ){
    enemy_index = random(3);
    load_enemy_data(enemy_index,0);
    combat_xp+=enemy_buffer[0].lvl;
    enemy_health[0] = enemy_buffer[0].lvl*10/2;
  }else{
    enemy_buffer[0].lvl = -1;
  }
  //50% chance of right enemy
  if( random(2) == 0 ){
    enemy_index = random(3);
    load_enemy_data(enemy_index,2);
    combat_xp+=enemy_buffer[2].lvl;
    enemy_health[2] = enemy_buffer[2].lvl*10/2;
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

//TODO: space optimize this
void copy_action_to_msg_buffer(uint8_t source, uint8_t dest, uint8_t amount, uint8_t type){
  uint8_t offset = 0;
  if( type == PL2EN || type == PHEAL || type == PSPEED || type == PITEM )
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
  }else if( type == PSPEED ){
    offset = append_to_msg_buffer( 8, combat_text, offset );
  }else if( type == PITEM ){
    offset = append_to_msg_buffer( 9, combat_text, offset );
    combat_message[offset++] = ' ';
    offset = append_to_msg_buffer( amount, item_names, offset );
  }else{
    offset = append_to_msg_buffer( 0, combat_text, offset );
  }
  if( type == PL2EN )
    offset = append_to_msg_buffer( dest, enemy_names, offset );
  else if( type == EN2PL )
    offset = append_to_msg_buffer( dest, player_names, offset );
  if( type != PSPEED && type != PITEM ){
    offset = append_to_msg_buffer( 1, combat_text, offset );
    combat_message[offset++] = amount/100+'0';
    combat_message[offset++] = amount%100/10+'0';
    combat_message[offset++] = amount%100%10+'0';
    offset = append_to_msg_buffer( 2, combat_text, offset );
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
    if( index <= SECONDARY_MENU ){
      offset = append_to_msg_buffer(index*4+i,menu_text,0);//Ordinary menus with fixed text
    }else if(index == ENEMY_MENU){//Dynamically generated menus with text for allies/enemies
      combat_message[0] = '\0';//If we don't copy anything in, print nothing
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
      combat_message[0] = '\0';//If we don't copy anything in, print nothing
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
    }else{
      //TODO: allies menu
    }
    combat_message[offset] = '\0';
    gb.display.print(combat_message);
  }
}

void do_combat_step(){
  combat_status[MUDLARK] += mudlark_speed + mudlark_bonus_speed;
  if( shadow_level > 0 ) combat_status[SHADOW] += shadow_speed;
  if( nurse_level > 0 ) combat_status[NURSE] += nurse_speed;
  if( enemy_buffer[0].lvl != -1 ) combat_status[ENEMY1] += enemy_buffer[0].spd;
  if( enemy_buffer[1].lvl != -1 ) combat_status[ENEMY2] += enemy_buffer[1].spd;
  if( enemy_buffer[2].lvl != -1 ) combat_status[ENEMY3] += enemy_buffer[2].spd;

  uint8_t maxi = 0;
  for(uint8_t i = 0; i < 6; i++){
    if( combat_status[i] > combat_status[maxi] ) maxi = i;
  }
  combat_mode = maxi;
  combat_selection = 0;//Force the first item to be selected by default
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
        if( enemy_buffer[0].lvl == -1 && enemy_buffer[1].lvl == -1 && enemy_buffer[2].lvl == -1 ){
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
          if( enemy_buffer[i].lvl != -1 && enemy_health[i] == 0 ){
            copy_action_to_msg_buffer(enemy_buffer[i].nme,0,0, EFALL);
            combat_status[ENEMY1+i] = -1;
            enemy_buffer[i].lvl = -1;//This will break center enemy TODO: support absence of center enemy
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
    
    gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+6;
    gb.display.print(F("MUDLARK"));
    gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
    gb.display.cursorY = SCREEN_HEIGHT/2+12;
    gb.display.print(mudlark_health);
    gb.display.print(F("/"));
    gb.display.print(mudlark_level*10);

    //DEBUG
    //gb.display.print(F(" S"));
    //gb.display.print(mudlark_speed);
    gb.display.cursorX = SCREEN_WIDTH-33;//8*4+1
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
      }else if( menu_selection == MUDLARK_MENU && combat_selection == 2 ){ //Scavenge
        if( random(100) < 50 ){ //Highest chance to get fruit
          inventory[ITEM_FRUIT]++;
          copy_action_to_msg_buffer(0,0,ITEM_FRUIT, PITEM);
        }else if( random(100) < 50 ){ //Second highest chance to get bread
          inventory[ITEM_BREAD]++;
          copy_action_to_msg_buffer(0,0,ITEM_BREAD, PITEM);
        }else{ //All the rest of the items have an equal chance
          uint8_t chosen = random(4) + 2;
          inventory[chosen]++;
          copy_action_to_msg_buffer(0,0, chosen, PITEM);
        }
        combat_mode = MESSAGE;
      }else if( menu_selection == MUDLARK_MENU && combat_selection == 3 ){
        menu_selection = SECONDARY_MENU;
        combat_selection = 0;
      }else if( menu_selection == SECONDARY_MENU ){
        if( combat_selection == 0 ){ // FOOD
          menu_selection = FOOD_MENU;
          combat_selection = 0;
        }else if( combat_selection == 1 ){ // DRINK
          menu_selection = DRINK_MENU;
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
        if( combat_selection == 1 && enemy_buffer[0].lvl == -1 ){
          combat_selection = 2;//Must be the third one
        }else{
          for( ; combat_selection < 3; combat_selection++ ){
            if( enemy_buffer[combat_selection].lvl != -1 ){
              break;  // As soon as we find an existing enemy, that's the one that was selected
            }
          }
        }
        copy_action_to_msg_buffer(0,enemy_buffer[combat_selection].nme,damage, PL2EN);
        if( damage > enemy_health[combat_selection] ){
          enemy_health[combat_selection] = 0;
        }else{
          enemy_health[combat_selection]-=damage;
        }
        combat_mode = MESSAGE;
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
          mudlark_health+=mudlark_level;//Heal 10%
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,mudlark_level, PHEAL);
        }else if( item == ITEM_BREAD ){
          mudlark_health+=mudlark_level*3/2;//Heal 15%
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,mudlark_level*3/2, PHEAL);
        }else if( item == ITEM_TONIC ){
          mudlark_health+=(mudlark_level*10/2);//Heal 50%
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,(mudlark_level*10/2), PHEAL);
        }else if( item == ITEM_TEA ){
          mudlark_bonus_speed++;//Speed up
          inventory[item]--;
          copy_action_to_msg_buffer(0,0,1, PSPEED);
        }
        if( mudlark_health > mudlark_level*10 ) mudlark_health = mudlark_level*10;//Cap off healing
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
    uint8_t damage = calculate_damage(enemy_buffer[combat_mode-ENEMY1].lvl);
    copy_action_to_msg_buffer(enemy_buffer[combat_mode-ENEMY1].nme,0,damage, EN2PL);
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
  gb.display.println(enemy_buffer[1].nme);
  gb.display.println(enemy_buffer[1].lvl);
  /*gb.display.print((char)(combat_mode+'0'));
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
