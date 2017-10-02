/*********************************************
Example 13.1.2
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include "xsdatabase.h"
#include "../Example13_1_1/isection.h"

int main ()
{
	CXSDatabase DBISection; // load the 3 x/s
	CISection New ("W12X336",98.8f,483.0f,177.0f,336.0f);

	DBISection.Add (New);

	CISection FoundOne = DBISection.GetOne(0);
	FoundOne.Display ();

	FoundOne = DBISection.GetOne(3);
	FoundOne.Display ();

	return 0;
}