#include <iostream>
#include <cmath>
#include <string>
#include "triangle.h"

const float PI= 3.1415926f;

CTriangle::CTriangle()   
{
	 m_ID = "Undefined";
	 m_fArea = m_fPerimeter = 0.0f;
     m_fAngle1= m_fAngle2 = m_fAngle3 = 0.0f;
}

CTriangle::CTriangle (const std::string& szID, const CPoint& P1, 
					  const CPoint& P2, const CPoint& P3)
{
    m_ID = szID;
	m_P1 = P1;
    m_P2 = P2;
    m_P3 = P3;
	ComputeProperties ();  
}

CTriangle::CTriangle (const CPoint& P1, const CPoint& P2, 
					  const CPoint& P3)
{
	m_P1 = P1;
    m_P2 = P2;
    m_P3 = P3;
	ComputeProperties ();  
}

CTriangle ::~CTriangle ()   // dtor
{
}

void CTriangle::ComputeProperties ()  
{
}

bool CTriangle::IsIsosceles() const             
{
    float fA = m_P2.DistanceTo (m_P3);  
	float fB = m_P1.DistanceTo (m_P3);
	float fC = m_P1.DistanceTo (m_P2);

	if (fA == fB || fC == fB || fA == fC)     
		return true;
	else
		return false;
}

bool CTriangle::IsRightAngle()        
{
    float fAngle1 = GetAngle1 ();
    float fAngle2 = GetAngle2 ();
    float fAngle3 = GetAngle3 ();
	if (abs(fAngle1-90) < 0.01 || abs(fAngle2-90)< 0.01
        || abs(fAngle3-90)< 0.01)   
		return true;
	else
		return false;
}

bool CTriangle::IsEquilateral()        
{
    float fA = m_P2.DistanceTo (m_P3); 
	float fB = m_P1.DistanceTo (m_P3);
	float fC = m_P1.DistanceTo (m_P2);
	if (fA == fB && fB == fC)
		return true;
	else
		return false;
}

void CTriangle::Display(const std::string& szMessage)   
{
	std::cout << szMessage << "\n";
    std::cout << "ID       : " << m_ID     << "\n";
	std::cout << "Area     : " << m_fArea  << "\n"
			  << "Perimeter: " << m_fPerimeter << "\n"
			  << "Angle1   : " << m_fAngle1<< "\n"
			  << "Angle2   : " << m_fAngle2<< "\n"
			  << "Angle3   : " << m_fAngle3<< "\n";
}

