/*********************************************
Example 7.6.2
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "point.h"

#include <iostream>
using std::cout;
using std::endl;

// default constructor
CPoint::CPoint (float fX, float fY)
{
	m_fXCoor = fX;
	m_fYCoor = fY;
}

// stub function
CPoint::CPoint (float fX, float fY, float fZ)
{
}

// destructor
CPoint::~CPoint ()
{
}

// helper functions
void CPoint::Print ()
{
	cout << endl;
	cout << "(X, Y) Coordinates: (" << m_fXCoor << "," 
		<< m_fYCoor << ")" << endl;
}

// accessor functions
void CPoint::GetValues (float& fX, float& fY)
{
	fX = m_fXCoor;
	fY = m_fYCoor;
}

// stub function
void CPoint::GetValues (float& fX, float& fY, float& fZ)
{
}

// accessor functions
void CPoint::SetValues (float fX, float fY)
{
	// set the values
	m_fXCoor = fX;
	m_fYCoor = fY;
}

// stub function
void CPoint::SetValues (float fX, float fY, float fZ)
{
}

