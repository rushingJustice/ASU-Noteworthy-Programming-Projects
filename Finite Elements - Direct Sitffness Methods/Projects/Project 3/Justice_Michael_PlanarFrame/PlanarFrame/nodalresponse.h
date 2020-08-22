/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_NODALRESPONSE_H__
#define __RAJAN_NODALRESPONSE_H__

#include "..\library\vectortemplate.h"
#include "constants.h"

class CNodalResponse
{
    public:
        CNodalResponse ();   // ctor
        ~CNodalResponse ();  // dtor

        // accessor functions
        void GetValues (CVector<float>& fVDisp) const;

        // modifier functions
        void SetValues (const CVector<float>& fVDisp);

    private:
        CVector<float> m_fVDisp; // nodal displacements	
};

#endif