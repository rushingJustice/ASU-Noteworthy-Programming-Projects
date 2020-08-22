/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_ELEMENTLOADS_H__
#define __RAJAN_ELEMENTLOADS_H__

#include "..\library\vectortemplate.h"
#include "constants.h"

class CElementLoads
{
    public:
        enum class ELType {CONCENTRATED_PX, CONCENTRATED_PY,
			CONCENTRATED_M, DISTRIBUTED};

        CElementLoads ();                         // default ctor
        CElementLoads (const CElementLoads& ELO); // copy ctor
        ~CElementLoads ();                        // dtor

        // accessor functions
		void GetValues (int& nE, int& nS, 
                        float& fLoad) const;
        void GetENF (int& nE, int& nSide, float& fload) const;

        // modifier functions
        void SetValues (const int nE, const int nS,
                        const float fLoad);
    private:
        int    m_nElement;	// element number
        int    m_Side;	    // side of T6 element
        float  m_fValue;	// load intensity
                            
};		

#endif