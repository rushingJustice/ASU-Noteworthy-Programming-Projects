/* Example 8.4.1
   Copyright(c) 2002-08, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <stdexcept>  
#include "myvector.h"

// default constructor
CMyVector::CMyVector ()
{
    m_nRows = 0;
    m_pData = NULL;
}

// overloaded constructor
CMyVector::CMyVector (int nRows)
{
    assert (nRows > 0);         // valid # of rows?
    try
    {
        m_pData = new float[nRows]; // allocated memory
    }
    catch (std::bad_alloc)
    {
        std::cerr << "Unable to allocate memory.\n";
        assert (!m_pData);
        return;
    }
    m_nRows = nRows;            // store size of vector
}

// destructor
CMyVector::~CMyVector ()
{
    if (m_nRows > 0)
    {
        delete [] m_pData;      // release memory
        m_nRows = 0;
    }
}

// vector access function via value
float CMyVector::At (int nIndex) const
{
    // valid index?
    assert (nIndex >= 0 && nIndex < m_nRows);
    // return the value at location
    return m_pData[nIndex];
}

// vector access function via reference
float& CMyVector::At (int nIndex)
{
    // valid index?
    assert (nIndex >= 0 && nIndex < m_nRows);
    // return memory reference
    return m_pData[nIndex];
}

// display vector values
void CMyVector::Display (const std::string& szHeader) const
{
    // display specified header
    std::cout << szHeader << "\n";
    // show all values one value per line
    for (int i=0; i < m_nRows; i++)
    {
        // value-based At function invoked here
        std::cout << "[" << std::setw(2) << i << "]: "
            << At(i) << "\n";
    }
}

// dot product of vector with another
float CMyVector::DotProduct (const CMyVector& fV) const
{
    int i;
    float fDP = 0.0f;
    
    // are the two vectors compatible?
    assert (m_nRows == fV.m_nRows);
    // now compute the dot product
    for (i=0; i < m_nRows; i++)
    {
        fDP += At(i) * fV.At(i);    // invoke value-based At function
    }
    
    return fDP;
}

