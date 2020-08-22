/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_ISECTION_H__
#define __RAJAN_ISECTION_H__

#include "..\library\vectortemplate.h"
#include "xstype.h"
const int numISDimensions = 4;

class CISection: public CXSType
{
    public:
        CISection (const CVector<float>& fV);
        ~CISection ();

        // helper functions
        virtual void ComputeProperties ();

    private:
};

#endif