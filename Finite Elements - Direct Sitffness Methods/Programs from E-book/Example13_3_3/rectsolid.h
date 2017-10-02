/*********************************************
Example 13.3.3
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
        CRectSolid (const std::string&, const CVector<float>& fV,
                    const float fMC);
        CRectSolid (const CRectSolid&);
        ~CRectSolid ();
        // helper functions
        virtual void DisplayDimensions ();
        virtual void ComputeProperties ();
        float GetMC ();
        void SetMC (const float);

    private:
        float m_fMC; // moisture content
};

#endif