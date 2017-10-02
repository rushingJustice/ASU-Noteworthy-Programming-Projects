/*********************************************
Example 7.6.1
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <vector>
using std::vector;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include "..\library\simpleio.h"
#include "point.h"
#include "line.h"

// prototypes
void ComputeScale (const vector<CPoint>&, const int, float&);
void GraphCoordinates (const float, const float, const float,
					   float&, float&);
void Move (const float, const float);
void Draw (const float, const float);

int main ()
{
	int nPoints;	// total number of points
	int nLines;		// total number of lines
	
	// obtain the number of points
	do {
		GetInteractive ("Total number of points: ", nPoints);
	} while (nPoints <= 1);

	// obtain the number of lines
	do {
		GetInteractive ("Total number of lines: ", nLines);
	} while (nLines <= 0);

	// vectors to store the point and line data
	vector<CPoint> PointData(nPoints); // stores point data
	vector<CLine>  LineData(nLines);   // stores line data

	// obtain the nodal coordinates
	for (int i=1; i <= nPoints; i++) {
		float fX, fY;
		{
			ostringstream szPrompt;
			szPrompt << "X coordinate for point " << i << ": ";
			GetInteractive (szPrompt.str(), fX);
		}
		{
			ostringstream szPrompt;
			szPrompt << "Y coordinate for point " << i << ": ";
			GetInteractive (szPrompt.str(), fY);
		}
		PointData[i-1].SetValues (fX, fY);
	}

	// obtain the start and end point numbers
	for (int i=1; i <= nLines; i++) {
		int nSP, nEP, nTemp;
		do {
			ostringstream szPrompt;
			szPrompt << "Start point for line " << i << ": ";
			GetInteractive (szPrompt.str(), nTemp);
		} while (nTemp < 1 || nTemp > nPoints);
		nSP = nTemp;
		do {
			ostringstream szPrompt;
			szPrompt << "End point for line " << i << ": ";
			GetInteractive (szPrompt.str(), nTemp);
		} while (nTemp < 1 || nTemp > nPoints);
		nEP = nTemp;
		LineData[i-1].SetValues (nSP, nEP);
	}

	// compute the scaling factor, fSF
	float fSF;
	ComputeScale (PointData, nPoints, fSF);

	// now draw
	for (int i=1; i <= nLines; i++) {
		float fX, fY;
		float fXgs, fYgs;
		int nSP, nEP;

		// get line information
		LineData[i-1].GetValues (nSP, nEP);
		// get start point coordinates
		PointData[nSP-1].GetValues (fX, fY);
		// compute graph coordinates
		GraphCoordinates (fX, fY, fSF, fXgs, fYgs);
		Move (fXgs, fYgs);

		// get end point coordinates
		PointData[nEP-1].GetValues (fX, fY);
		// compute graph coordinates
		GraphCoordinates (fX, fY, fSF, fXgs, fYgs);
		Draw (fXgs, fYgs);
	}

	return 0;
}