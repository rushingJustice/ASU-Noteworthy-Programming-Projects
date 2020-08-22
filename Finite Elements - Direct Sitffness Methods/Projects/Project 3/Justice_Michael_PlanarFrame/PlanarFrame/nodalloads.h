/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_NODALLOADS_H__
#define __RAJAN_NODALLOADS_H__

#include "..\library\vectortemplate.h"
#include "constants.h"

class CNodalLoads
{
    public:
        CNodalLoads ();                   // default ctor 
        CNodalLoads (const CNodalLoads&); //copy ctor
        ~CNodalLoads ();

        // accessor functions
        void GetValues (CVector<float>&);

        // modifier functions
        void SetValues (const CVector<float>&);

    private:
        CVector<float> m_fVLoads;	// nodal loads
};

#endif