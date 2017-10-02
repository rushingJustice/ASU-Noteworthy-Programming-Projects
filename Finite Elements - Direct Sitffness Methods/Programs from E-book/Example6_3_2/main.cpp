/* EXAMPLE 6.3.2
   Copyright(c) 2002-08, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate integration via Trapezoidal Rule
   (2) Illustrate integration via Simpson's Rule

*********************************************/
#include <iostream>
#include <iomanip>
#include <cmath>

double Func (double x) //function to integrate
{
    return pow(x,4);
}

int main ()
{
    const int nLow=1, nHigh=1024;          // controls n
    const double dLow = -1.0, dHigh = 1.0; // integ. limits
    double dTrapRule, dSimpsonRule, dFactor;

    // output table heading
    std::cout <<           "   n " << "   "
              << "     Trapezoid " << "   "
              << "       Simpson " << '\n';

    for (int n=nLow; n <= nHigh; n*=2)
    {
        double h = (dHigh-dLow)/static_cast<double>(n);
        dTrapRule = dSimpsonRule = Func(dLow); // left-end

        for (int i=2; i <= n; i++) // internal points
        {
            double dX = dLow + static_cast<double>(i-1)*h;
            dFactor = 2.0;
            dTrapRule += dFactor*Func(dX);
            dFactor = (i%2 == 0? 4.0 : 2.0);
            dSimpsonRule += dFactor*Func(dX);
        }

        dTrapRule += Func(dHigh);     // right-end
        dTrapRule *= (h/2.0);
        dSimpsonRule += Func(dHigh);  // right-end
        dSimpsonRule *= (h/3.0);

        // show the results
        std::cout << std::setw(4)  << n            << "   "
                  << std::setw(15) << dTrapRule    << "   "
                  << std::setw(15) << dSimpsonRule << '\n';
    }
    std::cout << std::endl;

    return 0;
}