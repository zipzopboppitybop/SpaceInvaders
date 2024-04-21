#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{

}

Vec2::Vec2(float xin, float yin)
    : x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x * val, y * val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return (x != rhs.x && y != rhs.y);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    x *= val;
    y *= val;
}

void Vec2::operator /= (const float val)
{
    x /= val;
    y /= val;
}

float Vec2::dist(const Vec2& rhs) const
{
    float diffY = y - rhs.y;
    float diffX = x - rhs.x;
    return sqrt((diffY * diffY) + (diffX * diffX));
}

float Vec2::angle(const Vec2& rhs) const
{
    float diffY = y - rhs.y;
    float diffX = x - rhs.x;
    return atan2f(diffY, diffX);
}

Vec2 Vec2::normalize(const Vec2& rhs) const
{
    float diffY = y - rhs.y;
    float diffX = x - rhs.x;
    float length = sqrt((diffY * diffY) + (diffX * diffX));

    return Vec2(diffX / length, diffY / length);
}