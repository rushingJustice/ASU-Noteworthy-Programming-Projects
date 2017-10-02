/* COMPREHENSIVE EXAMPLE FOR CHAPTER 2
   Copyright(c) 2015, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   Review the material learnt in Chapter 2 and use the program as a checklist for the major
   concepts

*/

#include <iostream>  // header file for input-output
#include <cmath>     // header file for math functions
#include <iomanip>   // header file for i/o format control

int main ()
{
    std::cout << "\tReviewing Material from Chapter 2\n"
              << "\t---------------------------------\n\n";
    // -----------------------------------------------------------------
    // compute the properties of a beam with a rectangular cross-section
    // -----------------------------------------------------------------
    std::cout << "Input will assume length units as m\n";

    double dWidth;                             // width of the beam
    std::cout << " Width of the beam: ";       // get the width value from the keyboard
    std::cin >> dWidth;

    double dHeight;                            // height of the beam
    std::cout << "Height of the beam: ";       // get the height value from the keyboard
    std::cin >> dHeight;

    double dLength;                            // length of the beam
    std::cout << "Length of the beam: ";       // get the length value from the keyboard
    std::cin >> dLength;

    // compute the cross-sectional area, major moment of inertia and
    // maximum first moment of area (Q)
	double dCrossprops[3];

    auto A = dWidth*dHeight;
    auto I = dWidth*pow(dHeight, 3.0)/12.0;
    auto Q = dWidth*(0.5*dHeight)*(0.25*dHeight);
    std::cout << "\n";
    std::cout << "    Cross-sectional area: " << A << " m^2\n";
    std::cout << " Major moment of inertia: " << I << " m^4\n";
    std::cout << "First moment of the area: " << Q << " m^3\n";
	dCrossprops[0] = A; dCrossprops[1] = I; dCrossprops[2] = Q;

    // compute volume of the beam
    float fVolume = A*dLength;
    std::cout << "     Volume of the beam : "  
              << std::setprecision(10) << std::setw(10) << fVolume << " m^3\n";

    // all done
    std::cout << "\nAll done. Program is terminating.\n\n";

    return 0;
}

/*
Review Questions

Related to the given program
----------------------------
(1) Line 15: What does int in int main imply?
(2) Line 17: What is \t?
(3) Line 18: What does \n\n do?
(4) Line 26: What happens if you type the letter a instead of a number?
(5) Line 38: What does the auto keyword imply?
(6) Line 39: What is pow? What does it require for correct compilation and execution?
(7) Line 40: Is the rvalue expression correct if you remove all the parenthesis from the expression?
(8) Line 49: What does setprecision(10) and setw(15) mean?
(9) Did you see any errors or warnings when the program was compiled?

General questions on the given program
--------------------------------------
(1) Is this program robust?
(2) Is double desirable for storing the 3 attributes of the beam?
(3) How would you store the three values in a vector?
(4) Identify all the C++ keywords.

*/