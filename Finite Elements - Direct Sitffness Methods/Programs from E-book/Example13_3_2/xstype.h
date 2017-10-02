/*********************************************
Example 13.3.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __CXSTYPE__H__
#define __CXSTYPE__H__

#include <string>
#include "../library/vectortemplate.h"

class CXSType
{
    public:
        CXSType ();
        CXSType (int);
        ~CXSType ();
        // helper function
        void DisplayProperties ();
        // accessor functions
        void GetProperties (std::string&, float&, float&, float&);
        void GetDimensions (std::string&, CVector<float>&);
        virtual void DisplayDimensions () = 0;
        virtual void ComputeProperties () = 0;
    private:
        void Initialize ();
    protected:
        std::string m_szID;            // identification tag
        float m_fArea;                 // x/s area
        float m_fSyy;                  // section modulus y-axis
        float m_fSzz;                  // section modulus z-axis
        int   m_numDimensions;         // number of dimensions
        CVector<float> m_fVDimensions; // the dimensions
};

#endif