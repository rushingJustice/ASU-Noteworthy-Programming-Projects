/* EXAMPLE 9.2.1
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of static member variables.
   (2) Illustrate usage of static member functions.

*/
#include <iostream>
#include "math.h"

int main ()
{
    std::cout << "1.0 + 2.0 = " << CMath::Add (1.0, 2.0)      << '\n';
    std::cout << "1.0 - 2.0 = " << CMath::Subtract (1.0, 2.0) << '\n';
    std::cout << "1.0 * 2.0 = " << CMath::Multiply (1.0, 2.0) << '\n';
    std::cout << "1.0 / 2.0 = " << CMath::Divide (1.0, 2.0)   << '\n';

    CMath::DisplayCounters ();

    return 0;
}