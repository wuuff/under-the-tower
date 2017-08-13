#ifdef NOTDEFINED
byte map_cache[16][16];

byte cache_originx = 0;
byte cache_originy = 0;

void draw_cache(){
  for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 16; j++){
      if( map_cache[i][j] == WATER ){
        if( worldframe == 0 || worldframe == 1 ){
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),water1);
        }else{
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),water2);
        }
      }else if( map_cache[i][j] == DRAIN ){
        if( worldframe == 0 || worldframe == 1 ){
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),drain1);
        }else{
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),drain2);
        }
      }else if( map_cache[i][j] == FLOW ){
        if( worldframe == 0 || worldframe == 1 ){
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),flow1);
        }else{
          gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),flow2);
        }
      }else if( map_cache[i][j] != 0 ){
        gb.display.drawBitmap((cache_originx-dudex)+(j*8)+(SCREEN_WIDTH/2-4),(cache_originy-dudey)+(i*8)+(SCREEN_HEIGHT/2-4),&tiles[(map_cache[i][j])*10]);
      }
    }
  }
  worldframe+=1;
  worldframe%=4;
}

byte test_collision(byte dir){
  return 0;




  
  if( dir == UP ){
    gb.display.println(map_cache[(cache_originy+dudey-1)/8][(cache_originx+dudex)/8]);
    return map_cache[(cache_originy+dudey-1)/8][(cache_originx+dudex)/8] != 0 ||
      map_cache[(cache_originy+dudey-1)/8][(cache_originx+dudex+7)/8] != 0;
  }else if( dir == DOWN ){
    return map_cache[(cache_originy+dudey+8)/8][(cache_originx+dudex)/8] != 0 ||
      map_cache[(cache_originy+dudey+8)/8][(cache_originx+dudex+7)/8] != 0;
  }else if( dir == LEFT ){
    return map_cache[(cache_originy+dudey)/8][(cache_originx+dudex-1)/8] != 0 ||
      map_cache[(cache_originy+dudey+7)/8][(cache_originx+dudex-1)/8] != 0;
  }else{
    return map_cache[(cache_originy+dudey)/8][(cache_originx+dudex+8)/8] != 0 ||
      map_cache[(cache_originy+dudey+7)/8][(cache_originx+dudex+8)/8] != 0;
  }
}
#endif
