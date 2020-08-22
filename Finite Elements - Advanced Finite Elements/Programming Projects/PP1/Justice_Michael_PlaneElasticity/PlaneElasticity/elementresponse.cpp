/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Implementation of the CElementResponse class.

*********************************************/
#include "ElementResponse.h"

CElementResponse::CElementResponse ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_fVFStartNode.SetSize (DOFPN);  m_fVFStartNode.Set(0.0f);
    m_fVFEndNode.SetSize (DOFPN);    m_fVFEndNode.Set(0.0f);

	m_fVSE.SetSize (NSTRESS);  m_fVSE.Set(0.0f);
}

CElementResponse::~CElementResponse ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CElementResponse::GetValues (CVector<float>& fVFSN,
                                  CVector<float>& fVFEN) const
// ---------------------------------------------------------------------------
// Function: gets the element force response
// Input:    vectors to hold element forces at start and end nodes
// Output:   the element force values
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= DOFPN; i++)
    {
        fVFSN(i) = m_fVFStartNode(i);
        fVFEN(i) = m_fVFEndNode(i);
    }
}

void CElementResponse::SetValues (const CVector<float>& fVFSN,
                                  const CVector<float>& fVFEN)
// ---------------------------------------------------------------------------
// Function: gets the element force response
// Input:    vectors to hold element forces at start and end nodes
// Output:   none
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= DOFPN; i++)
    {
        m_fVFStartNode(i) = fVFSN(i);
        m_fVFEndNode (i) = fVFEN(i);
    }
}

void CElementResponse::GetStress (CVector<float>& fVSE) const
// ---------------------------------------------------------------------------
// Function: gets the element stress response
// Input:    vectors to hold max element stresses at element
// Output:   the element stress values
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= NSTRESS; i++)
    {
        fVSE(i) = m_fVSE(i);
    }
}

void CElementResponse::SetStress (const CVector<float>& fVSE)
// ---------------------------------------------------------------------------
// Function: sets the element stress response
// Input:    vectors to hold max element stresses at element
// Output:   none
// ---------------------------------------------------------------------------
{
    for (int i=1; i <= NSTRESS; i++)
    {
        m_fVSE(i) = fVSE(i);
    }
}