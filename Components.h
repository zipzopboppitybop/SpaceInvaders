#pragma once

#include "Vec2.h"
#include "raylib.h"

class CTransform
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 velocity = { 0.0, 0.0 };
    float angle = 0;

    CTransform(const Vec2& p, const Vec2& v, float a)
        : pos(p), velocity(v), angle(a) {}
};

class CShape
{
public:
    Rectangle rectangle = { 0 };

    float x, y, width, height;
    Color color = WHITE;

    CShape(float posX, float posY, float w, float h, Color c)
        : x(posX), y(posY), width(w), height(h), color(c)
    {
        rectangle.x = x;
        rectangle.y = y;
        rectangle.width = width;
        rectangle.height = height;
    }
};

class CCollision
{
public:
    float radius = 0;
    CCollision(float r)
        : radius(r) {}
};

class CScore
{
public:
    int score = 0;
    CScore(int s)
        : score(s) {}
};

class CLifeSpan
{
public:
    int remaining = 0; // amount of life remaing on entity
    int total = 0;     // total initial amount of lifespan
    CLifeSpan(int total)
        : remaining(total), total(total) {}
};

class CInput
{
public:
    bool left = false;
    bool right = false;
    bool shoot = false;

    CInput() {}
};