/* STATISTICAL FUNCTIONS LIBRARY
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

   Header file for the Statistical Library
*/
#ifndef __RAJAN_STAT_H__
#define __RAJAN_STAT_H__

// function prototypes
float StatMean   (const float fV[], int nSize);
float StatMedian (const float fV[], int nSize);
float StatStandardDeviation (const float fV[], int nSize);
float StatVariance (const float fV[], int nSize);
float StatCoVariance (const float fVX[], const float fVY[],
                      int nSize);

#endif