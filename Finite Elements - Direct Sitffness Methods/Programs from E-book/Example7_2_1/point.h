/* EXAMPLE 7.2.1
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
        void Display (const std::string);
        // modifier function
        void SetValues (float, float);
        // accessor function
        void GetValues (float&, float&);

    private:
        float m_fXCoor;   // stores x-coordinate
        float m_fYCoor;   // stores y-coordinate
};
#endif