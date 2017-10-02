/* Example 6.4.1
   Copyright(c) 2008, S. D. Rajan
   Object-Oriented Numerical Analysis

   OBJECTIVES
   ----------
   (1) Illustrate how to use user-defined functions with math libraries
   (2) Illustrate (1) via Newton-Raphson technique

*/
#include <iostream>
#include "newtonraphson.h"

void MyFunction (double dX, double& dFX, double& dDX)
{
    // function
    dFX = (dX-2.3)*(dX+4.56)*(dX-3.7);

    // first derivative
    dDX = (dX+4.56)*(dX-3.7) +
          (dX-2.3)*(dX-3.7)  +
          (dX-2.3)*(dX+4.56);
}

int main ()
{
    const int    MAXITERATIONS  = 100;
    const double CONVERGENGETOL = 1.0e-6;
    double dRoot;
    
    // get initial guess from user
    std::cout << "Initial guess: ";
    std::cin >> dRoot;

    // get the root
    if (NewtonRaphson (dRoot, MAXITERATIONS, CONVERGENGETOL, MyFunction))
        std::cout << "Root is : " << dRoot << '\n';
    else
        std::cout << "Cannot find the root\n";

    return 0;
}