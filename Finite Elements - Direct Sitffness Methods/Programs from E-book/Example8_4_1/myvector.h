/* Example 8.2.4
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
#ifndef __RAJAN__MYVECTOR__H_
#define __RAJAN__MYVECTOR__H_

#include <string>

class CMyVector
{
    public:
        CMyVector ();           // default constructor
        CMyVector (int nRows);  // constructor
        ~CMyVector ();          // destructor

        // helper functions
        int GetSize () const { return m_nRows; };
        float At (int) const;
        float& At (int);
        void Display (const std::string&) const;

        // vector operations
        float DotProduct (const CMyVector&) const;
        
    public:
        float *m_pData;         // where the vector data are

    private:
        int m_nRows;            // # of rows
};

#endif