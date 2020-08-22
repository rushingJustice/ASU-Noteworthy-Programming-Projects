/*********************************************
Planar Truss Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CNode class implementation.

*********************************************/
#include "Node.h"

CNode::CNode ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_fXCoor = m_fYCoor = m_fZCoor = m_fXForce = m_fYForce = m_fZForce = 0.0f;
	m_fidx = m_fidy = m_fidz = m_fDTemp = 0.0f;
    m_nXFC = m_nYFC = m_nZFC = 0;
}

CNode::~CNode ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CNode::GetCoords (float& fX, float& fY, float& fZ) const
// ---------------------------------------------------------------------------
// Function: gets nodal coordinates
// Input:    variables to hold x-coordinate, y-coordinate values
// Output:   x-coordinate, y-coordinate values
// ---------------------------------------------------------------------------
{
    fX = m_fXCoor;
    fY = m_fYCoor;
	fZ = m_fZCoor;
}

void CNode::GetFixity (int& nXFC, int& nYFC, int& nZFC) const
// ---------------------------------------------------------------------------
// Function: gets nodal fixity conditions
// Input:    variables to hold x-fixity, y-fixity values
// Output:   x-fixity, y-fixity values
// ---------------------------------------------------------------------------
{
    nXFC = m_nXFC;
    nYFC = m_nYFC;
	nZFC = m_nZFC;
}

void CNode::GetLoads (float& fXF, float& fYF, float& fZF, float& fDTemp) const
// ---------------------------------------------------------------------------
// Function: gets nodal loads
// Input:    variables to hold x-force, y-force, temp change values
// Output:   x-force, y-force, temp change values
// ---------------------------------------------------------------------------
{
    fXF = m_fXForce;
    fYF = m_fYForce;
	fZF = m_fZForce;
	fDTemp = m_fDTemp;
}

void CNode::GetIDisp (float& fIXDisp, float& fIYDisp, float& fIZDisp) const
// ---------------------------------------------------------------------------
// Function: gets initial displacements 
// Input:    variables to hold initial x-displacement, y-displacement values
// Output:   initial x-displacement, y-displacement values
// ---------------------------------------------------------------------------
{
    fIXDisp = m_fidx;
    fIYDisp = m_fidy;
	fIZDisp = m_fidz;
}

void CNode::SetCoords (const float fX, const float fY, const float fZ)
// ---------------------------------------------------------------------------
// Function: sets nodal coordinates
// Input:    variables holding x-coordinate, y-coordinate values
// Output:   modified x-coordinate, y-coordinate values
// ---------------------------------------------------------------------------
{
    m_fXCoor = fX;
    m_fYCoor = fY;
	m_fZCoor = fZ;
}

void CNode::SetFixity (const int nXFC, const int nYFC, const int nZFC)
// ---------------------------------------------------------------------------
// Function: sets nodal fixities
// Input:    variables x-fixity, y-fixity values
// Output:   modified x-fixity, y-fixity values
// ---------------------------------------------------------------------------
{
    m_nXFC = nXFC;
    m_nYFC = nYFC;
	m_nZFC = nZFC;
}

void CNode::SetLoads (const float fXF, const float fYF, const float fZF, const float fDTemp)
// ---------------------------------------------------------------------------
// Function: sets nodal loads
// Input:    variables holding x-force, y-force, temp change values
// Output:   modified x-force, y-force, temp change values
// ---------------------------------------------------------------------------
{
    m_fXForce = fXF;
    m_fYForce = fYF;
	m_fZForce = fZF;
	m_fDTemp = fDTemp;
}

void CNode::SetIDisp (const float fIXDisp, const float fIYDisp, const float fIZDisp)
// ---------------------------------------------------------------------------
// Function: sets nodal initial displacements 
// Input:    variables holding initial x-displacements, y-displacements values
// Output:   modified initial x-displacement, y-displacement values
// ---------------------------------------------------------------------------
{
    m_fidx = fIXDisp;
    m_fidy = fIYDisp;
	m_fidz = fIZDisp;
}