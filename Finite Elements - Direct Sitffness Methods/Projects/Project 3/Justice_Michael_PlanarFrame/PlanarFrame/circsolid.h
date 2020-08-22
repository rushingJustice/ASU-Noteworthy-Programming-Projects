/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_CIRCSOLID_H__
#define __RAJAN_CIRCSOLID_H__

#include "..\library\vectortemplate.h"
#include "xstype.h"
const int numCircDimensions = 1;

class CCircSolid: public CXSType
{
    public:
        CCircSolid (const CVector<float>& fV);
        CCircSolid (const CCircSolid&);
        ~CCircSolid ();

        // helper functions
        virtual void ComputeProperties ();

    private:
};

#endif