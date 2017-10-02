/* EXAMPLE 2.7.3
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Consequences of bad programming habits.
   (2) Debugging

*/

#include <iostream>  // header file 
#include <cmath>     // contains math functions

int main()
{

    double Angle;        // to store the angle
    double SineAngle;    // to store sine of the angle

    // compute sin of an angle
    std::cout << "Input an angle in degrees: ";
    std::cin >> Angle;
	const double ATR = 3.14159/180.0;
    std::cout << "Sine of " << Angle << " degrees is: "
              << sin(Angle*ATR) << '\n';

	// all done
	return (0);
}
