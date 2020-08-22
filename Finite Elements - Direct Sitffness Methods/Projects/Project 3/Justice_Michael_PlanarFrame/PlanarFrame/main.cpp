/*********************************************
Planar Frame Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#include "frame.h"
#include "..\library\clock.h"

int main (int argc, char *argv[])
{
    CFrame TheFrame; // the one and only frame!

	// show program banner
    TheFrame.Banner (std::cout);

	// Prepare for I/O
	TheFrame.PrepareIO (argc, argv);
	
    // start the timer --------------------------------------------------------
    CClock Timer;
    std::string szDateTime;
    Timer.GetDateTime (szDateTime);
    std::cout << "\nStarting out at : " << szDateTime << "\n";

    // read the data and analyze
    TheFrame.Analyze ();

    // end the timer --------------------------------------------------------
    // get the current date and time
    Timer.GetDateTime (szDateTime);
    std::cout << "\n      Ending at : " << szDateTime << "\n";
    // compute the elapsed time -----------------------------------------------
    std::cout << "Elapsed wall clock time: " << Timer.DiffTime ()
              << " seconds\n";

	return 0;
}