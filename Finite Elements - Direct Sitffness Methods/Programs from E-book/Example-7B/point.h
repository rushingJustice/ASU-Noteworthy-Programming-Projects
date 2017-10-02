#ifndef __RAJAN_POINT_H__
#define __RAJAN_POINT_H__
#include <string>

class CPoint
{
	public:
		CPoint (); // default ctor
		~CPoint (); // dtor
		CPoint (float fX, float fY, float fZ); // overloaded ctor
		CPoint (float fX, float fY); // overloaded ctor
		CPoint (const CPoint&);
		
		// helper function
        void Display (const std::string&) const; // displays x and y coordinates
		float DistanceTo (const CPoint&) const;
		void UnitVector (const CPoint&, float fVU[3]) const;
		bool IsOrigin () const;
		
		// modifier function
		void SetCoordinates (float fX, float fY,float fZ);
		void SetCoordinates (float fX, float fY);
		
		// accessor function
		void GetCoordinates (float& fX, float& fY, float& fZ) const;
		void GetCoordinates (float& fX, float& fY) const;

	private:
		float m_fXCoor;
		float m_fYCoor;
		float m_fZCoor;

	protected:
};

#endif __RAJAN_POINT_H__
