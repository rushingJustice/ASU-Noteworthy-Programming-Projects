/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Implementation of the CElement class.

*********************************************/
#include "element.h"

CElement::CElement ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nSN = m_nEN = m_nMPGroup = 0;
	m_nVN.SetSize (6);
	m_dVEP.SetSize (5);
	m_dVEP.Set(0.0);
    m_pEPGroup = NULL;
}

CElement::CElement (const CElement& EO)
// ---------------------------------------------------------------------------
// Function: copy constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nSN = EO.m_nSN;
    m_nEN = EO.m_nEN;
    m_nMPGroup = EO.m_nMPGroup;
    m_pEPGroup = EO.m_pEPGroup;
}

CElement::~CElement ()
// ---------------------------------------------------------------------------
// Function: dtor 
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CElement::GetENodes (CVector<int>& nVN) const
// ---------------------------------------------------------------------------
// Function: obtains the element nodes
// Input:    the element number
// Output:   an array of the nodes
// ---------------------------------------------------------------------------
{
    nVN = m_nVN;
}

void CElement::SetENodes (const CVector<int> nVN) 
// ---------------------------------------------------------------------------
// Function: sets the element nodes
// Input:    an array of the nodes
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nVN = nVN;
}

void CElement::SetEProperties (const CVector<double> dVEP)
// ---------------------------------------------------------------------------
// Function: sets tgets the element property values (E,v&CTE)
// Input:    array of properties
// Output:   none
// ---------------------------------------------------------------------------
{
    m_dVEP = dVEP;
}

void CElement::GetEProperties (CVector<double>& dVEP) const
// ---------------------------------------------------------------------------
// Function: gets the element property values (E,v&CTE)
// Input:    none
// Output:   returns the array of properties
// ---------------------------------------------------------------------------
{
    dVEP = m_dVEP;
}

void CElement::SetMatPropertyGroup (const int nMPG)
// ---------------------------------------------------------------------------
// Function: sets the material group group # associated with the element
// Input:    material group #
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nMPGroup = nMPG;
}

int CElement::GetMatPropertyGroup () const
// ---------------------------------------------------------------------------
// Function: gets the material group group # associated with the element
// Input:    none
// Output:   returns the material group #
// ---------------------------------------------------------------------------
{
    return m_nMPGroup;
}
