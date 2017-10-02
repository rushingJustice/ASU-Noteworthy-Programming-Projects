/* EXAMPLE 4.1.2
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate the use of several functions in a program.
   (2) Use of vector data type as a function argument.

*/
#include <iostream>

// function prototypes
void ShowBanner ();
void GetValues (float fV[], const int nSize);
void AvgValue (const float fVEntries[], const int nSize,
               float& fAvg);
void MinMaxValues (const float fVEntries[], const int nSize,
                   float& fMinV, float& fMaxV);
float MaxValue (const float fVV[], const int nSize);
float MinValue (const float fVV[], const int nSize);

int main ()
{
    const int MAXVALUES = 5;
    float fVEntries[MAXVALUES]; // stores the user input values

    // print program banner
    ShowBanner ();
    
    // get the values from the user
    GetValues (fVEntries, MAXVALUES);

    // initialize
    float fAvgValue;    // stores the average value
    float fMaxValue;    // stores the maximum value
    float fMinValue;    // stores the minimum value

    // compute the average
    AvgValue (fVEntries, MAXVALUES, fAvgValue);

    // compute the min and max values
    MinMaxValues (fVEntries, MAXVALUES, fMinValue, fMaxValue);

    // display the results
    std::cout << "\n";
    std::cout << "DATA STATISTICS" << "\n";
    std::cout << "---------------" << "\n";
    std::cout << "Average value : " << fAvgValue << "\n";
    std::cout << "Minimum value : " << fMinValue << "\n";
    std::cout << "Maximum value : " << fMaxValue << "\n";
    
    return 0;
}

void ShowBanner ()
{
    std::cout <<"\tWELCOME TO EXAMPLE 4.1.2\n";
    std::cout <<"\t     S. D. Rajan        \n";
    std::cout <<"\t------------------------\n\n";
}

void GetValues (float fVV[], const int nSize)
{
    int i;
    
    // obtain the values one at a time
    for (i=0; i < nSize; i++)
    {
        std::cout << "Input element [" << i+1 << "]\a : ";
        std::cin >> fVV[i];
    }
}

void AvgValue (const float fVEntries[], const int nSize,
               float& fAvg)
{
    int i;

    // initialize
    fAvg = 0.0;

    // sum all the numbers
    for (i=0; i < nSize; i++)
        fAvg += fVEntries[i];

    // now the average
    fAvg /= static_cast<float>(nSize);

}

void MinMaxValues (const float fVEntries[], const int nSize,
                   float& fMinV, float& fMaxV)
{
    // compute the min and max values
    fMinV = MinValue (fVEntries, nSize);
    fMaxV = MaxValue (fVEntries, nSize);

}

float MinValue (const float fVV[], const int nSize)
{
    float fMinV;    // stores the min value
    int i;

    // set minimum value to the first entry
    fMinV = fVV[0];

    // now compare against the rest of the entries
    for (i=1; i < nSize; i++)
    {
        if (fVV[i] < fMinV) fMinV = fVV[i];
    }

    return fMinV;
}

float MaxValue (const float fVV[], const int nSize)
{
    float fMaxV;    // stores the max value
    int i;

    // set maximum value to the first entry
    fMaxV = fVV[0];

    // now compare against the rest of the entries
    for (i=1; i < nSize; i++)
    {
        if (fVV[i] > fMaxV) fMaxV = fVV[i];
    }

    return fMaxV;
}
