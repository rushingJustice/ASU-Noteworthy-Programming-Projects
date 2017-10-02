/*********************************************
Example 13.3.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_ISECTION_H__
#define __RAJAN_ISECTION_H__

#include "xstype.h"
#include <string>

class CISection: public CXSType
{
    static const int numISDimensions = 4;
    public:
        CISection (const std::string&, const CVector<float>& fV);
        CISection (const CISection&);
        ~CISection ();

        // helper functions
        virtual void DisplayDimensions () const;
        virtual void ComputeProperties ();

    private:
};

#endif