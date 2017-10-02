/*********************************************
Example 13.1.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_CXSSELECTOR_H__
#define __RAJAN_CXSSELECTOR_H__

#include "../Example13_1_1/ISection.h"
#include "../Example13_1_2/XSDatabase.h"

class CXSSelector
{
    public:
        CXSSelector ();
        ~CXSSelector ();
        // accessor functions
        int GetXSection (const CXSDatabase&,
                         CISection&, float, float, float);
};
#endif