/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_ELEMENT_H__
#define __RAJAN_ELEMENT_H__

#include "xstype.h"

class CElement
{
    public:
		enum class EType {PLANESTRESS, PLANESTRAIN};
        CElement ();                  // default ctor
        CElement(const CElement& EO); // copy ctor
        ~CElement ();                 // dtor

        // accessor functions
        void GetENodes (CVector<int>& nVN) const;
        int  GetMatPropertyGroup () const;
        void GetEProperties (CVector<double>& dVEP) const;
        
        // modifier functions
		void SetENodes (const CVector<int> nVN);
        void SetEProperties (const CVector<double> dVEP);
        void SetMatPropertyGroup (const int);

    private:
        int m_nSN;		        // start node
        int m_nEN;		        // end node
		CVector<double> m_dVEP; // Vector containing element props
		CVector<int> m_nVN;     // Vector containing nodes for element
        int m_nMPGroup;         // material property group
        CXSType* m_pEPGroup;    // element property group
};

#endif