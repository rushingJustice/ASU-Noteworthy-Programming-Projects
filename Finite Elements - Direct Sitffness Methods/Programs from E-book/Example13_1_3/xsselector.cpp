/*********************************************
Example 13.1.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include "XSSelector.h"
#include "math.h"

CXSSelector::CXSSelector ()
{
}

CXSSelector::~CXSSelector ()
{
}

int CXSSelector::GetXSection (const CXSDatabase& XSDatabase,
        CISection& ISection, float fMMax, float fTMax,
        float fCMax)
{
    std::string szID;
    float fArea, fSyy, fSzz, fWeight;
    
    for (int i=1; i <= XSDatabase.GetSize(); i++)
    {
        // get x/s properties
        ISection = XSDatabase.GetOne(i-1);
        ISection.GetProperties (szID, fArea, fSyy, fSzz, fWeight);

        // check tensile stress requirement
        float fTSMax = fabs(fMMax/fSyy) + fabs(fTMax/fArea);
        
        // check compressive stress requirement
        float fCSMax = fabs(fMMax/fSyy) + fabs(fCMax/fArea);

        // check
        if (fTSMax <= 20000.0f && fCSMax <= 20000.0f)
            return (1);
    }

    // not found
    return 0;
}
