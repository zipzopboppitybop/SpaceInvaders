#pragma once
#include "raylib.h"
#include "Vec2.h"

class Shape
{
public:
    int width = 0;
    int height = 0;
    Color color = WHITE;

    Shape();
    Shape(int w, int h, Color c);

    void Draw(const Vec2& pos);
};
