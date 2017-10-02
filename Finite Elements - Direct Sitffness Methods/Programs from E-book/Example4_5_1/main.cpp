/* EXAMPLE 4.5.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of function templates.

*/
#include <iostream>
#include "templates.h"

int main ()
{
    const int NUMELEMENTS = 5;

    // integer values
    int nValues[NUMELEMENTS];
    std::cout << "Input " << NUMELEMENTS << " integers.\n";
    GetValues (nValues, NUMELEMENTS);
    int nMinValue = MinValue (nValues, NUMELEMENTS);
    std::cout << "Minimum value is : " << nMinValue << "\n";

    // double values
    double dValues[NUMELEMENTS];
    std::cout << "Input " << NUMELEMENTS << " doubles.\n";
    GetValues (dValues, NUMELEMENTS);
    double dMinValue = MinValue (dValues, NUMELEMENTS);
    std::cout << "Minimum value is : " << dMinValue << "\n";

    return 0;
}
