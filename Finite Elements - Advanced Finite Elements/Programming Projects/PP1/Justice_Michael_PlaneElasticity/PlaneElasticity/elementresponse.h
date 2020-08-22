/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_ELEMENTRESPONSE_H__
#define __RAJAN_ELEMENTRESPONSE_H__

#include "..\library\vectortemplate.h"
#include "constants.h"

class CElementResponse
{
    public:
        CElementResponse ();    // default ctor
        ~CElementResponse ();   // dtor

        // accessor functions
        void GetValues (CVector<float>& fVFStartNode,
                        CVector<float>& fVFEndNode) const;
		void GetStress (CVector<float>& fVSE) const;

        // modifier functions
        void SetValues (const CVector<float>& fVFStartNode,
                        const CVector<float>& fVFEndNode);
		void SetStress (const CVector<float>& fVSE);

    private:
        CVector<float> m_fVFStartNode;
                // nodal forces at start node
        CVector<float> m_fVFEndNode;
                // nodal forces at end node
		CVector<float> m_fVSE;
                // stresses at element
};

#endif