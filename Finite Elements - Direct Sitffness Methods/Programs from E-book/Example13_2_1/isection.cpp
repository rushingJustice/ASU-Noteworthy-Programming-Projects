/*********************************************
Example 13.2.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <cmath>
#include <iostream>
#include "isection.h"

CISection::CISection (const std::string& szID,
              const CVector<float>& fV) : CXSType (numISDimensions)
{
    assert (fV.GetSize() >= numISDimensions);
    m_szID = szID;
    for (int i=1; i <= numISDimensions; i++)
        m_fVDimensions(i) = fV(i);
    ComputeProperties ();
}

CISection::CISection (const CISection& IS) : CXSType (numISDimensions)
{
    m_szID = IS.m_szID;
    for (int i=1; i <= numISDimensions; i++)
        m_fVDimensions(i) = IS.m_fVDimensions(i);
    ComputeProperties ();
}

CISection::~CISection ()
{
}

void CISection::DisplayDimensions () const
{
    std::cout << '\n'
         << "    I-Section Dimensions" << '\n'
         << "      Section ID : " << m_szID << '\n' 
         << "      Web height : " << m_fVDimensions(1) << " in\n"
         << "   Web thickness : " << m_fVDimensions(2) << " in\n"
         << "    Flange width : " << m_fVDimensions(3) << " in\n"
         << "Flange thickness : " << m_fVDimensions(4) << " in\n";
}

void CISection::ComputeProperties ()
{
    // web height
    float wH = m_fVDimensions(1); 
    // web thickness
    float wT = m_fVDimensions(2); 
    // flange width
    float fW = m_fVDimensions(3); 
    // flange thickness
    float fT = m_fVDimensions(4); 

    // cross-sectional area
    m_fArea = wH*wT + 2.0f*fW*fT;
    // section modulus y-axis
    float fOI, fII;
    fOI = (pow(wH+2.0f*fT, 3.0f) * fW)/12.0f;
    fII = 2.0f*(pow(wH, 3.0f) * (0.5f*(fW-wT)))/12.0f;
    m_fSyy = (fOI - fII)/(0.5f*wH+fT);
    // section modulus z-axis
    float fIzz = 2.0f*(pow(fW,3.0f)*fT)/12.0f + 
                 pow(wT,3)*wH/12.0f;
    m_fSzz = fIzz/(0.5f*fW);
}