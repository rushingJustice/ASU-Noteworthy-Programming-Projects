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
    m_Type = ELType::CONCENTRATED_PY;
    m_fValue1 = m_fValue2 = 0.0f;
}

CElementLoads::CElementLoads (const CElementLoads& ELO)
// ---------------------------------------------------------------------------
// Function: copy constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nElement = ELO.m_nElement;
    m_Type = ELO.m_Type;
    m_fValue1 = ELO.m_fValue1;
    m_fValue2 = ELO.m_fValue2;
}

CElementLoads::~CElementLoads ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CElementLoads::GetValues (int& nE, ELType& Type,
                               float& f1, float& f2) const
// ---------------------------------------------------------------------------
// Function: gets the element load information
// Input:    element #, load type, 2 load associated values
// Output:   values for all these variables
// ---------------------------------------------------------------------------
{
    nE = m_nElement;
    Type = m_Type;
    f1 = m_fValue1;
    f2 = m_fValue2;
}

void CElementLoads::SetValues (const int nE, const ELType Type,
                               const float f1, const float f2)
// ---------------------------------------------------------------------------
// Function: sets the element load information
// Input:    element #, load type, 2 load associated values
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nElement = nE;
    m_Type = Type;
    m_fValue1 = f1;
    m_fValue2 = f2;
}

void CElementLoads::GetENF (CVector<float>& fV1, CVector<float>& fV2,
                            const float fLength) const
// ---------------------------------------------------------------------------
// Function: gets the equivalent nodal forces
// Input:    vectors to store ENF at start node and end node, element length
// Output:   the two vectors suitably populated
// ---------------------------------------------------------------------------
{   
	float fa,fb,fload;

	if (m_Type != ELType::DISTRIBUTED)
	{
		fa = m_fValue1;
		fb = fLength - fa;
		fload = m_fValue2;
	}

	if (m_Type == ELType::DISTRIBUTED)
	{
		// start node
		fV1(1) = 0.0;
		fV1(2) = (fLength*(7.0*m_fValue1+3.0*m_fValue2)/20.0);
		fV1(3) = (fLength*fLength*(3.0*m_fValue1+2.0*m_fValue2)/60.0);
		// end node
		fV2(1) = 0.0;
		fV2(2) = (fLength*(7.0*m_fValue2+3.0*m_fValue1)/20.0);
		fV2(3) = -(fLength*fLength*(3.0*m_fValue2+2.0*m_fValue1)/60.0);
	}
	else if (m_Type == ELType::CONCENTRATED_PY)
	{
		// start node
		fV1(1) = 0.0;
		fV1(2) = (fload*fb*fb*(fLength+2.0*fa)/(fLength*fLength*fLength));
		fV1(3) = (fload*fa*fb*fb/(fLength*fLength));
		// end node
		fV2(1) = 0.0;
		fV2(2) = (fload*fa*fa*(fLength+2.0*fb)/(fLength*fLength*fLength));
		fV2(3) = -(fload*fa*fa*fb/(fLength*fLength));
	}
	else if (m_Type == ELType::CONCENTRATED_PX)
	{
		// start node
		fV1(1) = (fload*fb/fLength);
		fV1(2) = 0.0;
		fV1(3) = 0.0;
		// end node
		fV2(1) = (fload*fa/fLength);
		fV2(2) = 0.0;
		fV2(3) = 0.0;
	}
	else // concentrated moment
	{
		// start node
		fV1(1) = 0.0;
		fV1(2) = -(6.0*fload*fa*fb/(fLength*fLength*fLength));
		fV1(3) = -(fload*fb*(2.0*fa-fb)/(fLength*fLength));
		// end node
		fV2(1) = 0.0;
		fV2(2) = (6.0*fload*fa*fb/(fLength*fLength*fLength));
		fV2(3) = (fload*fa*(fa-2.0*fb)/(fLength*fLength));
	}
}

