/*********************************************
Example 13.1.1
Copyright(c) 2005-06, S. D. Rajan
Object-Oriented Numerical Analysis

*********************************************/

#include "isection.h"

int main ()
{
	CISection First ("W36X300", 88.3f, 1110.0f, 156.0f, 300.0f);
	First.Display ();
	
	CISection Second ("W21X201", 59.2f, 461.0f, 86.1f, 201.0f);
	Second.Display ();

	CISection Third;
	Third.SetProperties ("W12X336", 98.8f, 483.0f, 177.0f, 336.0f);
	Third.Display ();

	CISection Fourth (Second);
	// Fourth.Set (Second);    // alternate way
	Fourth.Display ();
	
	return 0;
}