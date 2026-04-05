#include<stdbool.h>
#include "raylib.h"
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
extern const int cellSize;//GRID_HEIGHT, GRID_WIDTH;
extern const Color colors[];
extern int shapes[][4][4];
extern int grid[][10];
extern Color currentColor;

bool CheckCollision(int piece[4][4], int grid[20][10], int moveX, int moveY);
void rotateAntiClockwise(int shape[4][4]);

void drawShape(int matrix[4][4], int pX, int pY, int cellSize, Color color) {
    for (int y = 0; y < 4; y++) { // Loop through rows
        for (int x = 0; x < 4; x++) { // Loop through columns
            // If the value is 1, draw a block
            if (matrix[y][x] == 1) {
                int screenX = (pX + x) * cellSize;
                int screenY = (pY + y) * cellSize;
                Color border_color = ColorBrightness(currentColor, -0.4f);
                // Draw the actual rectangle
                DrawRectangle(screenX, screenY, cellSize, cellSize, color);
                DrawRectangleLinesEx((Rectangle){ screenX, screenY, cellSize, cellSize }, 6, border_color);
            }
        }
    }
}

void rotateClockwise(int shape[4][4]){
    //clock wise 90 degrees
    int original_shape[4][4];
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) original_shape[i][j] = shape[i][j];
    //transpose matrix at first
    for(int i = 0 ; i < 4 ; i++){
        for(int j = i ; j < 4 ; j++){
            int temp = shape[i][j];
            shape[i][j] = shape[j][i];
            shape[j][i] = temp;
        }
    }
    
    //reverse the rows left to right
    for(int i = 0; i < 4; i++)
    {   
        for(int j = 0; j < 2; j++){
            int temp = shape[i][j];
            shape[i][j] = shape[i][3 - j];
            shape[i][3-j] = temp;
        }
    }
    if(!CheckCollision(shape, grid, 0, 0)) return;
    int x_pos = pX;
    if(pX < 0){
            for(pX = x_pos; pX < x_pos + 3; pX++){
            
            if(!CheckCollision(shape, grid, 0, 0)){
                return;
                }
        } 
    }
    else{
            for(pX = x_pos; pX > x_pos - 3; pX--){
            
            if(!CheckCollision(shape, grid, 0, 0)){
                return;
            }
        }
    }
    pX = x_pos;
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) shape[i][j] = original_shape[i][j];
    return;

    
}

//CheckCollision(currentPiece, pX, pY, grid, 0, 1)
// Returns true if there is a collision, false if the path is clear
bool CheckCollision(int piece[4][4], int grid[20][10], int moveX, int moveY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // We only care about the '1's in our 4x4 piece
            if (piece[y][x] != 0) {
                // Calculate where this specific block WANT to go
                int nextX = pX + x + moveX;
                int nextY = pY + y + moveY;

                // 1. Check Horizontal Walls (Left and Right)
                if (nextX < 0 || nextX >= 10) {
                    return true; 
                }

                // 2. Check the Floor (Bottom)
                if (nextY >= 20) {
                    return true;
                }

                // 3. Check for Landed Blocks in the Grid
                // Only check if nextY is within the grid (ignore spawn area above y=0)
                if (nextY >= 0) {
                    if (grid[nextY][nextX] != 0) {
                        return true;
                    }
                }
            }
        }
    }
    // If we checked all necessary cells and none hit anything, it's safe!
    return false;
}

void LockPieceIntoGrid(int piece[4][4], int pX, int pY, int grid[20][10]) {
    int color_index = 0;
    for(int i = 0; i < 7; i++){
        if(ColorIsEqual(currentColor, colors[i]))
        {
            color_index = i;
            break;
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Only transfer the actual blocks, not the empty space (0s)
            if (piece[y][x] != 0) {
                
                // Calculate the final position on the grid
                int gridX = pX + x;
                int gridY = pY + y;

                // Safety Check: Ensure we aren't writing outside array bounds
                if (gridY >= 0 && gridY < 20 && gridX >= 0 && gridX < 10) {
                    grid[gridY][gridX] = color_index; 
                }
            }
        }
    }
}

void SpawnNewPiece(int currentPiece[4][4], int *pX, int *pY) {
    //makes the nextPiece the currentPiece and picks a new nextPiece
    *pX = 3; 
    *pY = -1;
    //gotta initialize newPiece for the first time when new game starts
    if(!firstNewPieceDone){
        memcpy(nextPiece, shapes[GetRandomValue(0,6)], 16 * sizeof(int));
        firstNewPieceDone = true;
    }
    currentColor = nextColor;
    nextColor = colors[GetRandomValue(1,6)];
   //Copy the nextPiece matrix to the currentPiece matrix
    memcpy(currentPiece, nextPiece, 16 * sizeof(int));

    //Get a random piece as our nextPiece
    memcpy(nextPiece, shapes[GetRandomValue(0,6)], 16 * sizeof(int));
}


void drawGrid() {

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            
            // Only draw if the cell is not empty
            if (grid[y][x] != 0) {
                Color border_color = ColorBrightness(colors[grid[y][x]], -0.4f);
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, colors[grid[y][x]]);
                DrawRectangleLinesEx((Rectangle){ x * cellSize, y * cellSize, cellSize, cellSize }, 6, border_color);
            }
            else{
                DrawRectangle(x * cellSize, y * cellSize, cellSize - 1, cellSize - 1, BLACK);
            }
        }
    }
}

void updateGrid(int grid[20][10]){
    int erased_line = 0;
    for(int y = 0; y < 20; y++){
        int count = 0;
        for(int x = 0; x < 10; x++){
            if(grid[y][x] != 0){
                count++;
            }
        }
        if(count == 10){
            erased_line++;
            for(int row = y - 1; row >= 0; row--){
                for(int col = 0; col < 10; col++){
                    grid[row + 1][col] = grid[row][col];
                }
            }
        }        
    }
    prev_score = score;
    if(erased_line) erased = true;
    switch(erased_line){
        
            case 1:
                score += 100;
                break;
            case 2:
                score += 300;
                break;
            case 3:
                score += 500;
                break;
            case 4:
                score += 800;
                break;
            default:
                score += 0;
                break;
        }
    
}

bool isGameOver(){
    int count = 0;
    for(int y = 0; y < 20; y++){
       bool non_empty = false;
        for(int x = 0; x < 10; x++){
            if(grid[y][x] != 0){
                non_empty = true;
            }
        }
        if(non_empty){
            count++;
        }
    }
    if(count == 20)
        return true;
    else    
        return false;
}

void GetHighScore(){
    //high_score = 3800;
    FILE* fptr = fopen("highscore.txt", "r");
    if(fptr != NULL){
        fscanf(fptr, "%*s %d", &high_score);   // %*s means ignore the string 
        fclose(fptr);
    }
    else{
        high_score = 0;
    }
    
}

void UpdateHighScore(){
    high_score = score;
}

void RestartGame(){
    score = 0;
    prev_score = 0;
    erased = false;
    doneWithUserInput = false;
    isFileAppended = false;
    loaded_for_leaderboard = false;
    loaded_for_sorting = false;
    strcpy(name, "");
    letterCount = 0;
    for(int y = 0; y < 20; y++) for(int x = 0; x < 10; x++) grid[y][x] = 0;

}

void DisplayEndingPage(int score, int high_score){
    int font_size = 60;
                const char* ending_text;
                if(score >= high_score)
                    ending_text = "NEW HI-SCORE";
                else    
                    ending_text = "GAME OVER";
                const char* high_score_text = TextFormat("High Score: %i", high_score);
                int ending_width = MeasureText(ending_text, 50);
                int high_score_width = MeasureText(high_score_text, 40);
                const char* score_text = TextFormat("Score: %i", score);
                int score_width = MeasureText(score_text, 50);
                const char* restart_text = "Press R to Restart";
                int restart_width = MeasureText(restart_text, 40);
                DrawText(ending_text, GetScreenWidth()/2 - ending_width/2, GetScreenHeight()/2 - 30 - 8.5 * cellSize, 50, RED);
                DrawText(score_text, GetScreenWidth()/2 - score_width/2, GetScreenHeight()/2 + 35 - 8.5 * cellSize, 50, GREEN);
                DrawText(high_score_text, GetScreenWidth()/2 - high_score_width/2, GetScreenHeight()/2 + 100 - 8.5 * cellSize, 40, GOLD);
                DrawText(restart_text, GetScreenWidth()/2 - restart_width/2, 14 * cellSize - 8.5 * cellSize, 40, GREEN);
}

void AddToFile(char* name, int score){
    if(isFileAppended == true)
        return;
    FILE* fptr = fopen("highscore.txt", "a");
    fprintf(fptr, "\n%s %d", name, score);
    fclose(fptr);
    
    isFileAppended = true;
}
 
void DisplayLeaderBoard(){
    static struct users{     //making this struct only once and reading from file only once so using static here 
        char name[50];
        int score;
    }user[5];
    //loaded_for_leaderboard = false;
    if(!loaded_for_leaderboard){
        FILE* fptr = fopen("highscore.txt", "r");
        int i = 0;
        while(fscanf(fptr, "%s %d", user[i].name, &user[i].score) == 2 && i < 5){ //as long as 2 items read each time
            i++;
        }
        fclose(fptr);
        loaded_for_leaderboard = true;
    }
    
    int x_pos = 0.3 * cellSize;
    int y_pos = 11 * cellSize;
    DrawRectangle(x_pos + 0.2 * cellSize, y_pos + 0.7 * cellSize, 9 * cellSize, 4 * cellSize, GRAY);
    DrawText("LEADERBOARD", GetScreenWidth() / 2 - MeasureText("LEADERBOARD", 34) / 2, y_pos, 34, RED);
    for(int i = 0; i < 5; i++){  
        DrawText(TextFormat("%2d. %-15s", i + 1, user[i].name), x_pos, y_pos + (i + 1) * 34, 34, BLACK);
        DrawText(TextFormat("%d", user[i].score), x_pos + 7 * cellSize, y_pos + (i + 1) * 34, 34, BLACK);
    }
}

void SortLeaderBoard(){
    static struct Users{     //making this struct only once and reading from file only once so using static here 
        char name[50];
        int score;
    }User[100], temp;
   // static bool loaded_for_sorting = false;
    if(!loaded_for_sorting){
        FILE* fptr = fopen("highscore.txt", "r");
        int i = 0;
        while(fscanf(fptr, "%s %d", User[i].name, &User[i].score) == 2){ //as long as 2 items read each time
            i++;
        }
        fclose(fptr);

        int n = i; //i = no. of total lines and n = last index
        for(i = 0; i < n - 1; i++){
            for(int j = i + 1; j < n; j++){
                if(User[i].score < User[j].score){
                    temp = User[i];
                    User[i] = User[j];
                    User[j] = temp;
                }
            }
        }

        FILE* fp = fopen("highscore.txt", "w");
        for(i = 0; i < n; i++){
            fprintf(fp, "%s %d\n", User[i].name, User[i].score);
        }
        fclose(fp);
        loaded_for_sorting = true;
    }
}

void drawNextShape() {
    int cellsize = cellSize / 2;
    int corner_x = 7.8 * cellSize;
    int corner_y = 0.3 * cellsize;
    DrawRectangle(7.6 * cellSize, 0, 5 * cellsize, 5 * cellsize, GRAY);
    for (int y = 0; y < 4; y++) { // Loop through rows
        for (int x = 0; x < 4; x++) { // Loop through columns
            // If the value is 1, draw a block
            if (nextPiece[y][x] == 1) {
                int screenX = corner_x + x * cellsize;
                int screenY = corner_y + y * cellsize;
                Color border_color = ColorBrightness(nextColor, -0.4f);
                // Draw the actual rectangle
                DrawRectangle(screenX, screenY, cellsize, cellsize, nextColor);
                DrawRectangleLinesEx((Rectangle){ screenX, screenY, cellsize, cellsize }, 2, border_color);
            }
        }
    }
}