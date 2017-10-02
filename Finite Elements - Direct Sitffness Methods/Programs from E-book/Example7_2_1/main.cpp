/* EXAMPLE 7.2.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of a simple class.
   (2) Use of public member functions.

*********************************************/
#include <iostream>

#include "point.h"
#include "..\library\getinteractive.h"

int main ()
{
    // declaration invokes the default constructor
    CPoint Origin;
    // declaration invokes the overloaded constructor 
    CPoint CarCoords (0.0f, 0.0f);

    // display the coordinates of the origin
    CarCoords.Display ("Origin ");

	// get the coordinates from the user
    float fV[2];
    GetInteractive ("Enter car coordinates: ", fV, 2);
    // set the values via public member function SetValues
    CarCoords.SetValues (fV[0], fV[1]);

    // display the values
    CarCoords.Display ("Car ");

    return 0;
}