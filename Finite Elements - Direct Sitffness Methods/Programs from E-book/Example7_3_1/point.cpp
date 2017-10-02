/* EXAMPLE 7.4.1
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

// modifier functions
void CPoint::SetValues (float fX, float fY)
{
    // coordinates set to fX and fY
    m_fXCoor = fX;
    m_fYCoor = fY;
}

void CPoint::SetValues (const CPoint& Point)
{
    // coordinates set to fX and fY
    m_fXCoor = Point.m_fXCoor;
    m_fYCoor = Point.m_fYCoor;
}

// accessor functions
void CPoint::GetValues (float& fX, float& fY) const
{
    // coordinates returned in fX and fY
    fX = m_fXCoor;
    fY = m_fYCoor;
}

void CPoint::GetValues (CPoint& Point) const
{
    // coordinates returned in fX and fY
    Point.SetValues (m_fXCoor, m_fYCoor);
}

// helper function
void CPoint::Display (const std::string& szBanner) const
{
    // display the current coordinates
    std::cout << szBanner << m_fXCoor << ","
              << m_fYCoor << ".";
}

