/*********************************************
Example 7.6.2
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_POINT_H__
#define __RAJAN_POINT_H__

#pragma once

class CPoint
{
	public:
		CPoint (float fX=0.0f, float fY=0.0f);
		CPoint (float fX, float fY, float fZ);
		~CPoint ();
		// helper functions
		void Print ();
		// accessor functions
		void GetValues (float&, float&);
		void GetValues (float&, float&, float&);
		// modifier functions
		void SetValues (float, float);
		void SetValues (float, float, float);
	private:
		float m_fXCoor;
		float m_fYCoor;
		float m_fZCoor;
};

#endif