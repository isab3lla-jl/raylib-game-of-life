/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute 'raylib_compile_execute' script
*   Note that compiled executable is placed in the same folder as .c file
*
*   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
*   Web version of the program is generated in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Local Variable and Functions
//------------------------------------------------------------------------------------
int grid_w, grid_h;
int sqr_len = 10;
static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h]);
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
    //--------------------------------------------------------------------------------------

    // Main game loop
    int n = 1;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if(IsKeyPressed(KEY_R))
        {
            RandomizeGrid(grid_w, grid_h, n?state_a:state_b);
        }
        UpdateDrawFrame(n%2,grid_w,grid_h, n?state_a:state_b, n?state_b:state_a);
        n = !n;
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

static void UpdateDrawFrame(int n, int w, int h, int (*a)[h], int (*b)[h])
{
    
    // Update
    //----------------------------------------------------------------------------------
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
    
    //----------------------------------------------------------------------------------

    
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
    EndDrawing();
    //----------------------------------------------------------------------------------
}