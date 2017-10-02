/* EXAMPLE 7.2.2
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of classes and objects.
   (2) Use of public and private member functions.
   (3) Rudimentary error trapping.

*********************************************/
#include <iostream>

#include "time.h"
#include "..\library\getinteractive.h"

int main ()
{
	// declare variables (objects) to store start and end times
	CTime StartTime;
	CTime EndTime;
    int nState;         // error indicator
	
	// local variable to hold time data (hour, min and sec)
	int nV[3];

	// get start time
	GetInteractive ("Enter start time. Hour, Minute and Second: ",
        nV, 3);
	nState = StartTime.SetTime (nV[0], nV[1], nV[2]);
    // valid time value?
	if (nState != 0)
        std::cout << "Invalid input. Check the values.\n";

	// get end time
	GetInteractive ("Enter end time. Hour, Minute and Second: ",
        nV, 3);
	nState = EndTime.SetTime (nV[0], nV[1], nV[2]);
    // valid time value?
	if (nState != 0)
        std::cout << "Invalid input. Check the values.\n";

	// compute difference in time and display
	CTime TimeDiff;
	TimeDiff.TimeDifference (StartTime, EndTime);
	std::cout << "\nTime difference between "; StartTime.Print();
    std::cout << "\n                    and "; EndTime.Print ();
    std::cout << "\n                     is "; TimeDiff.Print (1);
    std::cout << std::endl;

	return 0;
}