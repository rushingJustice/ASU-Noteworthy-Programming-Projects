/* EXAMPLE 8.2.1
   Copyright(c) 2002-05, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of pointers.
   (2) Pointer arithmetic.

*********************************************/
#include <iostream>

void ShowValues (int nV, int* pnV)
{
    std::cout << "\n";
    std::cout << "                     nV is " << nV   << "\n";
    std::cout << "          pnV is (address) " << pnV  << "\n";
    std::cout << "Value pointed to by pnV is " << *pnV << "\n";
}

int main ()
{
    int nV1;     // an integer variable
    int *pnV1;   // pointer to an integer
    int *pnV2;   // pointer to an integer

    nV1 = 10;    // set the value
    pnV1 = &nV1; // set the address
    ShowValues (nV1, pnV1);

    // manipulate the value
    *pnV1 = 100;    // changes value of nV1!
    ShowValues (nV1, pnV1);

    // pointer arithmetic
    pnV2 = pnV1;    // sets address of pnV2
    *pnV2 = 400;    // changes value of nV1!
    ShowValues (nV1, pnV1);

    return 0;
}
