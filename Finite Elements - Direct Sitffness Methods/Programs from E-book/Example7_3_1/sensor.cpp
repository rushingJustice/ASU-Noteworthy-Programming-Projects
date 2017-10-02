/* EXAMPLE 7.4.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#include <iostream>
#include <string>
#include "sensor.h"

// default constructor
CSensor::CSensor ()
{
}

// destructor
CSensor::~CSensor ()
{

}

// modifier function
void CSensor::Set (const CTime& Time, const CPoint& Point,
                   float fTemp)
{
    m_TimeStamp.SetTime (Time);
    m_Location.SetValues (Point); 
    m_fTemperature = fTemp;
}

// accessor function
void CSensor::Get (CTime& Time, CPoint& Point,
                   float& fTemp) const
{
    m_TimeStamp.GetTime (Time);
    m_Location.GetValues (Point); 
    fTemp = m_fTemperature;
}

// helper function
void CSensor::Display () const
{
    std::cout << "Sensor Data ...\n";
    m_Location.Display (" Location: ");
    m_TimeStamp.Display (" Reading Time: ", 0);
    std::cout << " Temperature: " << m_fTemperature << ".\n";
}

