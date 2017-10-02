/*********************************************
Example 13.3.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <cmath>
#include <iostream>
#include "rectsolid.h"

CRectSolid::CRectSolid (const std::string& szID,
                const CVector<float>& fV) : CXSType (numRectDimensions)
{
    assert (fV.GetSize() >= numRectDimensions);
    m_szID = szID;
    for (int i=1; i <= numRectDimensions; i++)
        m_fVDimensions(i) = fV(i);
    ComputeProperties ();
}

CRectSolid::CRectSolid (const CRectSolid& IS) : CXSType (numRectDimensions)
{
    m_szID = IS.m_szID;
    for (int i=1; i <= numRectDimensions; i++)
        m_fVDimensions(i) = IS.m_fVDimensions(i);
    ComputeProperties ();
}

CRectSolid::~CRectSolid ()
{
}

void CRectSolid::DisplayDimensions () const
{
    std::cout << '\n'
         << "  Rectangular Solid Dimensions\n"
         << "      Section ID : " << m_szID << '\n'
         << "          Height : " << m_fVDimensions(1) << " in\n"
         << "           Width : " << m_fVDimensions(2) << " in\n";
}

void CRectSolid::ComputeProperties ()
{
    // height
    float fH = m_fVDimensions(1); 
    // width
    float fW = m_fVDimensions(2); 

    // cross-sectional area
    m_fArea = fH * fW;
    // section modulus y-axis
    m_fSyy = (pow(fH, 3.0f)*fW/12.0f)/(0.5f*fH);
    // section modulus z-axis
    m_fSzz = (pow(fW, 3.0f)*fH/12.0f)/(0.5f*fW);
}