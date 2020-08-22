/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN__CXSTYPE__H__
#define __RAJAN__CXSTYPE__H__

#include <string>
#include "../library/vectortemplate.h"

class CXSType
{
    public:
        CXSType ();
        CXSType (int);
        virtual ~CXSType ();

        // helper function
        void DisplayProperties ();

        // accessor functions
        void GetProperties (float&, float&, float&, float&, 
			float&, std::string&);
        void GetDimensions (CVector<float>&) const;
        virtual void ComputeProperties () = 0;

    private:
        void Initialize ();

    protected:
        std::string m_szID;            // identification tag
        float m_fArea;                 // x/s area
        float m_fIyy;                  // MOI y-axis
        float m_fIzz;                  // MOI z-axis
		float m_fS;                    // section modulus
		float m_fSF;                   // Shear factor (Izz*(t/Q)min
        int   m_numDimensions;         // number of dimensions
        CVector<float> m_fVDimensions; // the dimensions
};

#endif