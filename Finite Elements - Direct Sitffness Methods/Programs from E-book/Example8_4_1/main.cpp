/* EXAMPLE 8.2.4
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) A poor man's vector class
   (2) Use of destructor
   (3) reference return type

*********************************************/
#include <iostream>
#include "myvector.h"

int main ()
{
    // dynamically allocated vectors
    CMyVector fVX(3), fVY(3);   
    
    // populate the two vectors
    int i;
    for (i=0; i < 3; i++)
    {
        // populate using the public member variable
        fVX.m_pData[i] = static_cast<float>(i+1);

        // now populate using the reference-based At() function
        fVY.At(i) = static_cast<float>((i+1)*(i+1));
    }

    // compute the dot product
    float fDotP = fVX.DotProduct (fVY);

    // display the result
    std::cout << "Dot product of ...\n";
    fVX.Display ("    Vector X ");
    fVY.Display ("    Vector Y ");
    std::cout << "    is equal to " << fDotP << "\n";

    return 0;
}