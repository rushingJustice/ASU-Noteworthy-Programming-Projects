/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Derived class from CXSType base class
to store circular solid data and properties
*********************************************/
#include <cmath>
#include <iostream>
#include "circsolid.h"

CCircSolid::CCircSolid (const CVector<float>& fV) : CXSType (numCircDimensions)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    vector with I-section dimensions
// Output:   none
// ---------------------------------------------------------------------------
{
    assert (fV.GetSize() >= numCircDimensions);
    for (int i=1; i <= numCircDimensions; i++)
        m_fVDimensions(i) = fV(i);
	m_szID = "Circular Solid";
    ComputeProperties ();
}

CCircSolid::~CCircSolid ()
// ---------------------------------------------------------------------------
// Function: dtor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CCircSolid::ComputeProperties ()
// ---------------------------------------------------------------------------
// Function: computes the I-section properties
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // radius and pi
    float fr = m_fVDimensions(1); 
	float pi = 3.141592;

    // cross-sectional area
    m_fArea = pi*fr*fr;
    // MOI y-axis
    m_fIyy = pi*pow((2*fr),4.0)/64.0;
    // MOI z-axis
    m_fIzz = m_fIyy;
	// section modulus
	m_fS = m_fIzz/fr;
	// shear factor
	m_fSF = m_fIzz*(2.0*fr/(2.0*pow(fr,3.0)/3.0));
}