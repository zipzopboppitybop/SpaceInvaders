#include <iostream>
#include "Vec2.h"

int main()
{
    Vec2 vec1(10, 10);
    Vec2 vec2(15, 15);
    vec1 += vec2;

    std::cout << vec1.x << ", " << vec1.y << std::endl;
    std::cout << vec2.x << ", " << vec2.y << std::endl;
}
