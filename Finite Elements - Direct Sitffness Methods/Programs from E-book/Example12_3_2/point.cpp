/* EXAMPLE 12.3.2
   Copyright(c) 2002-05, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#include <iostream>
#include <string>
#include <cassert>
#include <stdio.h>
#include "point.h"

void mystrcpy (char *szTarget, const char *szSource)
{
    int i=0;
    for (;;)
    {
        if (szSource[i] == '\0')
            break;
        else
            szTarget[i] = szSource[i];
        ++i;
    }
    szTarget[i] = '\0';
}

// default constructor
CPoint::CPoint ()
{
    // default tag value
    mystrcpy (m_szTag, "blank");
    // coordinates initialized to zero
    m_fXCoor = 0.0f;
    m_fYCoor = 0.0f;
}

// copy constructor
CPoint::CPoint (const CPoint& P)
{
    SetValues (P);
}

// constructor
CPoint::CPoint (const std::string& szTag, float fX, float fY)
{
    SetTag (szTag);
    SetCoords (fX, fY);
}

// modifier functions
void CPoint::SetCoords (float fX, float fY)
{
    // coordinates set to fX and fY
    m_fXCoor = fX;
    m_fYCoor = fY;
}

void CPoint::SetTag (const std::string& szTag)
{
    assert (szTag.length() > 0 && szTag.length() <= MAXIDTAG);
    mystrcpy (m_szTag, szTag.c_str());
}

void CPoint::SetValues (const CPoint& Point)
{
    mystrcpy (m_szTag, Point.m_szTag);
    m_fXCoor = Point.m_fXCoor;
    m_fYCoor = Point.m_fYCoor;
}

// accessor functions
void CPoint::GetCoords (float& fX, float& fY) const
{
    fX = m_fXCoor;
    fY = m_fYCoor;
}

void CPoint::GetTag (std::string& szTag) const
{
    szTag = m_szTag;
}

void CPoint::GetValues (CPoint& Point) const
{
    mystrcpy (Point.m_szTag, m_szTag);
    Point.m_fXCoor = m_fXCoor;
    Point.m_fYCoor = m_fYCoor;
}

// helper function
void CPoint::Display (const std::string& szBanner) const
{
    // display the current coordinates
    std::cout << szBanner << m_szTag << ", "
              << m_fXCoor << ", "
              << m_fYCoor << ".\n";
}

// overloaded member operators
CPoint& CPoint::operator= (const CPoint& PRight)
{
    // copying itself?
    if (&PRight != this)
    {
        mystrcpy (m_szTag, PRight.m_szTag);
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
std::ostream &operator<< (std::ostream& ofs,
                          const CPoint& Point)
{
    // display the current coordinates
    ofs << Point.m_szTag << "," << Point.m_fXCoor
        << "," << Point.m_fYCoor << ".\n";

    return ofs;
}

std::istream &operator>> (std::istream& ifs, 
                          CPoint& Point)
{
    std::string szTemp;
    float fXCoor, fYCoor;

    do
    {
        std::cout << "Tag: ";
        ifs >> szTemp;
    } while (szTemp.length() <= 0 || szTemp.length() > MAXIDTAG);
    std::cout << "X Coordinate: ";
    ifs >> fXCoor;
    std::cout << "Y Coordinate: ";
    ifs >> fYCoor;
    Point.SetCoords (fXCoor, fYCoor);
    Point.SetTag (szTemp);

    return ifs;
}

