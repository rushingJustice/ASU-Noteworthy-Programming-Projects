/*********************************************
Example 13.4.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include "Math2Lib.h"

int main ()
{
    // 1. instantiate objects (specific math functions)
    CAddSimple2<float> OSimple2;
    CAddSquare2<float> OSquare2;

    // 2. instantiate objects ...
    //    functor which encapsulates pointer to object 
    //    and to specific member function
    CMath2Lib<CAddSimple2<float>, float> 
              simple2(&OSimple2, &CAddSimple2<float>::AddThem);
    CMath2Lib<CAddSquare2<float>, float>
              square2(&OSquare2, &CAddSquare2<float>::AddThem);

    // 3. make array with pointers to FunctorABC, the base class, 
    //    and initialize it
    FunctorABC<float>* TOF[] = {&simple2, &square2};
    enum                       {SIMPLE=0,   SQUARE};

    // 4. use array to call member functions without the need for an object
    std::cout << "AddSimple value : " 
              << (*TOF[SIMPLE]) (2.3f, 3.4f) << '\n'; 
    std::cout << "AddSquare value : " 
              << (*TOF[SQUARE]) (2.3f, 3.4f) << '\n'; 

    return 0;
}