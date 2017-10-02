/* EXAMPLE 4.4.1
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate modular program development.
   (2) Show how to debug programs.

*/
#include <iostream>
#include "stat.h"

void ComputeStats (const float fV[], int nSize);

int main ()
{
    const int SIZEA = 3;
    const int SIZEB = 3;
    const int SIZEC = 4;

    // define and populate the vectors
    float fVA[SIZEA] = {-5.1f, 12.3f, 33.9f};
    float fVB[SIZEB] = {5.1f,  19.0f, 22.3f};
    float fVC[SIZEC] = {-900.3f, -18.7f, 33.4f, 123.5f};

    // compute stats for vector A
    std::cout << "   Stats for vector A\n";
    std::cout << "   ------------------\n";
    ComputeStats (fVA, SIZEA);

    // compute stats for vector B
    std::cout << "   Stats for vector B\n";
    std::cout << "   ------------------\n";
    ComputeStats (fVB, SIZEB);

    // compute stats for vector C
    std::cout << "   Stats for vector C\n";
    std::cout << "   ------------------\n";
    ComputeStats (fVC, SIZEC);

    // covariance
    float fCoVariance = StatCoVariance (fVA, fVB, SIZEA);
    std::cout << "Covariance - vectors A and B: " <<
        fCoVariance << "\n\n";

    return 0;
}

void ComputeStats (const float fV[], int nSize)
{
    float fMean = StatMean (fV, nSize);
    float fMedian = StatMedian (fV, nSize);
    float fStandardDeviation = StatStandardDeviation (fV, nSize);
    float fVariance = StatVariance (fV, nSize);

    std::cout << "                Mean: " << fMean << "\n";
    std::cout << "              Median: " << fMedian << "\n";
    std::cout << "  Standard Deviation: " << fStandardDeviation << "\n";
    std::cout << "            Variance: " << fVariance << "\n\n";
}