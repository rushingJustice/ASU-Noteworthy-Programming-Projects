/* EXAMPLE 9.1.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#ifndef __RAJAN_POINT_H__
#define __RAJAN_POINT_H__

#include <string>
#include <iostream>
using std::istream;
using std::ostream;

class CPoint
{
    // friend overloaded operator functions
    friend istream &operator>> (istream&, CPoint&);
    friend ostream &operator<< (ostream&, const CPoint&);

    public:
        // constructor
        CPoint ();              // default
        CPoint (const CPoint&); // copy constructor
        CPoint (float, float);  // overloaded

        // helper function
        void Display (const std::string&) const;
        // modifier function
        void SetValues (float, float);
        void SetValues (const CPoint&);
        // accessor function
        void GetValues (float&, float&) const;
        void GetValues (CPoint&) const;

        // overloaded operators
        CPoint& operator= (const CPoint&);
        CPoint operator+ (const CPoint&) const; 
        CPoint operator- (const CPoint&) const; 
        bool operator!= (const CPoint&) const;
        bool operator== (const CPoint&) const;

    private:
        float m_fXCoor;   // stores x-coordinate
        float m_fYCoor;   // stores y-coordinate
};


#endif