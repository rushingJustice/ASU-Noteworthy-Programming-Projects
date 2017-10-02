/* EXAMPLE 5.1.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Show how machine epsilon can be estimated.

*/
#include <iostream>
#include <iomanip>
#include <cmath>

int main ()
{
    float fEP=1.0f, fX;
    int n = 0;
    std::cout << std::setprecision(15) << std::scientific;

    for (;;)
    {
        fX = 1.0f + fEP;
        std::cout << std::right
                  << std::setw(20) << "fEP = " << fEP << ". n = "
                  << std::left << std::setw(20) << n << "\n";
        if (fX == 1.0f)
            break;
        fEP = fEP/2.0f;
        ++n;
    }

    std::cout << "\n\nEstimate of machine epsilon : " << fEP << "\n\n";

    return 0;
}

