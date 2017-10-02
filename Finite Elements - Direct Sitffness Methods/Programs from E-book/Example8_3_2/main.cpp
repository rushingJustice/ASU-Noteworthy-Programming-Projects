/* EXAMPLE 8.3.2
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Function call via pointers
   (2) Pointer arithmetic

*********************************************/
#include <iostream>

void CallViaPointers (const int *nA, int *nB)
{
    // set B as two times A
    *nB = 2*(*nA);
}

int main ()
{
    // function call
    int nA = 100;
    int nB;         // undefined value

    std::cout << "Before function call\n"
              << "nA = " << nA << "   "
              << "nB = " << nB << "\n";
    CallViaPointers (&nA, &nB);
    std::cout << "After function call\n"
              << "nA = " << nA << "   "
              << "nB = " << nB << "\n";
    CallViaPointers (&nA, &nB);

    // pointer arithmetic
    int i;
    double dVX[4] = {101.0, 102.0, 103.0, 104.0};
    double *pdVX = dVX; // address of the first location in dVX

    // display all the four values
    std::cout << "\nValues in vector X\n";
    for (i=0; i < 4; i++)
    {
        std::cout << "Location " << i << ": "
                  << *(pdVX+i) << "\n";
    }

    return 0;
}