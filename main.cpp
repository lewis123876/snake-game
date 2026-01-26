#include <stdio.h>
#include <iostream>
#include "include/raylib.h"
#include "include/raymath.h"
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(cellCount * cellSize, cellCount * cellSize, "Snake Game");
    SetTargetFPS(60);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        ClearBackground(green);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}