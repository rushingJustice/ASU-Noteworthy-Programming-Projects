/*********************************************
Example 13.3.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include "../library/vectortemplate.h"
#include "isection.h"
#include "rectsolid.h"

int main ()
{
    CVector<float> fV(4);

    // define the I-Section dimensions
    fV(1) = 19.77f;     // web height 
    fV(2) = 0.91f;      // web thickness
    fV(3) = 12.575f;    // flange width
    fV(4) = 1.63f;      // flange thickness

    CISection ISection1 ("W21x201", fV);
    ISection1.DisplayDimensions ();
    ISection1.CXSType::DisplayDimensions ();
    ISection1.DisplayProperties ();

    // define the Rect Solid dimensions
    fV(1) = 20.0f;  // height
    fV(2) = 10.0f;  // width

    CRectSolid RectSolid1 ("R20x10", fV);
    RectSolid1.DisplayDimensions ();
    RectSolid1.DisplayProperties ();

    return 0;
}