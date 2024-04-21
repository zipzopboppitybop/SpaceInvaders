#include "Shape.h"

Shape::Shape()
{

}

Shape::Shape(int w, int h, Color c)
    : width(w), height(h), color(c)
{

}

void Shape::Draw(const Vec2& pos)
{
    DrawRectangle(pos.x, pos.y, width, height, color);
}