#ifndef __RAJAN_TRIANGLE_H__
#define __RAJAN_TRIANGLE_H__

#include "point.h"

class CTriangle
{
	public:
		CTriangle();  
		~CTriangle(); 
		CTriangle (const std::string& szID,
			       const CPoint& P1, const CPoint& P2, const CPoint& P3);
		CTriangle (const CPoint& P1, const CPoint& P2, const CPoint& P3);
		bool IsIsosceles() const;    
		bool IsRightAngle();
	    bool IsEquilateral ();
		void Display (const std::string& szMessage);
        float GetAngle1 () { return m_fAngle1; };
        float GetAngle2 () { return m_fAngle2; };
        float GetAngle3 () { return m_fAngle3; };
        
    private:
		std::string m_ID;
		CPoint m_P1;
        CPoint m_P2;
        CPoint m_P3;
        float m_fPerimeter;
        float m_fArea;
        float m_fAngle1;
        float m_fAngle2; 
        float m_fAngle3;
		void ComputeProperties();

	protected:
};

#endif  __RAJAN_TRIANGLE_H__