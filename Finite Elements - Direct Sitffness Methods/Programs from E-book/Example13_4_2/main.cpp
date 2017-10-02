/* Example 13.4.2
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis
*/

#include <iostream>
#include "myaddlibrary.h"

int main ()
{
    // define a pointer variable to a function 
    // with a specified signature
    float (CMyAddLibrary::*ptAddFunction) (float, float) = NULL;
    CMyAddLibrary MAL1;

    // assign the address of the function
    ptAddFunction = &CMyAddLibrary::AddSimpleTwo;
    // use the function
    std::cout << "Simple addition gives " 
              << (MAL1.*ptAddFunction)(2.3f, 3.4f) << '\n';

    // pass the pointer variable as an argument
    MAL1.Use_In_A_Function (ptAddFunction);

    // reassign and reuse
    ptAddFunction = &CMyAddLibrary::AddSquareTwo;
    MAL1.Use_In_A_Function (ptAddFunction);

    return 0;
}


