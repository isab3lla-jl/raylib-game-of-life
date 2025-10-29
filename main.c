#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Local Variable and Functions
//------------------------------------------------------------------------------------
int grid_w, grid_h;
int sqr_len = 10;
static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h], bool isPaused);
static void RandomizeGrid(int w, int h, int (*a)[h]);
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib game of life");

    grid_w = screenWidth/sqr_len;
    grid_h = screenWidth/sqr_len;

    int state_a[grid_w][grid_h];
    int state_b[grid_w][grid_h];

    RandomizeGrid(grid_w, grid_h, state_a);
    
    SetTargetFPS(9);

    bool isPaused = false;
    //--------------------------------------------------------------------------------------

    // Main game loop
    int n = 1;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }

        if (IsKeyPressed(KEY_R) && !isPaused)
        {
            RandomizeGrid(grid_w, grid_h, n ? state_a : state_b);
        }

        if (!isPaused)
        {
            UpdateDrawFrame(n % 2, grid_w, grid_h, n ? state_a : state_b, n ? state_b : state_a, isPaused);
            n = !n;
        }
        else
        {
            UpdateDrawFrame(n % 2, grid_w, grid_h, n ? state_a : state_b, n ? state_b : state_a, isPaused);
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static void RandomizeGrid(int w, int h, int (*a)[h])
{
    srand(time(NULL));
    for(int row=0; row<w; row++)
    {
        for(int col=0; col<h; col++)
        {
            a[row][col] = rand()%2;
        }
    }
}

static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h], bool isPaused)
{
    // Update logic only runs if the game is not paused
    if (!isPaused)
    {
        for(int row=0; row<grid_w; row++)
        {
            for(int col=0; col<grid_h; col++)
            {
                int neighbors = 0;
                for(int drow=-1; drow<=1; drow++)
                {
                    for(int dcol=-1; dcol<=1; dcol++)
                    {
                        if(drow==0 && dcol==0) continue;
                        int nrow = row + drow;
                        int ncol = col + dcol;
                        if(nrow>=0 && nrow<grid_w && ncol>=0 && ncol<grid_h)
                        {
                            neighbors += a[nrow][ncol];
                        }
                    }
                }

                // Apply Conway's Game of Life rules
                if(a[row][col])
                {
                    if(neighbors < 2 || neighbors > 3)
                        b[row][col] = 0; // Cell dies
                    else
                        b[row][col] = 1; // Cell lives
                }
                else
                {
                    if(neighbors == 3)
                        b[row][col] = 1; // Cell becomes alive
                    else
                        b[row][col] = 0; // Cell remains dead
                }
            }
        }
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(BLACK);
        for(int row=0; row<grid_w; row++)
        {
            for(int col=0; col<grid_h; col++)
            {
                if(a[row][col])
                DrawRectangle(row*sqr_len+1,col*sqr_len+1, sqr_len-2, sqr_len-2, DARKPURPLE);
            }
        }

        // Draw the pause text when the game is paused
        if (isPaused)
        {
            const char* pauseText = "PAUSED";
            int textWidth = MeasureText(pauseText, 40);
            DrawText(pauseText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - 20, 40, LIGHTGRAY);
        }
    EndDrawing();
    //----------------------------------------------------------------------------------
}
