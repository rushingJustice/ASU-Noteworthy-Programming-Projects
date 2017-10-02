/* EXAMPLE 5.1.3
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Learning to troubleshoot.
   (2) Understanding numerical errors.

*/

#include <iostream>
#include <cmath>
#include <cfloat>

int main ()
{
    float fX = -33.56f;
    float fY = 0.0f;

    float fA = fX/fY;       // divide by zero
    std::cout << fX << "/" << fY << " = " << fA << "\n";

    float fB = sqrt(fX);    // illegal operation
    std::cout << "sqrt(" << fX << ") = " << fB << "\n";

    double dU = 4.5e155, dV = 4.6e185;
    double dC =  dU * dV;   // overflow
    std::cout << dU << " * " << dV << " = " << dC << "\n";

    double dQ = 4.5e-135, dR = 4.6e-195;
    double dD =  dQ * dR;   // underflow
    std::cout << dQ << " * " << dR << " = " << dD << "\n";

    float fE = fY/fY;       // NaN (not a number)
    if (_isnan(fE))
        std::cout << fY << " / " << fY << " is not a number.\n";
    else
        std::cout << fY << " / " << fY << " = " << fE << "\n";

    return 0;
}