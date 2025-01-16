// tetris.h
//========================================================================
// TETRIS with Wio Terminal
// Wio_Tetris : 2020.05.22 Transplant by macsbug
//            : https://macsbug.wordpress.com/2020/05/22/wio-tetris/ 
// Controller : KEY_C or 5S_LEFT = LEFT, KEY_B or 5S_LEFT = RIGHT, 
//            : KEY_A or 5S_PRESS = START or ROTATE
// Controller : Buttons A = LEFT, B = RIGHT, C = START, ROTATE
// Display    : Left = 100x240, Center = 120x240, Right = 100x240
// Block      : 8ea, 12x12 pixel
// SD         : tetris.jpg : BackGround Image : R.G.B 320x240 pixel
// Github     : (reference)https://github.com/MhageGH/esp32_ILI9328_Tetris
// Github     : (Original) https://macsbug.wordpress.com/2018/01/20/tetris-with-m5stack/
// Github     : (M5Stack)  https://github.com/m5stack/M5Stack/tree/master/examples/Games/Tetris
// Github     : (revision) http://shikarunochi.matrix.jp/?p=2296
// How to     : How to make tetris https://www.nicovideo.jp/watch/sm17983957
// Sound      : Sound processing with ULP co-processor
// Github     : bitluni/ULPSoundESP32：https://github.com/bitluni/ULPSoundESP32
// tetris.h   : Sound : 2.4MB
//========================================================================

#include "tetris_image.c"

uint16_t BlockImage[8][12][12];// Block
uint16_t backBuffer[240][120]; // GAME AREA
uint16_t nextBlockBuf[60][48]; // NEXT BLOCK AREA
const int Length = 12;         // the number of pixels for a side of a block
const int Width  = 10;         // the number of horizontal blocks
const int Height = 20;         // the number of vertical blocks
int screen[Width][Height]={0}; // it shows color-numbers of all positions
struct Point {int X, Y;};
struct Block {Point square[4][4]; int numRotate, color;};
Point pos; Block block;
int nextBlockType = -1;
long score = 0;
Block nextBlock;
int rot, fall_cnt = 0;
bool started = false, gameover = false;
boolean b_CTR=false, b_LEFT=false, b_RIGHT=false, b_DOWN=false;
int game_speed = 50;           // 25msec
Block blocks[7] = {
  {{{{-1,0},{0,0},{1,0},{2,0}},{{0,-1},{0,0},{0,1},{0,2}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,1},
  {{{{0,-1},{1,-1},{0,0},{1,0}},{{0,0},{0,0},{0,0},{0,0}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},1,2},
  {{{{-1,-1},{-1,0},{0,0},{1,0}},{{-1,1},{0,1},{0,0},{0,-1}},
  {{-1,0},{0,0},{1,0},{1,1}},{{1,-1},{0,-1},{0,0},{0,1}}},4,3},
  {{{{-1,0},{0,0},{0,1},{1,1}},{{0,-1},{0,0},{-1,0},{-1,1}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,4},
  {{{{-1,0},{0,0},{1,0},{1,-1}},{{-1,-1},{0,-1},{0,0},{0,1}},
  {{-1,1},{-1,0},{0,0},{1,0}},{{0,-1},{0,0},{0,1},{1,1}}},4,5},
  {{{{-1,1},{0,1},{0,0},{1,0}},{{0,-1},{0,0},{1,0},{1,1}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,6},
  {{{{-1,0},{0,0},{1,0},{0,-1}},{{0,-1},{0,0},{0,1},{-1,0}},
  {{-1,0},{0,0},{1,0},{0,1}},{{0,-1},{0,0},{0,1},{1,0}}},4,7}
};

void make_block(int n, uint16_t color){          // Make Block color       
  for (int i=0; i<12; i++) for(int j=0; j<12; j++){
    BlockImage[n][i][j] = color;                 // Block color
    if (i == 0 || j == 0) BlockImage[n][i][j]=0; // BLACK Line
  } 
}

void PutStartPos() {
  pos.X = 4; pos.Y = 1;
  if (nextBlockType == -1){ block = blocks[random(7)];
  }else{                    block = blocks[nextBlockType];
  }
  nextBlockType = random(7);
  rot = random(block.numRotate);
}

void Draw() { // Draw 120x240 in the center
  for (int i = 0; i < Width; ++i) for (int j = 0; j < Height; ++j)
   for (int k = 0; k < Length; ++k) for (int l = 0; l < Length; ++l)
    backBuffer[j*Length+l][i*Length+k]=BlockImage[screen[i][j]][k][l];
    tft.pushImage(100,0,120,240,(uint16_t *)backBuffer); // draw a block
}

void DrawNextBlock() {  
  for(int x = 0; x < 48;x++) {
    for(int y = 0; y < 60; y++){nextBlockBuf[y][x] = 0;}
  }
  nextBlock = blocks[nextBlockType];
  int offset = 6 + 12;
  for (int i = 0; i < 4; ++i) {
    for (int k=0; k<Length; ++k) for (int l=0; l<Length; ++l){
      nextBlockBuf[60 - (nextBlock.square[0][i].X * Length + l + 
      offset)][nextBlock.square[0][i].Y * Length + k + offset] = 
      BlockImage[nextBlockType + 1][k][l];
    }
  }
  tft.pushImage(26, 100, 48, 60, (uint16_t *)nextBlockBuf);
  tft.fillRect ( 5,  76, 92, 19, TFT_BLACK);
  tft.setCursor(12, 82); tft.printf("%7d",score); // score
}

bool GetSquares(Block block,Point pos,int rot,Point* squares){
  bool overlap = false;
  for (int i = 0; i < 4; ++i) {
    Point p;
    p.X = pos.X + block.square[rot][i].X;
    p.Y = pos.Y + block.square[rot][i].Y;
    overlap |= p.X < 0 || p.X >= Width || p.Y < 0 
            || p.Y >= Height || screen[p.X][p.Y] != 0;
    squares[i] = p;
  }
  return !overlap;
}

void GameOver() {
  for (int i=0; i<Width; ++i) for (int j=0; j<Height; ++j)
  if (screen[i][j] != 0) screen[i][j] = 4;
  gameover = true;
}
 
void ClearKeys(){b_CTR=false;b_LEFT=false;b_RIGHT=false;b_DOWN=false;}

bool KeyPadLoop(){
  if(digitalRead(WIO_KEY_C)==LOW || digitalRead(WIO_5S_LEFT)==LOW)
    {
    ClearKeys(); b_LEFT =true; delay( 50); return true;
    } // LEFT
  if(digitalRead(WIO_KEY_A)==LOW || digitalRead(WIO_5S_RIGHT)==LOW){
    ClearKeys(); b_RIGHT=true; delay( 50); return true;
    } // RIGHT
  if(digitalRead(WIO_KEY_B)==LOW || digitalRead(WIO_5S_PRESS)==LOW){
    ClearKeys(); b_CTR  =true; delay(250); return true;
    } // CENTER
  if(digitalRead(WIO_5S_UP)==LOW){                       // PAUSE
    tft.setCursor(148, 10); tft.println("PAUSE");
    while(digitalRead(WIO_5S_DOWN)==HIGH){};ClearKeys(); delay(250); 
    tft.fillRect (148,0,30,10,TFT_BLACK);return true;
    }   // RE-START 
  return false;
}

void GetNextPosRot(Point* pnext_pos, int* pnext_rot) {
  bool received = KeyPadLoop();
  if (b_CTR) started = true;
  if (!started) return;
  pnext_pos->X = pos.X;
  pnext_pos->Y = pos.Y;
  if ((fall_cnt = (fall_cnt + 1) % 10) == 0) pnext_pos->Y += 1;
  else if (received) {
    if (b_LEFT) { b_LEFT = false; pnext_pos->X -= 1;}
    else if (b_RIGHT) { b_RIGHT = false; pnext_pos->X += 1;}
    else if (b_DOWN)  { b_DOWN  = false; pnext_pos->Y += 1;}
    else if (b_CTR) { b_CTR = false;
     *pnext_rot=(*pnext_rot+block.numRotate-1)%block.numRotate; 
    }
  }
}
//========================================================================
void DeleteLine() {
  int deleteCount = 0;
  for (int j = 0; j < Height; ++j) {
    bool Delete = true;
    for (int i=0; i<Width; ++i) if (screen[i][j]==0) Delete=false;
    if (Delete) {
     for (int k = j; k >= 1; --k) {
      for (int i=0; i<Width; ++i) { screen[i][k]=screen[i][k-1];}
     }
     deleteCount++;
   }
  }
  switch (deleteCount){
    case 1:score = score +   40; break;
    case 2:score = score +  100; break;
    case 3:score = score +  300; break;
    case 4:score = score + 1200; break;
  }
  if(score > 9999999){ score = 9999999;}
}
//========================================================================
void ReviseScreen(Point next_pos, int next_rot) {
  if (!started) return;
  Point next_squares[4];
  for (int i = 0; i < 4; ++i) screen[pos.X + 
    block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = 0;
  if (GetSquares(block, next_pos, next_rot, next_squares)) {
   for (int i = 0; i < 4; ++i){
     screen[next_squares[i].X][next_squares[i].Y] = block.color;
   }
   pos = next_pos; rot = next_rot;
  }else{
   for (int i = 0; i < 4; ++i) screen[pos.X + 
    block.square[rot][i].X][pos.Y+block.square[rot][i].Y]=block.color;
    if (next_pos.Y == pos.Y + 1) {
    DeleteLine(); PutStartPos();DrawNextBlock();
     if (!GetSquares(block, pos, rot, next_squares)) {
     for (int i = 0; i < 4; ++i) screen[pos.X + 
     block.square[rot][i].X][pos.Y+block.square[rot][i].Y]=block.color;
     GameOver();
     }
    }
   }
  Draw();
}

void setup_tetris(void) {
  tft.fillScreen(TFT_BLACK);    // CLEAR SCREEN
  //----------------------------// Make Block ----------------------------
  make_block( 0, TFT_BLACK);    // Type No, Color
  make_block( 1, 0x00F0);       // DDDD     RED
  make_block( 2, 0xFBE4);       // DD,DD    PUPLE 
  make_block( 3, 0xFF00);       // D__,DDD  BLUE
  make_block( 4, 0xFF87);       // DD_,_DD  GREEN 
  make_block( 5, 0x87FF);       // __D,DDD  YELLO
  make_block( 6, 0xF00F);       // _DD,DD_  LIGHT GREEN
  make_block( 7, 0xF8FC);       // _D_,DDD  PINK
  //----------------------------------------------------------------------
  tft.pushImage(0,0,320,240,(uint16_t *)tetris_image); // background
  PutStartPos();  // Start Position
  for (int i = 0; i < 4; ++i) 
    screen[pos.X + block.square[rot][i].X][pos.Y+block.square[rot][i].Y]=block.color;
  Draw();         // draw a block
  DrawNextBlock();
}

void loop_tetris() {
  if (gameover) return;
  Point next_pos;
  int next_rot = rot;
  GetNextPosRot(&next_pos, &next_rot);
  ReviseScreen(next_pos, next_rot);
  delay(game_speed);  // Speed Adjust
}

