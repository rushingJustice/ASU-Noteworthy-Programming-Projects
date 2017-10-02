#include <iostream>
#include <cmath>
#include "point.h"

CPoint::CPoint () // default ctor
{
	m_fXCoor = m_fYCoor = m_fZCoor = 0.0f;
}

CPoint::CPoint (float fX, float fY, float fZ)   
{
	m_fXCoor = fX;
	m_fYCoor = fY;
	m_fZCoor = fZ;
}

CPoint::CPoint (float fX, float fY)   
{
	m_fXCoor = fX;
	m_fYCoor = fY;
	m_fZCoor = 0.0f;
}

CPoint::CPoint (const CPoint& A)    
{
	m_fXCoor = A.m_fXCoor;
    m_fYCoor = A.m_fYCoor;
    m_fZCoor = A.m_fZCoor;
}

CPoint::~CPoint() // dtor
{
}

void CPoint::Display (const std::string& szMessage) const
{
	std::cout << szMessage << "\n"
              << "(X, Y, Z) = (" << m_fXCoor << ", "
	                             << m_fYCoor << ", "
	                             << m_fZCoor << ")\n";
}

void CPoint::SetCoordinates (const float fX, const float fY, const float fZ)
{
	m_fXCoor = fX;
	m_fYCoor = fY;
	m_fZCoor = fZ;
}

void CPoint::SetCoordinates (const float fX, const float fY)
{
	m_fXCoor = fX;
	m_fYCoor = fY;
	m_fZCoor = 0.0f;
}

void CPoint::GetCoordinates (float& fX, float& fY, float& fZ) const
{
	fX = m_fXCoor;
	fY = m_fYCoor;
	fZ = m_fYCoor;
}

void CPoint::GetCoordinates (float& fX, float& fY) const
{
	fX = m_fXCoor;
	fY = m_fYCoor;
}

float CPoint::DistanceTo (const CPoint& P) const
{
	float fXDiff = P.m_fXCoor - m_fXCoor;
	float fYDiff = P.m_fYCoor - m_fYCoor;
	float fZDiff = P.m_fZCoor - m_fZCoor;
	return (sqrt(fXDiff*fXDiff + fYDiff*fYDiff + fZDiff*fZDiff));
}

void CPoint::UnitVector (const CPoint& P, float fVU[3]) const
{
	float fDist = DistanceTo (P);
	fVU[0] = (P.m_fXCoor - m_fXCoor)/fDist;
	fVU[1] = (P.m_fYCoor - m_fYCoor)/fDist;
	fVU[2] = (P.m_fZCoor - m_fZCoor)/fDist;
}

bool CPoint::IsOrigin () const
{
	if (m_fXCoor != 0.0f || m_fYCoor != 0.0f || m_fZCoor != 0.0f)
	     return false;
	else 
		return true;
}

