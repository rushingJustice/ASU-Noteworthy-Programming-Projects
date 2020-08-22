/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Implementation of the CElementLoads class.

*********************************************/
#include "ElementLoads.h"

CElementLoads::CElementLoads ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nElement = 0;
    m_Side = 0;
    m_fValue = 0.0f;
}

CElementLoads::CElementLoads (const CElementLoads& ELO)
// ---------------------------------------------------------------------------
// Function: copy constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nElement = ELO.m_nElement;
    m_Side = ELO.m_Side;
    m_fValue = ELO.m_fValue;
}

CElementLoads::~CElementLoads ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CElementLoads::GetValues (int& nE, int& nS,
                               float& f1) const
// ---------------------------------------------------------------------------
// Function: gets the element load information
// Input:    element #, side number, normal dist. load on sides
// Output:   values for all these variables
// ---------------------------------------------------------------------------
{
    nE = m_nElement;
    nS = m_Side;
    f1 = m_fValue;

}

void CElementLoads::SetValues (const int nE, const int nS ,
                               const float f1)
// ---------------------------------------------------------------------------
// Function: sets the element load information
// Input:    element #, side number, normal dist. load on side
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nElement = nE;
    m_Side = nS;
    m_fValue = f1;
}

void CElementLoads::GetENF (int& nE, int& nSide, float& fload) const
// ---------------------------------------------------------------------------
// Function: gets the equivalent nodal forces
// Input:    element number, element side, load value
// Output:   load vector of equivalent nodal loads
// ---------------------------------------------------------------------------
{   

	
}

