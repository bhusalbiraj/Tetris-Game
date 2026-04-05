#include "raylib.h"  
#include <stdio.h>
#include <stdbool.h>
#include<string.h>
#include "shapes.c"
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
const int cellSize = 40;
#include "initialization.c" //all the global variables are declared here
#include "functions.c"

int main(){

    InitWindow(GRID_WIDTH * cellSize, GRID_HEIGHT * cellSize, "Tetris Game"); // Initialize the window (Width, Height, Title)
    SetTargetFPS(60); // Set the target frames per second
    
    SpawnNewPiece(currentPiece, &pX, &pY);
    GetHighScore();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {       

            //take input from the user first and then only go to the game section
        if(doneWithUserInput == false){

            if(IsKeyPressed(KEY_ENTER)){
                doneWithUserInput = true;
                continue;
            } 

            int key = GetCharPressed();
            while(key > 0){ //until the character queue is finished
                if(key >= 32 && key <= 125){
                    name[letterCount++] = (char)key; //casting ascii value to characters
                    name[letterCount] = '\0';
                }
                key = GetCharPressed(); //if user presses multiple characters really fast then we gotta take other characters stored in the queue
            }

                if(IsKeyPressed(KEY_BACKSPACE)){
                    name[--letterCount] = '\0'; //letterCount index points to the vacant space
                }

            BeginDrawing();
            ClearBackground(WHITE);
            DrawText("Enter your name", 1 * cellSize, 8 * cellSize, 30, BLACK);   //text format, x_pos, y_pos, font_size, color
            DrawText(name, 2 * cellSize, 9 * cellSize, 30, GREEN);
            EndDrawing();
            continue;
        }

        if(isGameOver()){
            AddToFile(name, score);
            if(IsKeyPressed(KEY_R)){
                GetCharPressed(); //take the r out of the state queue so that it wont appear in the enter your name window
                RestartGame();
                continue;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            drawGrid();
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
            DisplayEndingPage(score, high_score);
            SortLeaderBoard(); //runs only once for each game
            DisplayLeaderBoard();
            EndDrawing();
            continue;
        }
        if(score > high_score){
            UpdateHighScore();
        }


        timer += GetFrameTime();
        if(erased) added_score_timer += GetFrameTime();

        if (IsKeyPressed(KEY_LEFT)) {
        if (!CheckCollision(currentPiece, grid, -1, 0)) pX--;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (!CheckCollision(currentPiece, grid, 1, 0)) pX++;
        }
        if (IsKeyPressed(KEY_UP)){
            //if(!CheckCollision(currentPiece, grid, 0, 0)){
                rotateClockwise(currentPiece);
            //}
        }

    // Automatic Vertical Movement
        if (timer >= moveSpeed || IsKeyDown(KEY_DOWN)) {
            if (!CheckCollision(currentPiece, grid, 0, 1)) {
                pY++;
            } 
            else {
                // If it can't move down, it has landed
                LockPieceIntoGrid(currentPiece, pX, pY, grid); 
                updateGrid(grid);
                SpawnNewPiece(currentPiece, &pX, &pY);
            }
            timer = 0.0f; // Reset the clock
        }
            
            BeginDrawing();
            ClearBackground(BLACK);
            drawGrid();
            DrawText(TextFormat("SCORE: %i", score), 10, 10, 24, BLUE);
            DrawText(TextFormat("RECORD: %i", high_score), 10, 65, 22, GOLD);
            if(erased == true){
                DrawText(TextFormat("+%i", score - prev_score), 110, 34, 24, GREEN);
                if(added_score_timer > added_score_timer_limit){
                    erased = false;
                    added_score_timer = 0.0f; //reset the timer
                }

            }
            drawShape(currentPiece , pX , pY , cellSize , currentColor);
            drawNextShape();
            
        EndDrawing(); 
    
    }
    return 0;
}

// gcc main.c -o game \
// -I/opt/homebrew/include \
// -L/opt/homebrew/lib \
// -lraylib \
// -framework OpenGL \
// -framework Cocoa \
// -framework IOKit \
// -framework CoreVideo

// ./game