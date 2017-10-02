/*********************************************
Example 10.3.1
Copyright(c) 2000, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
*********************************************/
#ifndef __RAJAN_TIME_H__
#define __RAJAN_TIME_H__

#pragma once

class CTime {
	public:
		CTime ();	            // default constructor
		CTime (int, int, int);	// constructor
		~CTime ();				// destructor

		// helper functions
		void Print (int nOrder=0);
		void TimeDifference (CTime&, CTime&);

		// modifier functions
		int SetTime (int, int, int);
		int SetHour (int);
		int SetMinute (int);
		int SetSecond (int);

		// accessor functions
		void GetTime (int&, int&, int&);
		int  GetHour ();
		int  GetMinute ();
		int  GetSecond ();

	private: // store in 24-hour format
		int m_nHour;
		int m_nMinute;
		int m_nSecond;
		// helper function
		int ConvertToSeconds();
		void ConvertFromSeconds (int, int&, int&, int&);
};

#endif