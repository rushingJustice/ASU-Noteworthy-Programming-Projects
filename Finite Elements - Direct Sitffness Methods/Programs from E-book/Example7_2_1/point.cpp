/* EXAMPLE 7.2.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#include <iostream>
#include <string>
#include "point.h"

// default constructor
CPoint::CPoint ()
{
    // coordinates initialized to zero
    m_fXCoor = 0.0f;
    m_fYCoor = 0.0f;
}

// constructor
CPoint::CPoint (float fX, float fY)
{
    // coordinates set to fX and fY
    m_fXCoor = fX;
    m_fYCoor = fY;
}

// modifier function
void CPoint::SetValues (float fX, float fY)
{
    // coordinates set to fX and fY
    m_fXCoor = fX;
    m_fYCoor = fY;
}

// accessor function
void CPoint::GetValues (float& fX, float& fY)
{
    // coordinates returned in fX and fY
    fX = m_fXCoor;
    fY = m_fYCoor;
}

// helper function
void CPoint::Display (const std::string szBanner)
{
    // display the current coordinates
    std::cout << szBanner
              << "[X,Y] Coordinates = [" 
              << m_fXCoor << ","
              << m_fYCoor << "].\n";
}

