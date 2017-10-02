/*********************************************
Example 13.3.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include "../library/vectortemplate.h"
#include "isection.h"
#include "rectsolid.h"

int main ()
{
    const int NUMSECTIONS = 2;
    CVector<CXSType*> pVXSections(NUMSECTIONS);
    CVector<float> fV(4);

    // define the I-Section dimensions
    fV(1) = 19.77f;     // web height 
    fV(2) = 0.91f;      // web thickness
    fV(3) = 12.575f;    // flange width
    fV(4) = 1.63f;      // flange thickness

    pVXSections(1) = new CISection ("W21x201", fV);
    pVXSections(1)->DisplayDimensions ();
    pVXSections(1)->DisplayProperties ();

    // define the Rect Solid dimensions
    fV(1) = 20.0f;  // height
    fV(2) = 10.0f;  // width

    pVXSections(2) = new CRectSolid ("R20x10", fV, 10.5f);
    pVXSections(2)->DisplayDimensions ();
    pVXSections(2)->DisplayProperties ();

    std::cout << std::endl;
    for (int i=1; i <= NUMSECTIONS; i++)
    {
        if (CRectSolid *pRS = dynamic_cast<CRectSolid *>(pVXSections(i)))
            std::cout << "Deleting rectangular solid section ...\n";
        else if (CISection *pIS = dynamic_cast<CISection *>(pVXSections(i)))
            std::cout << "Deleting I-section ...\n";

        delete pVXSections(i);
    }

    return 0;
}