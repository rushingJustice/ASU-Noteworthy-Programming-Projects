/* EXAMPLE 7.3.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate class composition.

*********************************************/
#include <iostream>

#include "sensor.h"
#include "..\library\getinteractive.h"

int main ()
{
    const int NUMSENSORS = 4;
    CSensor SensorData[NUMSENSORS]; // stores sensor data in a vector

    // variable to generate the sensor data
    int    i;            // loop index
    float  fV[2];        // x-y coordinates
    int    nV[3];        // time data
    float  fTemp;        // temperature
    CPoint Location;     // to store location
    CTime  ReadingTime;  // to store time of reading

    // get the sensor data from the user
    for (i=0; i < NUMSENSORS; i++)
    {
        // location, time and temperature
        GetInteractive ("Sensor (x,y) location : ", fV, 2);
        GetInteractive ("Time of reading (hr:min:sec) : ", nV, 3);
        GetInteractive ("Temperature : ", fTemp);

        // construct the location object
        Location.SetValues (fV[0], fV[1]);

        // construct the time object
        ReadingTime.SetTime (nV[0], nV[1], nV[2]);

        // set the sensor data
        SensorData[i].Set (ReadingTime, Location, fTemp);
    }

    // display the sensor information
    for (i=0; i < NUMSENSORS; i++)
    {
        SensorData[i].Display ();
    }

    // all done
    return 0;
}