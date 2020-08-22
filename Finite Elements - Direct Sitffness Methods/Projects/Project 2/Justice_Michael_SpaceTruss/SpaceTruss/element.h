/*********************************************
Planar Truss Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CElement class definition.

*********************************************/
#ifndef __RAJAN_ELEMENT_H__
#define __RAJAN_ELEMENT_H__

class CElement
{
    public:
        // ctor and dtor
        CElement ();
        ~CElement ();

        // accessor function
        void GetData (int& nSN, int& nEN, float& fArea, 
			float& fE, float& fCTE) const;

        // modifier functions
        void SetData (const int nSN, const int nEN, const float fArea,
                      const float fE, const float fCTE);

    private:
        int m_nSN;		// start node
        int m_nEN;		// end node
        float m_fArea;	// x/s area
        float m_fE;		// modulus of elasticity
		float m_fCTE;   // coefficient of thermal expansion
};

#endif