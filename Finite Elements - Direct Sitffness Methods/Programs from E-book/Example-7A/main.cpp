/*
    This example will find the real roots of a quadratic polynomial

*/
#include <iostream>
#include "roots.h"

int main ()
{
    double dVRoots[2];  // quadratic polynomial can have 2 real roots
    CQuadPoly QP (1.0, -4.7, 4.8);
                        // x^2 - 4.7*x + 4.8  (roots are 1.5 and 3.2)
    CRoots Roots;
    
    if (Roots.FindRoots (QP, dVRoots))
    {
        std::cout << "Roots are real and they are : "
                  << dVRoots[0] << ", " << dVRoots[1] << "\n";
    }
    else
        std::cout << "Unable to find the real roots.\n";

   return 0;
}
