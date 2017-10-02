/*********************************************
Example 7.6.1
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "line.h"

#include <iostream>
using std::cout;
using std::endl;

// default constructor
CLine::CLine (int nSP, int nEP)
{
	m_nStartPoint = nSP;
	m_nEndPoint = nEP;
}

// destructor
CLine::~CLine ()
{
}

// helper functions
void CLine::Print ()
{
	cout << endl;
	cout << "(Start, End) Points: (" << m_nStartPoint << "," 
		<< m_nEndPoint << ")" << endl;
}

// accessor functions
void CLine::GetValues (int& nSP, int& nEP)
{
	nSP = m_nStartPoint;
	nEP = m_nEndPoint;
}

// accessor functions
void CLine::SetValues (int nSP, int nEP)
{
	// set the values
	m_nStartPoint = nSP;
	m_nEndPoint = nEP;
}
