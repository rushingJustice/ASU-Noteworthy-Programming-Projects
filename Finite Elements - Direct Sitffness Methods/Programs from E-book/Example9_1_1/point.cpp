/* EXAMPLE 9.1.1
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

// copy constructor
CPoint::CPoint (const CPoint& P)
{
    m_fXCoor = P.m_fXCoor;
    m_fYCoor = P.m_fYCoor;
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
              << m_fYCoor << ".\n";
}

// overloaded member operators
CPoint& CPoint::operator= (const CPoint& PRight)
{
    // copying itself?
    if (&PRight != this)
    {
        // copy the coordinate values
        m_fXCoor = PRight.m_fXCoor;
        m_fYCoor = PRight.m_fYCoor;
    }

    return (*this);
}

CPoint CPoint::operator+ (const CPoint& PRight) const
{
    CPoint PResult;

    // add the coordinates
    PResult.m_fXCoor = m_fXCoor + PRight.m_fXCoor;
    PResult.m_fYCoor = m_fYCoor + PRight.m_fYCoor;

    return PResult;
}

CPoint CPoint::operator- (const CPoint& PRight) const
{
    CPoint PResult;

    // subtract the coordinates
    PResult.m_fXCoor = m_fXCoor - PRight.m_fXCoor;
    PResult.m_fYCoor = m_fYCoor - PRight.m_fYCoor;

    return PResult;
}

bool CPoint::operator== (const CPoint& PRight) const
{
    // check whether both coordinates are equal
    return (m_fXCoor == PRight.m_fXCoor &&
            m_fYCoor == PRight.m_fYCoor);
}

bool CPoint::operator!= (const CPoint& PRight) const
{
    // check whether one of the coordinates are unequal
    return (m_fXCoor != PRight.m_fXCoor ||
            m_fYCoor != PRight.m_fYCoor);
}

// overloaded operator friend functions
ostream &operator<< (ostream& ofs, const CPoint& Point)
{
    // display the current coordinates
    ofs << Point.m_fXCoor << "," << Point.m_fYCoor << ".\n";

    return ofs;
}

istream &operator>> (istream& ifs, CPoint& Point)
{
    // get the coordinate values
    std::cout << "X Coordinate: ";
    ifs >> Point.m_fXCoor;
    std::cout << "Y Coordinate: ";
    ifs >> Point.m_fYCoor;

    return ifs;
}

