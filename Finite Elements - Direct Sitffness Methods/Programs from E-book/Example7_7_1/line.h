/*********************************************
Example 7.6.1
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_LINE_H__
#define __RAJAN_LINE_H__

#pragma once

class CLine
{
	public:
		CLine (int nSP=0, int nEP=0);
		~CLine ();
		// helper functions
		void Print ();
		// accessor functions
		void GetValues (int&, int&);
		// modifier functions
		void SetValues (int, int);
	private:
		int m_nStartPoint;
		int m_nEndPoint;
};

#endif