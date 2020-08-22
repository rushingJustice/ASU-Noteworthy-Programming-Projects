/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Implementation of the CNodalLoads class.

*********************************************/
#include "NodalLoads.h"

CNodalLoads::CNodalLoads ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_fVLoads.SetSize (3); m_fVLoads.Set(0.0f);
}

CNodalLoads::CNodalLoads (const CNodalLoads& NLO)
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_fVLoads.SetSize (3); 
    m_fVLoads = NLO.m_fVLoads;
}

CNodalLoads::~CNodalLoads ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CNodalLoads::GetValues (CVector<float>& fV)
// ---------------------------------------------------------------------------
// Function: gets the nodal loads at the current node
// Input:    vector to hold the nodal load values
// Output:   nodal load values
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= DOFPN; i++)
        fV(i) = m_fVLoads(i);
}

void CNodalLoads::SetValues (const CVector<float>& fV)
// ---------------------------------------------------------------------------
// Function: sets the nodal loads at the current node
// Input:    vector with the nodal load values
// Output:   none
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= 3; i++)
        m_fVLoads(i) = fV(i);
}

