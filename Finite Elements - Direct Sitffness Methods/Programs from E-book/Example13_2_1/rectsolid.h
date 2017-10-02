/*********************************************
Example 13.2.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_RECTSOLID_H__
#define __RAJAN_RECTSOLID_H__

#include <string>
#include "xstype.h"

class CRectSolid: public CXSType
{
    static const int numRectDimensions = 2;
    public:
        CRectSolid (const std::string&, const CVector<float>& fV);
        CRectSolid (const CRectSolid&);
        ~CRectSolid ();
        // helper functions
        void DisplayDimensions () const;

    private:
        void ComputeProperties ();
};

#endif