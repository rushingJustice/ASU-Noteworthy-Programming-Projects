/*********************************************
Sensor Class
Copyright(c) 2002, S. D. Rajan
All rights reserved

Object Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN__SENSOR_H__
#define __RAJAN__SENSOR_H__

#include "time.h"
#include "point.h"

class CSensor {
	public:
		CSensor ();	  // default constructor
		~CSensor ();  // destructor

		// helper functions
		void Display () const;

		// modifier functions
        void Set (const CTime&, const CPoint&, float);

		// accessor functions
        void Get (CTime&, CPoint&, float&) const;

	private:
        CTime        m_TimeStamp;       // time temperature taken
        CPoint       m_Location;        // location temperature taken
        float        m_fTemperature;    // temperature
};

#endif