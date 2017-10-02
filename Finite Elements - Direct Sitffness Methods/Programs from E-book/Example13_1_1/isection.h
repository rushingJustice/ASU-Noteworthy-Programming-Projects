/*********************************************
Example 13.1.1
Copyright(c) 2005-12, S. D. Rajan
Object-Oriented Numerical Analysis

*********************************************/

#ifndef __RAJAN_ISECTION_H__
#define __RAJAN_ISECTION_H__

#include <string>

class CISection
{
    public:
        CISection ();
        CISection (const CISection&);
        CISection (const std::string&, float, float,
                   float, float);
        ~CISection ();

        // helper functions
        void Display () const;
        // accessor functions
        void GetProperties (std::string&, float&, float&,
                            float&, float&) const;
        // modifier functions
        void SetProperties (const std::string&, const float,
                            const float, const float, const float);
        void SetProperties (const CISection&);

    private:
        std::string m_szID;   // identification tag
        float       m_fArea;  // x/s area
        float       m_fSyy;   // section modulus y-axis
        float       m_fSzz;   // section modulus z-axis
        float       m_fWeight;// weight per unit length
};

#endif