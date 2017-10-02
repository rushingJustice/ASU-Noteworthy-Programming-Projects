/*********************************************
Example 7.6.2
Copyright(c) 2000-05, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "..\library\simpleio.h"
#include "wireframe.h"

int main ()
{
	CWireFrame Object; // an object (structure) represented as a wireframe
	int nPoints, nLines;

	// obtain the number of points
	do {
		GetInteractive ("Total number of points: ", nPoints);
	} while (nPoints <= 1);

	// obtain the number of lines
	do {
		GetInteractive ("Total number of lines: ", nLines);
	} while (nLines <= 0);

	// set problem size
	Object.SetSize (nPoints, nLines);

	// obtain the point information
	Object.ReadPointData ();

	// obtain the line information
	Object.ReadLineData ();

	// compute the scaling factor, fSF
	Object.ComputeScale ();

	// now draw
	Object.DrawModel ();

	return 0;
}