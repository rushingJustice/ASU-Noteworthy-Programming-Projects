/*********************************************
Time Class
Copyright(c) 2002, S. D. Rajan
All rights reserved

Object Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN__TIME_H__
#define __RAJAN__TIME_H__

class CTime {
	public:
		CTime ();	            // default constructor
		CTime (int, int, int);	// constructor
		~CTime ();			    // destructor

		// helper functions
        void Display (const std::string&, int nOrder=0) const;
		void TimeDifference (const CTime&, const CTime&);

		// modifier functions
		int SetTime (int, int, int);
		int SetTime (const CTime&);
		int SetHour (int);
		int SetMinute (int);
		int SetSecond (int);

		// accessor functions
		void GetTime (int&, int&, int&) const;
		void GetTime (CTime&) const;
        int  GetHour () const;
		int  GetMinute () const;
		int  GetSecond () const;

	private: // store in 24-hour format
		int m_nHour;
		int m_nMinute;
		int m_nSecond;

		// helper functions
		int ConvertToSeconds() const;
		void ConvertFromSeconds (int, int&, int&, int&) const;
};

#endif