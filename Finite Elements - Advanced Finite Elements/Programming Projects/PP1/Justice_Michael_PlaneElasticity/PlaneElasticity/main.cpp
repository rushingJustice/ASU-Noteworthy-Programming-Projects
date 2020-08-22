/*********************************************
Planr Stress/Strain T6 FE Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "frame.h"
#include "..\library\clock.h"

int main (int argc, char *argv[])
{
    CFrame ThePlanarFE; // the one and only planar FE!

	// show program banner
    ThePlanarFE.Banner (std::cout);

	// Prepare for I/O
	ThePlanarFE.PrepareIO (argc, argv);
	
    // start the timer --------------------------------------------------------
    CClock Timer;
    std::string szDateTime;
    Timer.GetDateTime (szDateTime);
    std::cout << "\nStarting out at : " << szDateTime << "\n";

    // read the data and analyze
    ThePlanarFE.Analyze ();

    // end the timer --------------------------------------------------------
    // get the current date and time
    Timer.GetDateTime (szDateTime);
    std::cout << "\n      Ending at : " << szDateTime << "\n";
    // compute the elapsed time -----------------------------------------------
    std::cout << "Elapsed wall clock time: " << Timer.DiffTime ()
              << " seconds\n";

	return 0;
}