/* EXAMPLE 12.3.2
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

*********************************************/
#ifndef __RAJAN_POINT_H__
#define __RAJAN_POINT_H__

#include <string>
#include <iostream>

const int MAXIDTAG = 10;

class CPoint
{
    // friend overloaded operator functions
    friend std::istream &operator>> (std::istream&, 
                                     CPoint&);
    friend std::ostream &operator<< (std::ostream&,
                                     const CPoint&);

    public:
        // constructor
        CPoint ();              // default
        CPoint (const CPoint&); // copy constructor
        CPoint (const std::string&, 
                float, float);  // overloaded

        // helper function
        void Display (const std::string&) const;
        // modifier function
        void SetTag (const std::string&);
        void SetCoords (float, float);
        void SetValues (const CPoint&);
        // accessor function
        void GetTag (std::string&) const;
        void GetCoords (float&, float&) const;
        void GetValues (CPoint&) const;

        // overloaded operators
        CPoint& operator= (const CPoint&);
        CPoint operator+ (const CPoint&) const; 
        CPoint operator- (const CPoint&) const; 
        bool operator!= (const CPoint&) const;
        bool operator== (const CPoint&) const;

    private:
        char  m_szTag[MAXIDTAG+1]; // stores point's id tag
        float m_fXCoor;            // stores x-coordinate
        float m_fYCoor;            // stores y-coordinate
};


#endif