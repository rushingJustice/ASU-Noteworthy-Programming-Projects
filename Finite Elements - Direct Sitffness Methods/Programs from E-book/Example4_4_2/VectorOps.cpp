/* VECTOR FUNCTIONS LIBRARY
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
// include files
#include <cmath>

// global variables
extern int nFLOPs; // nFPOps is defined somewhere else
                   // in the program

float VectorDotProduct (const float fVA[], const float fVB[],
                  int nSize)
{
    int i;
    float fSum=0.0f;

    for (i=0; i < nSize; i++)
        fSum += fVA[i] * fVB[i];

    nFLOPs += nSize;

    return (fSum);
}

float VectorLength (const float fV[], int nSize)
{
    int i;
    float fLength = 0.0f;

    for (i=0; i < nSize; i++)
    {
        fLength += fV[i] * fV[i];
    }
    fLength = static_cast<float>(sqrt(fLength));

    // take square root as one FLOP
    nFLOPs += nSize+1;

    return (fLength);
}
