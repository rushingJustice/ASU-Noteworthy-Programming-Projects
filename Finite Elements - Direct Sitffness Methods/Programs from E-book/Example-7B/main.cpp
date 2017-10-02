#include <iostream>
#include "triangle.h"

int main ()
{
    CPoint P1 (0.0f, 0.0f), P2(10.0f, 0.0f), P3(5.0f, 8.66025f);
    CTriangle T1(P1, P2, P3);

    if (T1.IsEquilateral ())
        std::cout << "Triangle is equilateral.\n";
    else
        std::cout << "Triangle is NOT equilateral.\n";

    return 0;
}