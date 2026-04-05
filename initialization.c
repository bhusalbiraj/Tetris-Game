#include<string.h>
int grid[GRID_HEIGHT][GRID_WIDTH] = {0};
const Color colors[7] = { SKYBLUE, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED };
Color nextColor = YELLOW;
Color currentColor;
int currentPiece[4][4];
int nextPiece[4][4];

//gcc main.c -o main.exe -I C:/raylib/raylib/src -L C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
//.\main.exe
int pX, pY;
int score = 0, prev_score = 0;
bool erased = false;
bool doneWithUserInput = false;
bool isFileAppended = false;
bool loaded_for_leaderboard = false;
bool loaded_for_sorting = false;
bool firstNewPieceDone = false;
char name[50] = "\0";
int letterCount = 0;
int high_score;

bool game_over = false;
float timer = 0.0f;
float moveSpeed = 0.17f;
float added_score_timer = 0.0f;
float added_score_timer_limit = 1.0f;