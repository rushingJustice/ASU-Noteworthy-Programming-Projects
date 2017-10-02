/* EXAMPLE 4.2.1
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of overloaded functions.

*/
#include <iostream>

// overloaded function prototypes
void GetValues (int nVV[], const int nSize);
void GetValues (double dVV[], const int nSize);
int   MinValue (const int nVV[], int nSize);
double MinValue (const double dVV[], int nSize);

int main ()
{
    const int NUMELEMENTS = 5;

    // integer values
    int nValues[NUMELEMENTS];
    std::cout << "Input " << NUMELEMENTS << " integers.\n";
    GetValues (nValues, NUMELEMENTS);
    int nMinValue = MinValue (nValues, NUMELEMENTS);
    std::cout << "Minimum value is : " << nMinValue << "\n";

    // double values
    double dValues[NUMELEMENTS];
    std::cout << "Input " << NUMELEMENTS << " doubles.\n";
    GetValues (dValues, NUMELEMENTS);
    double dMinValue = MinValue (dValues, NUMELEMENTS);
    std::cout << "Minimum value is : " << dMinValue << "\n";

    return 0;
}

void GetValues (int nVV[], const int nSize)
{
    int i;
    
    // obtain the values one at a time
    for (i=0; i < nSize; i++)
    {
        std::cout << "Input element [" << i+1 << "]\a : ";
        std::cin >> nVV[i];
    }
}

void GetValues (double dVV[], const int nSize)
{
    int i;
    
    // obtain the values one at a time
    for (i=0; i < nSize; i++)
    {
        std::cout << "Input element [" << i+1 << "]\a : ";
        std::cin >> dVV[i];
    }
}

int MinValue (const int nVV[], int nSize)
{
    int nMinV;    // stores the min value
    int i;

    // set minimum value to the first entry
    nMinV = nVV[0];

    // now compare against the rest of the entries
    for (i=1; i < nSize; i++)
    {
        if (nVV[i] < nMinV) nMinV = nVV[i];
    }

    return nMinV;
}

double MinValue (const double dVV[], int nSize)
{
    double dMinV;    // stores the min value
    int i;

    // set minimum value to the first entry
    dMinV = dVV[0];

    // now compare against the rest of the entries
    for (i=1; i < nSize; i++)
    {
        if (dVV[i] < dMinV) dMinV = dVV[i];
    }

    return dMinV;
}
