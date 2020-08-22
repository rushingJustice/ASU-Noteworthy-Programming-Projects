/*********************************************
Planar Truss Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CNodalResponse class implementation.

*********************************************/
#include "NodalResponse.h"

CNodalResponse::CNodalResponse ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_fXDisp = m_fYDisp = m_fZDisp = 0.0f;
}

CNodalResponse::~CNodalResponse ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CNodalResponse::GetDisplacements (float& fXD, float& fYD, float& fZD) const
// ---------------------------------------------------------------------------
// Function: gets x and y nodal displacements
// Input:    variables to hold x-displacement, y-displacement values
// Output:   x-displacement, y-displacement values
// ---------------------------------------------------------------------------
{
    fXD = m_fXDisp;
    fYD = m_fYDisp;
	fZD = m_fZDisp;
}

void CNodalResponse::SetDisplacements (const float fXD, const float fYD, const float fZD)
// ---------------------------------------------------------------------------
// Function: sets x and y nodal displacements
// Input:    variables holding x-displacement, y-displacement values
// Output:   modified x-displacement, y-displacement values
// ---------------------------------------------------------------------------
{
    m_fXDisp = fXD;
    m_fYDisp = fYD;
	m_fZDisp = fZD;
}

