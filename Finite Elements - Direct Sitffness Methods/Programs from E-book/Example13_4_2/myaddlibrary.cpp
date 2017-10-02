/* Example 13.4.2
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis
*/

#include <iostream>
#include "myaddlibrary.h"

CMyAddLibrary::CMyAddLibrary ()
{
}

CMyAddLibrary::~CMyAddLibrary ()
{
}

float CMyAddLibrary::AddSimpleTwo (float fA, float fB)
{
    return (fA + fB);
}

float CMyAddLibrary::AddSquareTwo (float fA, float fB)
{
    return (fA*fA + fB*fB);
}

void CMyAddLibrary::Use_In_A_Function (float (CMyAddLibrary::*ptrFunc)(float fA, float fB))
{
    // use with comparison operators
    if (!ptrFunc)
        std::cout << "Add function is not initialized.\n";
    else
    {
        if (ptrFunc == &CMyAddLibrary::AddSimpleTwo)
            std::cout << "Variable points to member function AddSimpleTwo\n";
        else if (ptrFunc == &CMyAddLibrary::AddSquareTwo)
            std::cout << "Variable points to member function AddSquareTwo\n";
    }
    std::cout << "Function call gives " 
              << (*this.*ptrFunc)(2.3f, 3.4f) << '\n';
}
