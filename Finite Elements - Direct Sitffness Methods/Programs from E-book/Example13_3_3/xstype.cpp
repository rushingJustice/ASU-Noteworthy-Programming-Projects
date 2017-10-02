/*********************************************
Example 13.3.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include <cassert>
#include "xstype.h"

CXSType::CXSType ()
{
    Initialize ();
}

CXSType::CXSType (int numDimensions)
{
	Initialize ();
    m_numDimensions = numDimensions;
    m_fVDimensions.SetSize (m_numDimensions);
}

void CXSType::Initialize ()
{
    m_numDimensions = 0;
    m_szID = "Undefined";
    m_fArea = 0.0f;
    m_fSyy = 0.0f;
    m_fSzz = 0.0f;
}

CXSType::~CXSType ()
{
}

void CXSType::DisplayProperties ()
{
    ComputeProperties ();
    std::cout << '\n'
         << "ID                : " << m_szID    << '\n'
         << "Area              : " << m_fArea   << " in^2\n"
         << "Section Modulus yy: " << m_fSyy    << " in^3\n"
         << "Section Modulus zz: " << m_fSzz    << " in^3\n";
}

void CXSType::GetProperties (std::string& szID, float& fArea,
                     float& fSyy, float& fSzz)
{
    ComputeProperties ();
    szID = m_szID;
    fArea = m_fArea;
    fSyy = m_fSyy;
    fSzz = m_fSzz;
}

void CXSType::GetDimensions (std::string& szID, CVector<float>& fV)
{
    assert (fV.GetSize() >= m_numDimensions);
    szID = m_szID;
    for (int i=1; i <= m_numDimensions; i++)
        fV(i) = m_fVDimensions(i);
}
