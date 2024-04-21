#include <iostream>
#include "Vec2.h"
#include "raylib.h"
#include "Shape.h"
#include "Components.h"

int main()
{
    Color darkGreen = Color{ 20, 160, 133, 255 };

    const int screenWidth = 800;
    const int screenHeight = 600;

    auto lol = CShape(10, 10, WHITE);
    Vec2 vec1(100, 100);

    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(darkGreen);
        lol.rectangle.Draw(vec1);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
