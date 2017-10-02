/* EXAMPLE 8.3.1
   Copyright(c) 2002-05, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Dynamic memory allocation.
   (2) Illustrate use of new and delete.

*********************************************/
#include <iostream>
#include "..\Example7_2_1\point.h"

int main ()
{
    CPoint *pPoint12;   // pointer to an object of class CPoint

    // define an object of class CPoint
    // memory is allocated
    pPoint12 = new CPoint;    // default constructor is called

    // set the x and y coordinate values
    pPoint12->SetValues (1.2f, -17.65f);

    // display the coordinate values
    pPoint12->Display ("Point 12 ");

    // release the memory
    delete pPoint12;

    return 0;
}
