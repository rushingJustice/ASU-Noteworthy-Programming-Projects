/*********************************************
Example 7.6.2
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_WIREFRAME_H__
#define __RAJAN_WIREFRAME_H__

#pragma once

#include <vector>
using std::vector;

#include "point.h"
#include "line.h"

class CWireFrame
{
	public:
		CWireFrame ();
		~CWireFrame ();
		// helper functions
		void ReadPointData ();
		void ReadLineData ();
		void ComputeScale ();
		void Print ();
		void DrawModel ();
		// modifier functions
		void SetSize (int, int);
		// accessor functions
		void GetSize (int&, int&);
	private:
		int m_nPoints;
		int m_nLines;
		vector<CPoint> m_PointData;
		vector<CLine> m_LineData;
		float m_fXMin, m_fXMax;
		float m_fYMin, m_fYMax;
		float m_fSFX, m_fSFY;
		void GraphCoordinates (int nP, float& fXgs, float& fYgs);
		void Move (float fX, float fY);
		void Draw (float fX, float fY);
};

#endif