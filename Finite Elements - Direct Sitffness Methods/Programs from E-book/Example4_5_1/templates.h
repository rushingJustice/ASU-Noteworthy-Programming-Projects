/* EXAMPLE 4.5.1
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis
*/

#ifndef __RAJAN__TEMPLATES_H__
#define __RAJAN__TEMPLATES_H__

template <class T>
void GetValues (T TVV[], const int nSize)
{
    int i;
    
    // obtain the values one at a time
    for (i=0; i < nSize; i++)
    {
        std::cout << "Input element [" << i+1 << "]\a : ";
        std::cin >> TVV[i];
    }
}

template <class T>
T MinValue (const T TVV[], int nSize)
{
    T TMinV;    // stores the min value
    int i;

    // set minimum value to the first entry
    TMinV = TVV[0];

    // now compare against the rest of the entries
    for (i=1; i < nSize; i++)
    {
        if (TVV[i] < TMinV) TMinV = TVV[i];
    }

    return TMinV;
}

#endif