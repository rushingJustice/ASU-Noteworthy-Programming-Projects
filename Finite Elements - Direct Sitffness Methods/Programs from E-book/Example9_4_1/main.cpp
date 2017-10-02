/* EXAMPLE 9.4.1
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of CVector class.
   (2) Show how function calls can be made
   (3) Develop a non-class functions for vector display and addition

*/

#include <iostream>
#include <string>
#include <iomanip>
#include "../library/vectortemplate.h"

void Display (const std::string& szHeader, const CVector<float>& fV)
{
    std::cout << szHeader << "\n";
    for (int i=1; i <= fV.GetSize(); i++)
    {
        std::cout << "(" << std::setw(2) << i << "): "
            << fV(i) << "\n";
    }
}

int AddVectors (const CVector<float>& fV1, const CVector<float>& fV2, 
                CVector<float>& fV3)
{
    if ((fV1.GetSize() != fV2.GetSize()) ||
        (fV1.GetSize() != fV3.GetSize()))
        return 1;

    for (int i=1; i <= fV1.GetSize(); i++)
        fV3(i) = fV1(i) + fV2(i);

    return 0;
}

int main ()
{
    CVector<float> fVA(3), fVB(3); // define vectors with 3 rows/columns
    fVA.Set (0.0f);                // clear the vectors to zero
    fVB.Set (0.0f);                // clear the vectors to zero

    CVector<float> fVC(3); // define a vector with 3 rows/columns

    // set the values for vectors A and B
    for (int i=1; i <= 3; i++)
    {
        fVA(i) = static_cast<float>(i);
        fVB(i) = static_cast<float>(2*i);
    }
    Display ("Vector A", fVA);
    Display ("Vector B", fVB);

    // add the vectors with the result in vector C
    AddVectors (fVA, fVB, fVC);
    Display ("Vector C", fVC);

    // the following statement will lead to 
    // an assertion failure during execution (debug version)
    fVC(4) = 10.0f;

    return 0;
}
