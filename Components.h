#pragma once

#include "Vec2.h"
#include "raylib.h"
#include "Shape.h"

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
    Shape rectangle = Shape();

    CShape(int width, int height, Color color)
        : rectangle(width, height, color) {}
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