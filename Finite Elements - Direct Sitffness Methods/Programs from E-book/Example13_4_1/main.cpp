/* Example 13.4.1
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis

   Program to illustrate pointer to functions
   using ordinary functions and (class) member functions
*/

#include <iostream>

float AddSimpleTwo (float fA, float fB) { return (fA+fB); }
float AddSquareTwo (float fA, float fB) { return (fA*fA+fB*fB); }

void Use_In_A_Function (float (*ptrFunc)(float fA, float fB))
{
    // use with comparison operators
    if (!ptrFunc)
        std::cout << "Add function is not initialized.\n";
    else
    {
        if (ptrFunc == &AddSimpleTwo)
            std::cout << "Variable points to function AddSimpleTwo\n";
        else if (ptrFunc == &AddSquareTwo)
            std::cout << "Variable points to function AddSquareTwo\n";
    }
    std::cout << "Function call gives " 
              << ptrFunc(2.3f, 3.4f) << '\n';
}

int main ()
{
    // define a pointer variable to a function 
    // with a specified signature
    float (*ptAddFunction) (float, float) = NULL;

    // assign the address of the function
    ptAddFunction = &AddSimpleTwo;
    // use the function
    std::cout << "Simple addition gives " 
              << ptAddFunction(2.3f, 3.4f) << '\n';

    // pass the pointer variable as an argument
    Use_In_A_Function (ptAddFunction);

    return 0;
}


