/* STATISTICAL FUNCTIONS LIBRARY
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

*/
#include <cmath>
#include "stat.h"

///////////////////////////////////////
// functions used by the stat library
///////////////////////////////////////
bool IsEven (int n)
{
	if ((n % 2) == 0)
		return true;	// even number
	else
		return false;	// odd number
}

///////////////////////////////////////
// stat library functions
///////////////////////////////////////
float StatMean (const float fV[], int nSize)
{
    int i;
    float fSum = 0.0f;
    float fMean = 0.0;

    if (nSize <= 0)
        return (fMean);

    for (i=0; i < nSize; i++)
        fSum += fV[i];

    fMean = fSum/static_cast<float>(nSize);

    return (fMean);
}

float StatMedian (const float fV[], int nSize)
{
    float fMedian = 0.0f;
    if (nSize <= 0)
        return (fMedian);

    if (IsEven(nSize))
    {
        int n = nSize/2;
        fMedian = 0.5f*(fV[n-1] + fV[n]);
    }
    else
    {
        int n = (nSize+1)/2;
        fMedian = fV[n-1];
    }

    return (fMedian);
}

float StatStandardDeviation (const float fV[], int nSize)
{
    int i;
    float fStandardDeviation = 0.0f;
    
    float fMean = StatMean (fV, nSize);
    if (fMean == 0.0f)
        return (fMean);

    for (i=0; i < nSize; i++)
    {
        fStandardDeviation += static_cast<float>(pow(fV[i]-fMean, 2.0f));
    }

    float fTemp = fStandardDeviation/static_cast<float>(nSize);
    fStandardDeviation = static_cast<float>(sqrt(fTemp));

    return (fStandardDeviation);

}

float StatVariance (const float fV[], int nSize)
{
    float fVariance = StatStandardDeviation (fV, nSize);
    fVariance *= fVariance;

    return (fVariance);
}

float StatCoVariance (const float fVX[], const float fVY[],
                      int nSize)
{
    int i;
    float fCoVariance = 0.0f;

    float fMeanX = StatMean (fVX, nSize);
    float fMeanY = StatMean (fVY, nSize);

    if (fMeanX == 0.0f || fMeanY == 0.0f)
        return (fCoVariance);

    for (i=0; i < nSize; i++)
    {
        fCoVariance += (fVX[i]-fMeanX)*(fVY[i]-fMeanY);
    }
    fCoVariance /= static_cast<float>(nSize);

    return (fCoVariance);
}

