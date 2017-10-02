/* EXAMPLE 4.4.2
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Modular program development.
   (2) Illustrate the concept of global variables.

*/
// include files
#include <iostream>
#include "VectorOps.h"

// global variables
int nFLOPs; // nFLOPs is a global variable

int main ()
{
    // define vectors used in the program
    float fX[3]={1.0f, 2.0f, 3.0f};
    float fY[3]={4.0f, 5.0f, 6.0f};
    
    // initialize
    nFLOPs = 0;

    // carry out the vector operations
    int i;
    for (i=1; i <= 10000; i++)
    {
        float fDP = VectorDotProduct (fX, fY, 3);
        float fLengthX = VectorLength (fX, 3);
        float fLengthY = VectorLength (fY, 3);
    }

    // display the # of floating point operations
    std::cout << "Number of floating-point operations: " 
        << nFLOPs << "\n";

    return 0;
}