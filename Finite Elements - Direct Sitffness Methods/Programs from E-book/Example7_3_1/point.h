/* EXAMPLE 7.3.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#ifndef __RAJAN_POINT_H__
#define __RAJAN_POINT_H__

#include <string>

class CPoint
{
    public:
        // constructor
        CPoint ();              // default
        CPoint (float, float);  // overloaded
        // helper function
        void Display (const std::string&) const;
        // modifier function
        void SetValues (float, float);
        void SetValues (const CPoint&);
        // accessor function
        void GetValues (float&, float&) const;
        void GetValues (CPoint&) const;

    private:
        float m_fXCoor;   // stores x-coordinate
        float m_fYCoor;   // stores y-coordinate
};
#endif