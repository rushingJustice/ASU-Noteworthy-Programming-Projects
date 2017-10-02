/*********************************************
Example 13.1.1
Copyright(c) 2005-12, S. D. Rajan
Object-Oriented Numerical Analysis

*********************************************/

#include <iostream>
#include "isection.h"

CISection::CISection ()
{
	m_szID = "Undefined";
	m_fArea = 0.0f;
	m_fSyy = 0.0f;
	m_fSzz = 0.0f;
	m_fWeight = 0.0f;
}

CISection::CISection (const CISection& IXS)
{
    SetProperties (IXS);
}

CISection::CISection (const std::string& szID, const float fArea,
					  const float fSyy, const float fSzz, 
					  const float fWeight)
{
	m_szID = szID;
	m_fArea = fArea;
	m_fSyy = fSyy;
	m_fSzz = fSzz;
	m_fWeight = fWeight;
}

CISection::~CISection ()
{
}

void CISection::Display () const
{
    std::cout << '\n'
		 << "ID                : " << m_szID    << '\n'
	     << "Area              : " << m_fArea   << " in^2" << '\n'
	     << "Section Modulus yy: " << m_fSyy    << " in^3" << '\n'
	     << "Section Modulus zz: " << m_fSzz    << " in^3" << '\n'
	     << "Weight            : " << m_fWeight << " lb/ft" << '\n';
}

void CISection::GetProperties (std::string& szID, float& fArea,
					           float& fSyy, float& fSzz,
                               float& fWeight) const
{
	szID = m_szID;
	fArea = m_fArea;
	fSyy = m_fSyy;
	fSzz = m_fSzz;
	fWeight = m_fWeight;
}

void CISection::SetProperties (const std::string& szID, 
                               const float fArea,
					           const float fSyy,
                               const float fSzz,
					           const float fWeight)
{
	m_szID = szID;
	m_fArea = fArea;
	m_fSyy = fSyy;
	m_fSzz = fSzz;
	m_fWeight = fWeight;
}

void CISection::SetProperties (const CISection& Source)
{
	m_szID = Source.m_szID;
	m_fArea = Source.m_fArea;
	m_fSyy = Source.m_fSyy;
	m_fSzz = Source.m_fSzz;
	m_fWeight = Source.m_fWeight;
}
