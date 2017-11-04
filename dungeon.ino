#include "overworld.h"
#include "dungeon.h"
#include "battle.h"
char dungeon_map[16][16];
uint8_t dungeonid = 0;//Used to identify dungeon
uint8_t dungeon_generated = 0;
uint8_t dungeon_level = 0;
int8_t previous_level = -1; //-1 means that the player entered the dungeon

#define DUN_STACK 0 // A stack of rooms
#define DUN_ZIGZAG 1 // Zig-zagging rooms, ascending, starting to the left
#define DUN_ZIGZAGR 2 // Zig-zagging rooms, ascending, starting to the right
#define DUN_SPLIT 3 // Rooms to the left AND right
#define DUN_SPIRAL 4 // Spiral of rooms, ascending, 4 per floor

#define DUN_THEME_CATPAW 0 // Catpaw theme
#define DUN_THEME_BRICK 1 // Brick building theme
#define DUN_THEME_SHIP 2 // Ship theme
#define DUN_THEME_WAREHOUSE 1 // Warehouse theme (NOW MERGED WITH BRICK)
#define DUN_THEME_HOSPITAL 3 // Hospital theme
#define DUN_THEME_HOUSE 0 // House theme (NOW MERGED WITH CATPAW)
#define DUN_THEME_TOWER 3 // The final tower theme (NOW MERGED WITH HOSPITAL)

#define DUN_END_DOOR 0 // Dungeon ends in an exit door 
#define DUN_END_BOSS 1 // Dungeon ends in a boss
#define DUN_END_CHEST 2 // Dungeon has a guaranteed treasure chest at the end

#define DUN_TILE_SIDE 2
#define DUN_TILE_DETAIL1 3
#define DUN_TILE_DETAIL2 4
#define DUN_TILE_DETAIL3 5
#define DUN_TILE_DOOR 6
#define DUN_TILE_STAIRSUP 7
#define DUN_TILE_STAIRSDN 8

const byte common_tiles[] PROGMEM = {
//Door
8,8,
B11111111,
B10000001,
B10000001,
B10000001,
B11000001,
B10000001,
B10000001,
B10000001,
//Stairs up
8,8,
B11110000,
B10011100,
B10010111,
B10010101,
B10010101,
B11110101,
B10011101,
B10000111,
//Stairs down
8,8,
B11111111,
B11111111,
B10111111,
B10101111,
B10101011,
B10101011,
B10101011,
B11111111,
//Wall top
8,8,
B10101010,
B01010101,
B10101010,
B01010101,
B10101010,
B01010101,
B10101010,
B01010101,
};

#define MAPSIZE 16
/*const struct dungeon dungeons[] PROGMEM = {
  {10,24,DUN_THEME_CATPAW,2,{BIG_RAT,PATRON,BOUNCER}},
  {10,24,DUN_THEME_CATPAW,2,{BIG_RAT,PATRON,BOUNCER}}
};*/

const struct dungeon dungeons[] PROGMEM = {
  {10,24,DUN_THEME_CATPAW,2,{BIG_RAT,PATRON,BOUNCER}},//{10,24,DUN_STACK,DUN_THEME_CATPAW,2,DUN_END_BOSS},
  {40,56,DUN_THEME_BRICK,3,{THUG,SEA_RAT,SWABBIE}},//{40,56,DUN_STACK,DUN_THEME_BRICK,3,DUN_END_CHEST},
  {40,48,DUN_THEME_BRICK,6,{THUG,SEA_RAT,SWABBIE}},//{40,48,DUN_STACK,DUN_THEME_BRICK,6,DUN_END_CHEST},//{40,48,DUN_ZIGZAG,DUN_THEME_BRICK,6,DUN_END_CHEST},
  {57,51,DUN_THEME_SHIP,2,{SWABBIE,SAILOR,SKIPPER}},//{57,51,DUN_STACK,DUN_THEME_SHIP,2,DUN_END_CHEST},//{57,51,DUN_ZIGZAG,DUN_THEME_SHIP,2,DUN_END_CHEST},
  {56,42,DUN_THEME_SHIP,4,{SWABBIE,SAILOR,SKIPPER}},//{56,42,DUN_STACK,DUN_THEME_SHIP,4,DUN_END_BOSS},//{56,42,DUN_SPLIT,DUN_THEME_SHIP,4,DUN_END_BOSS},
  {54,31,DUN_THEME_BRICK,2,{WATCHER,BRUISER,MUSCLER}},//{54,31,DUN_STACK,DUN_THEME_BRICK,2,DUN_END_CHEST},//{54,31,DUN_STACK,DUN_THEME_BRICK,2,DUN_END_CHEST},
  //The following two were originally intended to connect
  {44,31,DUN_THEME_WAREHOUSE,2,{WATCHER,BRUISER,MUSCLER}},//{44,31,DUN_STACK,DUN_THEME_WAREHOUSE,2,DUN_END_CHEST},
  //{36,31,DUN_THEME_WAREHOUSE,2,{WATCHER,BRUISER,MUSCLER}},//{36,31,DUN_STACK,DUN_THEME_WAREHOUSE,2,DUN_END_CHEST},
  //Two warehouse doors
  //{26,29,DUN_THEME_WAREHOUSE,4,{WATCHER,BRUISER,MUSCLER}},//{26,29,DUN_STACK,DUN_THEME_WAREHOUSE,4,DUN_END_CHEST},//{26,29,DUN_ZIGZAG,DUN_THEME_WAREHOUSE,4,DUN_END_CHEST},
  //{25,29,DUN_THEME_WAREHOUSE,4,{WATCHER,BRUISER,MUSCLER}},//{25,29,DUN_STACK,DUN_THEME_WAREHOUSE,4,DUN_END_CHEST},//{25,29,DUN_ZIGZAG,DUN_THEME_WAREHOUSE,4,DUN_END_CHEST},
  //The two hospital doors
  {50,16,DUN_THEME_HOSPITAL,9,{DOCTOR,PATIENT,SUBJECT}},//{50,16,DUN_STACK,DUN_THEME_HOSPITAL,9,DUN_END_BOSS},//{50,16,DUN_SPLIT,DUN_THEME_HOSPITAL,9,DUN_END_BOSS},
  {49,16,DUN_THEME_HOSPITAL,9,{DOCTOR,PATIENT,SUBJECT}},//{49,16,DUN_STACK,DUN_THEME_HOSPITAL,9,DUN_END_BOSS},//{49,16,DUN_SPLIT,DUN_THEME_HOSPITAL,9,DUN_END_BOSS},
  {55,4,DUN_THEME_WAREHOUSE,8,{RICHMAN,MAX_RAT,GUARD}},//{55,4,DUN_STACK,DUN_THEME_WAREHOUSE,8,DUN_END_CHEST},//{55,4,DUN_SPIRAL,DUN_THEME_WAREHOUSE,8,DUN_END_CHEST},
  {14,4,DUN_THEME_HOUSE,2,{SNOB,RICHMAN,MAX_RAT}},//{14,4,DUN_STACK,DUN_THEME_HOUSE,2,DUN_END_CHEST},
  {8,13,DUN_THEME_HOUSE,2,{SNOB,RICHMAN,MAX_RAT}},//{8,13,DUN_STACK,DUN_THEME_HOUSE,2,DUN_END_CHEST},
  {14,13,DUN_THEME_HOUSE,2,{SNOB,RICHMAN,MAX_RAT}},//{14,13,DUN_STACK,DUN_THEME_HOUSE,2,DUN_END_CHEST},
  //The two doors of THE TOWER
  {28,16,DUN_THEME_TOWER,40,{GUARD,GOLEM,OFFICER}},//{28,16,DUN_STACK,DUN_THEME_TOWER,40,DUN_END_BOSS},//{28,16,DUN_SPIRAL,DUN_THEME_TOWER,40,DUN_END_BOSS},
  {29,16,DUN_THEME_TOWER,40,{GUARD,GOLEM,OFFICER}},//{29,16,DUN_STACK,DUN_THEME_TOWER,40,DUN_END_BOSS},//{28,16,DUN_SPIRAL,DUN_THEME_TOWER,40,DUN_END_BOSS},
};

void mapinit(char map[][MAPSIZE], int width, int height);
void mapgen(char map[][MAPSIZE], int mapwidth, int mapheight, int startx, int starty, int endx, int endy);
void mapdetail(char map[][MAPSIZE], int mapwidth, int mapheight);

const byte dungeon_tiles[] PROGMEM = {
  //DUN_THEME_CATPAW
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
  //DUN_THEME_BRICK
8,8,
B00100010,
B11111111,
B10001000,
B11111111,
B00100010,
B11111111,
B10001000,
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
  //DUN_THEME_SHIP
8,8,
B00000000,
B00000000,
B00000000,
B11111111,
B00000000,
B00000000,
B00000000,
B11111111,
8,8,
B00011000,
B01100110,
B10000001,
B11100111,
B10011001,
B10000001,
B11100111,
B11111111,
8,8,
B10011001,
B10000001,
B11100111,
B11111111,
B10011001,
B10000001,
B01100110,
B00011000,
8,8,
B11111111,
B10000001,
B10000001,
B11111111,
B10000111,
B10011001,
B11100001,
B11111111,
  //DUN_THEME_HOSPITAL
8,8,
B00000001,
B00000001,
B00000001,
B11111111,
B00010000,
B00010000,
B00010000,
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
B00011100,
B00110110,
B11101100,
B11011110,
B01011011,
B00111011,
B01110111,
B11011100,
};

void draw_dungeon(){
  uint8_t tile;
  for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 16; j++){
      tile = dungeon_map[i][j];
      if( tile != 0 ){
        if( tile <= NUM_DUN_TILES+1 ){
          /*gb.display.cursorX = (j*8 - dudex)+(SCREEN_WIDTH/2-4);
          gb.display.cursorY = (i*8 - dudey)+(SCREEN_HEIGHT/2-4);
          gb.display.print( ((tile-2)+(pgm_read_byte(&(dungeons[dungeonid].theme))*NUM_DUN_TILES) ) );*/
          gb.display.drawBitmap((j*8 - dudex)+(SCREEN_WIDTH/2-4),(i*8 - dudey)+(SCREEN_HEIGHT/2-4),&dungeon_tiles[((tile-2)+(pgm_read_byte(&(dungeons[dungeonid].theme))*NUM_DUN_TILES))*10]);
        }else{
          //If this is a common tile, then draw that specific tile
          gb.display.drawBitmap((j*8 - dudex)+(SCREEN_WIDTH/2-4),(i*8 - dudey)+(SCREEN_HEIGHT/2-4),&common_tiles[(tile-2-NUM_DUN_TILES)*10]);
        }
      }
    }
  }
  //Draw player.  If moving and frames dictate it so, play walking animation
  if( player_moving && (dudeframe/2 == 0 || dudeframe/2 == 2) ){
    gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,player_sprites+((dudeframe/2)+dudeanimation*3)*10);
  }else{
    gb.display.drawBitmap(SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-4,player_sprites+(1+dudeanimation*3)*10);
  }
  gb.display.cursorX = 0;
  gb.display.cursorY = 0;
  //gb.display.print(dungeon_map[1][1]-1+((pgm_read_byte(&(dungeons[dungeonid].theme))*NUM_DUN_TILES)*10));
  gb.display.println(dungeonid);
}

byte test_collision(byte dir){
  uint8_t tile = 0;

  //Prioritize exit tiles over other tiles
  if( dir == UP ){
    tile = dungeon_map[(dudey-1)/8][(dudex)/8];
    if( tile != DUN_TILE_DOOR && tile != DUN_TILE_STAIRSUP && tile != DUN_TILE_STAIRSDN && dungeon_map[(dudey-1)/8][(dudex+7)/8] ){
      return dungeon_map[(dudey-1)/8][(dudex+7)/8];
    }else{
      return tile;
    }
  }else if( dir == DOWN ){
    tile = dungeon_map[(dudey+8)/8][(dudex)/8];
    if( tile != DUN_TILE_DOOR && tile != DUN_TILE_STAIRSUP && tile != DUN_TILE_STAIRSDN && dungeon_map[(dudey+8)/8][(dudex+7)/8] ){
      return dungeon_map[(dudey+8)/8][(dudex+7)/8];
    }else{
      return tile;
    }
  }else if( dir == LEFT ){
    tile = dungeon_map[(dudey)/8][(dudex-1)/8];
    if( tile != DUN_TILE_DOOR && tile != DUN_TILE_STAIRSUP && tile != DUN_TILE_STAIRSDN && dungeon_map[(dudey+7)/8][(dudex-1)/8] ){
      return dungeon_map[(dudey+7)/8][(dudex-1)/8];
    }else{
      return tile;
    }
  }else{
    tile = dungeon_map[(dudey)/8][(dudex+8)/8];
    if( tile != DUN_TILE_DOOR && tile != DUN_TILE_STAIRSUP && tile != DUN_TILE_STAIRSDN && dungeon_map[(dudey+7)/8][(dudex+8)/8] ){
      return dungeon_map[(dudey+7)/8][(dudex+8)/8];
    }else{
      return tile;
    }
  }
}

void test_collide_exit(uint8_t collision){
  switch( collision ){
    case DUN_TILE_DOOR: 
      //switch( pgm_read_byte(&(dungeons[dungeonid].type)) ){
      //  case DUN_STACK:
          //In a stack layout, the only door is the exit to the dungeon,
          //so this one is simple: exit the dungeon and place the player outside
          mode = TO_WORLD;
          transition = -SCREEN_HEIGHT/2;
          gb.display.persistence = true;
          dudex = pgm_read_byte(&(dungeons[dungeonid].x))*8;
          dudey = (pgm_read_byte(&(dungeons[dungeonid].y))+1)*8;
      //  break;
        //TODO: This requires more complex logic, depending on dungeon layout
      //}
      return;
    case DUN_TILE_STAIRSUP:
      previous_level = dungeon_level;
      dungeon_level++;
      dungeon_generated = 0;
      mode = TO_DUNGEON;
      transition = -SCREEN_HEIGHT/2;
      gb.display.persistence = true;
      return;
    case DUN_TILE_STAIRSDN:
      previous_level = dungeon_level;
      dungeon_level--;
      dungeon_generated = 0;
      mode = TO_DUNGEON;
      transition = -SCREEN_HEIGHT/2;
      gb.display.persistence = true;
      return;
  }
}

void step_dungeon(){

  if( dungeon_generated == 0 ){
     mapinit(dungeon_map,16,16);
     mapgen(dungeon_map,16,16,0,0,16,16);
     mapdetail(dungeon_map,16,16);
     //Default spawn position if spawning fails
     dudex = 16;
     dudey = 16;
     //Generate exits and spawn player
     mapexits(dungeon_map,16,16);
     dungeon_generated = 1;
  }
  
  player_moving = 0;
  
  
  uint8_t collision = 0;

    
  dudeframe++;
  dudeframe%=7;

  worldframe+=1;
  worldframe%=4;
  
  if( gb.buttons.repeat(BTN_UP,1) ){
    collision = test_collision(UP);
    if( collision ){
      test_collide_exit(collision);
    }else{
      dudey--;
      dudeanimation = UP;
      player_moving = 1;
    }
  }
  else if( gb.buttons.repeat(BTN_DOWN,1) ){
    collision = test_collision(DOWN);
    if( collision ){
      test_collide_exit(collision);
    }else{
      dudey++;
      dudeanimation = DOWN;
      player_moving = 1;
    }
  }
  if( gb.buttons.repeat(BTN_LEFT,1) ){
    collision = test_collision(LEFT);
    if( collision ){
      test_collide_exit(collision);
    }else{
      dudex--;
      dudeanimation = LEFT;
      player_moving = 1;
    }
  }
  else if( gb.buttons.repeat(BTN_RIGHT,1) ){
    collision = test_collision(RIGHT);
    if( collision ){
      test_collide_exit(collision);
    }else{
      dudex++;
      dudeanimation = RIGHT;
      player_moving = 1;
    }
  }

  if( player_moving ){
    try_combat();
  }
}

void mapinit(char map[][MAPSIZE], int width, int height){
    int i,j;

    //Clear all tiles
    for( i = 0; i < width; i++ ){
      for( j = 0; j < height; j++ ){
        map[j][i] = 0;
        
        //Less runtime efficiency, but more space efficiency putting this here
        map[0][j] = DUN_WALL;
        map[height-1][j] = DUN_WALL;

        //Take advantage of square room TODO: if we have rectangular rooms this will break
        map[j][0] = DUN_WALL;
        map[j][width-1] = DUN_WALL;
      }
    }
    //Generate walls around the edges
    /*for( i = 0; i < width; i++ ){
      map[0][i] = DUN_WALL;
      map[height-1][i] = DUN_WALL;

      //Take advantage of square room TODO: if we have rectangular rooms this will break
      map[i][0] = DUN_WALL;
      map[i][width-1] = DUN_WALL;
    }*/
    /*for( j = 0; j < height; j++ ){
      map[j][0] = DUN_WALL;
      map[j][width-1] = DUN_WALL;
    }*/
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
                map[position][i] = DUN_WALL;
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
        door = starty + 1 + (rand()%(height-3));//I don't quite follow why this would need to be 3, but it seems to prevent a too-small doorway.  TODO: investigate
        //Generate an extra door if wall is long enough.
        //Doors may overlap or be next to each other because such
        //doors shouldn't be a problem and it might result in interesting maps.
        //If it is not long enough set to -1 so it won't interfere
        door2 = height >= EXTRA_DOOR ? starty + 1 + (rand()%(height-3)) : -1;
        //printf("VERT %d\n",position);
        for( i = starty; i < starty + height; i++ ){
            if( i != door && i != door+1 && i != door2 && i != door2+1 )
                map[i][position] = DUN_WALL;
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
      if( map[k][m] == DUN_WALL ){
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
      if( map[i-1][j] == DUN_WALL && map[i][j] == 0 ){
        map[i][j] = 2;
      }
      //Check if we want to put something decorative
      if( i > 1 && map[i][j] == 0 && DECORATION_CHANCE > (rand()%100) ){
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
}

void mapexits(char map[][MAPSIZE], int width, int height){
  uint8_t i,j;
  //switch( pgm_read_byte(&(dungeons[dungeonid].type)) ){
  //  case DUN_STACK:
      //If on ground floor, generate entrance door
      if( dungeon_level == 0 ){
        while(1){//Generate until we get a valid placement
          i = height-1;
          j = 1+(rand()%(width-1));
          //If the space above is clear, place a door
          if( map[i-1][j] == 0 ){
            map[i][j] = DUN_TILE_DOOR;
            //If the previous room is outside, spawn player here
            if( previous_level == -1 ){
              dudex = j*8;
              dudey = (i-1)*8;
            }
            break;
          }
        }
      }else{ //If not on ground level, generate stairs down
        while(1){//Generate until we get a valid placement
          i = 1+(rand()%(height-1));
          j = 1+(rand()%(width-1));
          //If the space for the stairs is clear, place stairs down
          if( map[i][j] == 0 ){
            map[i][j] = DUN_TILE_STAIRSDN;
            //If the previous room is below this one, spawn player here
            if( previous_level >= 0 && previous_level < dungeon_level ){
              if( map[i][j+1] == 0 ){
                dudex = (j+1)*8;
                dudey = i*8;
              }else if( map[i+1][j] == 0 ){
                dudex = j*8;
                dudey = (i+1)*8;
              }else if( map[i][j-1] == 0 ){
                dudex = (j-1)*8;
                dudey = i*8;
              }else if( map[i-1][j] == 0 ){
                dudex = j*8;
                dudey = (i-1)*8;
              }
            }
            break;
          }
          
        }
      }
      //If not at the top of the stack of rooms, generate stairs up
      if( dungeon_level < pgm_read_byte(&(dungeons[dungeonid].size))-1 ){
        while(1){//Generate until we get a valid placement
          i = 1+(rand()%(height-1));
          j = 1+(rand()%(width-1));
          //If the space for the stairs is clear, place stairs up
          if( map[i][j] == 0 ){
            map[i][j] = DUN_TILE_STAIRSUP;
            //If the previous room is above this one, spawn player here
            if( previous_level > dungeon_level ){
              if( map[i][j+1] == 0 ){
                dudex = (j+1)*8;
                dudey = i*8;
              }else if( map[i+1][j] == 0 ){
                dudex = j*8;
                dudey = (i+1)*8;
              }else if( map[i][j-1] == 0 ){
                dudex = (j-1)*8;
                dudey = i*8;
              }else if( map[i-1][j] == 0 ){
                dudex = j*8;
                dudey = (i-1)*8;
              }
            }
            break;
          }
        }
      }
      //break;
    //TODO: Add code that puts doors in random positions on fixed sides,
    //based on which level of the dungeon we are on, making sure they do
    //not have a wall blocking them. Do this for all other types.
  //}
}
//ALSO TODO: add drawing of exit doors (and chests) as a separate thing
//since it's common to all dungeons
//ALSO ALSO TODO: Add code to spawn the player at the correct door based
//on the room they last came from (or if they just entered the dungeon).
