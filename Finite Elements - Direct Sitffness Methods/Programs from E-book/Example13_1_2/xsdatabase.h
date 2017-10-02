/*********************************************
Example 13.1.2
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN__XSDATABASE_H__
#define __RAJAN__XSDATABASE_H__

#include <vector>
#include <fstream>
#include "../Example13_1_1/ISection.h"

class CXSDatabase
{
    public:
        CXSDatabase ();
        ~CXSDatabase ();
        // helper functions
        CISection GetOne (int) const;
        // accessor functions
        int GetSize () const;
        // modifier functions
        void Add (const CISection& ISection);
        int  Remove (const CISection& ISection);

    private:
        std::vector<CISection> m_ListofISections; 
                               // list of available sections
        int m_nSize;           // # of sections
        std::ifstream m_IFile; // (file) source of database
};

#endif