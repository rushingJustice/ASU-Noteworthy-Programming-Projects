/* EXAMPLE 2.8.1
   Copyright(c) 2008-10, S. D. Rajan
   Object-Oriented Numerical Analysis

   It may be more appropriate to revisit this example
   after finishing Chapter 4 (modular program development
   and functions) 

  OBJECTIVES
   (1) Illustrate how to define a user-defined namespace
   (2) Illustrate how to use a user-defined namespace

*/
#include <iostream>
#include "mynamespace.h"

int main ()
{
    double dX;
    std::cout << "Input a number: ";
    std::cin >> dX;

    std::cout << "Square of " << dX << " is " 
              << MyNameSpace::MySquare (dX) << '\n';
    std::cout << "Cube of " << dX << " is " 
              << MyNameSpace::MyCube (dX) << '\n';

    return 0;
}
