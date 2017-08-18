#include "overworld.h"
char dungeon_map[16][16];
uint8_t dungeonx,dungeony;//Used to identify dungeon
uint8_t dungeon_generated = 0;
uint8_t dungeon_level = 0;

#define DUN_STACK 0 // A stack of rooms
#define DUN_ZIGZAG 1 // Zig-zagging rooms, ascending, starting to the left
#define DUN_ZIGZAGR 2 // Zig-zagging rooms, ascending, starting to the right
#define DUN_SPLIT 3 // Rooms to the left AND right
#define DUN_SPIRAL 4 // Spiral of rooms, ascending, 4 per floor

#define DUN_THEME_CATPAW 0 // Catpaw theme
#define DUN_THEME_BRICK 1 // Brick building theme
#define DUN_THEME_SHIP 2 // Ship theme
#define DUN_THEME_WAREHOUSE 3 // Warehouse theme
#define DUN_THEME_HOSPITAL 4 // Hospital theme
#define DUN_THEME_HOUSE 5 // House theme
#define DUN_THEME_TOWER 6 // The final tower theme

#define DUN_END_DOOR 0 // Dungeon ends in an exit door 
#define DUN_END_BOSS 1 // Dungeon ends in a boss
#define DUN_END_CHEST 2 // Dungeon has a guaranteed treasure chest at the end

byte cache_originx = 0;
byte cache_originy = 0;

#define MAPSIZE 16

struct dungeon{
  uint8_t x;
  uint8_t y;
  uint8_t type;
  uint8_t theme;
  uint8_t size;
  uint8_t exit;
};

const struct dungeon dungeons[] PROGMEM = {
  {10,24,DUN_STACK,DUN_THEME_CATPAW,2,DUN_END_BOSS},
  {40,56,DUN_STACK,DUN_THEME_BRICK,3,DUN_END_CHEST},
  {40,48,DUN_ZIGZAG,DUN_THEME_BRICK,6,DUN_END_CHEST},
  {57,51,DUN_ZIGZAG,DUN_THEME_SHIP,2,DUN_END_CHEST},
  {56,42,DUN_SPLIT,DUN_THEME_SHIP,4,DUN_END_BOSS}
};

void mapinit(char map[][MAPSIZE], int width, int height);
void mapgen(char map[][MAPSIZE], int mapwidth, int mapheight, int startx, int starty, int endx, int endy);
void mapdetail(char map[][MAPSIZE], int mapwidth, int mapheight);

const byte dungeon_tiles[] PROGMEM = {
8,8,
B10101010,
B01010101,
B10101010,
B01010101,
B10101010,
B01010101,
B10101010,
B01010101,
8,8,
B01010101,
B01010101,
B01010101,
B01010101,
B11111111,
B10000001,
B10000001,
B11111111,
8,8,
B01111110,
B10000001,
B10000001,
B11000011,
B10111101,
B10000001,
B10000001,
B10000001,
8,8,
B10000001,
B10000001,
B10000001,
B11100111,
B10111101,
B10000001,
B10111101,
B11000011,
8,8,
B00000000,
B00000000,
B01111110,
B10000001,
B01111110,
B00011000,
B00111100,
B01000010,
};

void draw_dungeon(){
  for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 16; j++){
      if( dungeon_map[i][j] != 0 ){
        gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),&dungeon_tiles[(dungeon_map[i][j]-1)*10]);
      }
    }
  }
  worldframe+=1;
  worldframe%=4;
}

byte test_collision(byte dir){
  return 0;




  
  if( dir == UP ){
    gb.display.println(dungeon_map[(cache_originy+dudey-1)/8][(cache_originx+dudex)/8]);
    return dungeon_map[(cache_originy+dudey-1)/8][(cache_originx+dudex)/8] != 0 ||
      dungeon_map[(cache_originy+dudey-1)/8][(cache_originx+dudex+7)/8] != 0;
  }else if( dir == DOWN ){
    return dungeon_map[(cache_originy+dudey+8)/8][(cache_originx+dudex)/8] != 0 ||
      dungeon_map[(cache_originy+dudey+8)/8][(cache_originx+dudex+7)/8] != 0;
  }else if( dir == LEFT ){
    return dungeon_map[(cache_originy+dudey)/8][(cache_originx+dudex-1)/8] != 0 ||
      dungeon_map[(cache_originy+dudey+7)/8][(cache_originx+dudex-1)/8] != 0;
  }else{
    return dungeon_map[(cache_originy+dudey)/8][(cache_originx+dudex+8)/8] != 0 ||
      dungeon_map[(cache_originy+dudey+7)/8][(cache_originx+dudex+8)/8] != 0;
  }
}

void step_dungeon(){

  if( dungeon_generated == 0 ){
     mapinit(dungeon_map,16,16);
     mapgen(dungeon_map,16,16,0,0,16,16);
     mapdetail(dungeon_map,16,16);
     dudex = 16;
     dudey = 16;
     dungeon_generated = 1;
  }
  
  byte moved = 0;
  
  dudeframe%=7;
  
  dudeframe++;
  
  if( gb.buttons.repeat(BTN_UP,1) && !test_collision(UP) ){
    //Overwrite standing sprite
    if(dudeframe/2 == 0){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudeup1);
    }else if(dudeframe/2 == 2){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudeup2);
    }else{
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudeup);
    }
    
    dudey--;
    dudeanimation = UP;
    moved = 1;
    //try_combat();  TODO: Restore combat
  }
  else if(gb.buttons.repeat(BTN_DOWN,1) && !test_collision(DOWN)){
    //Overwrite standing sprite
    if(dudeframe/2 == 0){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudedn1);
    }else if(dudeframe/2 == 2){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudedn2);
    }else{
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dude);
    }
    
    dudey++;
    dudeanimation = DOWN;
    moved = 1;
    //try_combat();  TODO: Restore combat
  }
  if(gb.buttons.repeat(BTN_LEFT,1) && !test_collision(LEFT)){
    if(dudeframe/2 == 0){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert1,NOROT,FLIPH);
    }else if(dudeframe/2 == 2){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert2,NOROT,FLIPH);
    }else{
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert,NOROT,FLIPH);
    }
    
    dudex--;
    dudeanimation = LEFT;
    moved = 1;
    //try_combat();  TODO: Restore combat
  }
  else if(gb.buttons.repeat(BTN_RIGHT,1) && !test_collision(RIGHT)){
    if(dudeframe/2 == 0){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert1);
    }else if(dudeframe/2 == 2){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert2);
    }else{
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert);
    }
    
    dudex++;
    dudeanimation = RIGHT;
    moved = 1;
    //try_combat();  TODO: Restore combat
  }

  if( !moved ){
    if( dudeanimation == UP ){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudeup);
    }
    else if( dudeanimation == DOWN ){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dude);
    }
    else if( dudeanimation == LEFT ){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert,NOROT,FLIPH);
    }
    else if( dudeanimation == RIGHT ){
      gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,dudert);
    }
  }
}

void mapinit(char map[][MAPSIZE], int width, int height){
    int i,j;
    
    //Generate walls around the edges
    for( i = 0; i < width; i++ ){
        map[0][i] = 1;
        map[height-1][i] = 1;
    }
    for( j = 0; j < height; j++ ){
        map[j][0] = 1;
        map[j][width-1] = 1;
    }
}

#define HORIZONTAL 0
#define VERTICAL 1
#define MIN_WIDTH 6
#define MIN_HEIGHT 8
#define HALL_CHANCE 90
#define MIN_HALL_WIDTH 8
#define MIN_HALL_HEIGHT 10
#define MAX_HALL_WIDTH 12
#define MAX_HALL_HEIGHT 14
#define EXTRA_DOOR 10
void mapgen(char map[][MAPSIZE], int mapwidth, int mapheight, int startx, int starty, int endx, int endy){
    int i,j,orientation,position,door,door2,doorcount,hall,colspacex,colspacey;
    int width = endx-startx;
    int height = endy-starty;
    
    if( width < MIN_WIDTH && height < MIN_HEIGHT ){
        return;
    }

    //Determine whether to generate a big hall or not.
    //The size of the room determines the column spacing for now
    if( width >= MIN_HALL_WIDTH && height >= MIN_HALL_HEIGHT && width < MAX_HALL_WIDTH && height < MAX_HALL_HEIGHT ){
        hall = rand()%100;//Percent chance out of 100
        if( HALL_CHANCE > hall ){
            return;//Do not subdivide; return immediately
        }
    }
    
    //Determine whether we will split the space
    //horizontally or vertically by choosing whichever
    //orientation is larger (this avoids extremely long rooms)
    if( width >= height ){
        //If there is a door (or more than one door!) into a small room,
        //we may not be able to generate a wall in any location!
        //so abort if room is min + number of horiz door tiles
        doorcount = 0;
        for( i = startx; i < endx; i++ ){
            if( map[starty][i] == 0 ) doorcount++;
            if( map[endy][i] == 0 ) doorcount++;
        }
        if( width < MIN_WIDTH + doorcount ){
            return;
        }
        orientation = VERTICAL;
        //puts("Trying vertical");
    }else{
        //If there is a door (or more than one door!) into a small room,
        //we may not be able to generate a wall in any location!
        //so abort if room is min + number of vert door tiles
        doorcount = 0;
        for( i = starty; i < endy; i++ ){
            if( map[i][startx] == 0 ) doorcount++;
            if( map[i][endx] == 0 ) doorcount++;
        }
        if( height < MIN_HEIGHT + doorcount ){
            return;
        }
        orientation = HORIZONTAL;
        //puts("Trying horizontal");
    }
    //printf("startx %d, starty %d\n",startx,starty);
    //mapprint(map,MAPSIZE,MAPSIZE);
    position = -1;
    if( orientation == HORIZONTAL ){
        //Make sure the position is valid:
        //1. It must have generated at least one number
        //2. It must not be too close to existing walls
        //3. It must not be over a door
        while( position == -1 || position < starty + (MIN_HEIGHT/2) || position > endy - (MIN_HEIGHT/2) || map[position][startx] == 0 || map[position][endx] == 0 ){
            position = starty + (rand()%height);
        }
        //Generate a door at a random position
        door = startx + 1 + (rand()%(width-1));
        //Generate an extra door if wall is long enough.
        //Doors may overlap or be next to each other because such
        //doors shouldn't be a problem and it might result in interesting maps.
        //If it is not long enough set to -1 so it won't interfere
        door2 = width >= EXTRA_DOOR ? startx + 1 + (rand()%(width-1)) : -1;
        //printf("HORIZ %d\n",position);
        for( i = startx; i < startx + width; i++ ){
            if( i != door && i != door2 )
                map[position][i] = 1;
        }
        //Recursively call to fill the two new spaces we generated
        mapgen(map, mapwidth, mapheight, startx, starty, endx,position);
        mapgen(map, mapwidth, mapheight, startx, position, endx, endy);
    }else if( orientation == VERTICAL ){
        //Make sure the position is valid:
        //1. It must have generated at least one number
        //2. It must not be too close to existing walls
        //3. It must not be over a door
        while( position == -1 || position < startx + (MIN_WIDTH/2) || position > endx - (MIN_WIDTH/2) || map[starty][position] == 0 || map[endy][position] == 0 ){
            position = startx + (rand()%width);
        }
        //Generate a door at a random position
        //(allocating space for it to be 2 high)
        door = starty + 1 + (rand()%(height-2));
        //Generate an extra door if wall is long enough.
        //Doors may overlap or be next to each other because such
        //doors shouldn't be a problem and it might result in interesting maps.
        //If it is not long enough set to -1 so it won't interfere
        door2 = height >= EXTRA_DOOR ? starty + 1 + (rand()%(height-2)) : -1;
        //printf("VERT %d\n",position);
        for( i = starty; i < starty + height; i++ ){
            if( i != door && i != door+1 && i != door2 && i != door2+1 )
                map[i][position] = 1;
        }
        //Recursively call to fill the two new spaces we generated
        mapgen(map, mapwidth, mapheight, startx, starty, position,endy);
        mapgen(map, mapwidth, mapheight, position, starty, endx, endy);
    }
}

uint8_t check_proximity(char map[][MAPSIZE], uint8_t i, uint8_t j, uint8_t extra){
  uint8_t k,m;
  for( k = i-2; k <= i+1+extra; k++ ){
    for( m = j-1; m <= j+1; m++ ){
      if( map[k][m] == 1 ){
        return 0;
      }
    }
  }
  return 1;
}

#define DECORATION_CHANCE 20
void mapdetail(char map[][MAPSIZE], int width, int height){
  uint8_t i,j,k,m;
  //Avoid checking the boundary tiles
  for( i = 1; i < height-1; i++ ){
    for( j = 1; j < width-1; j++ ){
      //Place a sideways wall under each wall top
      if( map[i-1][j] == 1 && map[i][j] == 0 ){
        map[i][j] = 2;
      }
      //Check if we want to put something decorative
      if( i > 1 && map[i][j] != 1 && DECORATION_CHANCE > (rand()%100) ){
        if( 50 > (rand()%100) ){
          if( check_proximity( map, i, j, 0 ) ){
            map[i][j] = 5;
          }
        }else{
          if( check_proximity( map, i, j, 1 ) ){
            map[i][j] = 3;
            map[i+1][j] = 4;
          }
        }
      }
    }
  }
  //TODO: Add code that puts doors in random positions on fixed sides,
  //based on which level of the dungeon we are on, making sure they do
  //not have a wall blocking them.
}
//ALSO TODO: add drawing of exit doors (and chests) as a separate thing
//since it's common to all dungeons
//ALSO ALSO TODO: Add code to spawn the player at the correct door based
//on the room they last came from (or if they just entered the dungeon).
