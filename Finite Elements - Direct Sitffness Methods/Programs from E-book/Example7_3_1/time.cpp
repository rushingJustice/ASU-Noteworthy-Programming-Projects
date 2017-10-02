/*********************************************
Time Class
Copyright(c) 2002, S. D. Rajan
All rights reserved

Object Oriented Numerical Analysis
*********************************************/
#include <cassert>
#include <iostream>
#include <string>
#include <iomanip>
#include "time.h"

// default constructor
CTime::CTime ()
{
	m_nHour = 0;
	m_nMinute = 0;
	m_nSecond = 0;
}

CTime::CTime (int nH, int nM, int nS)
{
	// valid input?
	assert (nH >= 0 && nH <= 23);
	assert (nM >= 0 && nM <= 59);
	assert (nS >= 0 && nS <= 59);

	// set the values
	m_nHour = nH;
	m_nMinute = nM;
	m_nSecond = nS;
}

// destructor
CTime::~CTime ()
{
}

// helper functions
void CTime::Display (const std::string& szBanner, int nOrder) const
{
    if (nOrder == 0)
        std::cout << szBanner
                  << std::setw(2) << m_nHour   << ":"
                  << std::setw(2) << m_nMinute << ":"
                  << std::setw(2) << m_nSecond << ".";
    else
        std::cout << szBanner
                  << std::setw(2) << m_nHour   << " Hour(s) "     
                  << std::setw(2) << m_nMinute << " Minute(s) "  
                  << std::setw(2) << m_nSecond << " Seconds(s) "
                  << ".";
}

void CTime::TimeDifference (const CTime& TFrom, const CTime& TTo)
{
	// find the difference between the two times in seconds
	int nTFrom = TFrom.ConvertToSeconds();
	int nTTo = TTo.ConvertToSeconds();

	int nDiff = nTTo - nTFrom;
	// adjust if time crosses midnight
	if (nDiff < 0)
		nDiff = nDiff + 86400;

	// now convert from seconds back to hr:min:s
	ConvertFromSeconds (nDiff, m_nHour, m_nMinute, m_nSecond);
}

int CTime::ConvertToSeconds () const
{
	return (m_nHour*3600 + m_nMinute*60 + m_nSecond);
}

void CTime::ConvertFromSeconds (int nTime, int& nH, int &nM, int& nS) const
{
	nH = (nTime/3600);
	nTime = nTime - nH*3600;
	nM = (nTime/60);
	nTime = nTime - nM*60;
	nS = nTime;
}

// accessor functions
void CTime::GetTime (int& nH, int& nM, int& nS) const
{
	nH = m_nHour;
	nM = m_nMinute;
	nS = m_nSecond;
}

void CTime::GetTime (CTime& Time) const
{
	Time.SetTime (m_nHour, m_nMinute, m_nSecond);
}

int CTime::GetHour () const
{
	return (m_nHour);
}

int CTime::GetMinute () const
{
	return (m_nMinute);
}

int CTime::GetSecond () const
{
	return (m_nSecond);
}

// modifier functions
int CTime::SetTime (int nH, int nM, int nS)
{
	// check for valid values
	if (nH < 0 || nH > 23) return(1);
	if (nM < 0 || nM > 59) return(1);
	if (nS < 0 || nS > 59) return(1);

	// set the values
	m_nHour = nH;
	m_nMinute = nM;
	m_nSecond = nS;
	return (0);
}

int CTime::SetTime (const CTime& Time)
{
	// set the values
	m_nHour = Time.m_nHour;
	m_nMinute = Time.m_nMinute;
	m_nSecond = Time.m_nSecond;
	return (0);
}

int CTime::SetHour (int nH)
{
	if (nH < 0 || nH > 23) return(1);

	m_nHour = nH;
	return (0);
}

int CTime::SetMinute (int nM)
{
	if (nM < 0 || nM > 59) return(1);

	m_nMinute = nM;
	return (0);
}

int CTime::SetSecond (int nS)
{
	if (nS < 0 || nS > 59) return(1);

	m_nSecond = nS;
	return (0);
}

