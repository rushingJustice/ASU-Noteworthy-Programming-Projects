/*********************************************
Example 7.6.2
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "wireframe.h"

#include <iostream>
using std::cout;
using std::endl;

#include <assert.h>

#include <sstream>
using std::ostringstream;

//#include "../library/simpleio.h"
#include "..\library\simpleio.h"

CWireFrame::CWireFrame ()
{
	m_nPoints = 0;
	m_nLines = 0;
}

CWireFrame::~CWireFrame ()
{
}

void CWireFrame::SetSize (int nP, int nL)
{
	// valid data?
	assert (nP > 1 && nL > 0);
	m_nPoints = nP;
	m_nLines = nL;

	// set the size of the point and line vectors
	m_PointData.resize(m_nPoints); 
	m_LineData.resize(m_nLines); 
}

void CWireFrame::GetSize (int& nP, int& nL)
{
	nP = m_nPoints;
	nL = m_nLines;
}

void CWireFrame::ReadPointData ()
{
	// read the point coordinates
	for (int i=1; i <= m_nPoints; i++) {
		float fX, fY;
		{
			ostringstream szPrompt;
			szPrompt << "X coordinate for point " << i << ": ";
			GetInteractive (szPrompt.str(), fX);
		}
		{
			ostringstream szPrompt;
			szPrompt << "Y coordinate for point " << i << ": ";
			GetInteractive (szPrompt.str(), fY);
		}
		m_PointData[i-1].SetValues (fX, fY);
	}
}

void CWireFrame::ReadLineData ()
{
	for (int i=1; i <= m_nLines; i++) {
		int nSP, nEP, nTemp;
		do {
			ostringstream szPrompt;
			szPrompt << "Start point for line " << i << ": ";
			GetInteractive (szPrompt.str(), nTemp);
		} while (nTemp < 1 || nTemp > m_nPoints);
		nSP = nTemp;
		do {
			ostringstream szPrompt;
			szPrompt << "End point for line " << i << ": ";
			GetInteractive (szPrompt.str(), nTemp);
		} while (nTemp < 1 || nTemp > m_nPoints);
		nEP = nTemp;
		m_LineData[i-1].SetValues (nSP, nEP);
	}
}

void CWireFrame::Print ()
{
	cout << endl;
	cout << "Number of points: " << m_nPoints << endl;
	cout << " Number of lines: " << m_nLines << endl;

	cout << "Point Data" << endl;
	for (int i=1; i <= m_nPoints; i++)
		m_PointData[i-1].Print();

	cout << "Line Data" << endl;
	for (int i=1; i <= m_nLines; i++)
		m_LineData[i-1].Print();

}

void CWireFrame::ComputeScale ()
{
	// find (xmin, xmax), (ymin, ymax)
	float fXC, fYC;
	m_PointData[0].GetValues (fXC, fYC);
	m_fXMin = m_fXMax = fXC;
	m_fYMin = m_fYMax = fYC;

	for (int i=1; i < m_nPoints; i++) {
		m_PointData[i].GetValues (fXC, fYC);
		m_fXMin = (fXC < m_fXMin? fXC : m_fXMin);
		m_fXMax = (fXC > m_fXMax? fXC : m_fXMax);
		m_fYMin = (fYC < m_fYMin? fYC : m_fYMin);
		m_fYMax = (fYC > m_fYMax? fYC : m_fYMax);
	}

	// range in the x and y directions
	float fXRange = m_fXMax - m_fXMin;
	float fYRange = m_fYMax - m_fYMin;

	// assume 6" x 6" graph paper
	m_fSFX = 6.0f/fXRange;
	m_fSFY = 6.0f/fYRange;
}

void CWireFrame::DrawModel ()
{
	for (int i=1; i <= m_nLines; i++) {
		float fXgs, fYgs;
		int nSP, nEP;

		// get line information
		m_LineData[i-1].GetValues (nSP, nEP);
		// compute graph coordinates
		GraphCoordinates (nSP, fXgs, fYgs);
		Move (fXgs, fYgs);

		// compute graph coordinates
		GraphCoordinates (nEP, fXgs, fYgs);
		Draw (fXgs, fYgs);
	}

}

void CWireFrame::GraphCoordinates (int nP, float& fXgs, float& fYgs)
{
	float fX, fY;
	
	// get point coordinates
	m_PointData[nP-1].GetValues (fX, fY);
	// compute graph coordinates
	fXgs = (fX - m_fXMin) * m_fSFX;
	fYgs = (fY - m_fYMin) * m_fSFY;
}

void CWireFrame::Move (float fX, float fY)
{
	cout << "Move : " << fX << ":" << fY << endl;
}

void CWireFrame::Draw (float fX, float fY)
{
	cout << "Draw : " << fX << ":" << fY << endl;
}