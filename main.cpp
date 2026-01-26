#include <stdio.h>
#include <iostream>
#include "include/raylib.h"
#include "include/raymath.h"
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(750, 750, "Snake Game");
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