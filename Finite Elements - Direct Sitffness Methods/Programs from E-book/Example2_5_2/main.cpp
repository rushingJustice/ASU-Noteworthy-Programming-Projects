/* EXAMPLE 2.5.2
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of math functions.
   (2) Illustrate simple input and output.

*/

#include <iostream> // header file 
using std::cin;     // standard input class
using std::cout;    // standard output class
using std::endl;    // standard newline character

#include <cmath>    // contains math function declarations

int main()
{

    // conversion values
    const double PI = 3.1415926;
	double ANGLETORADIANS = PI/180.0; // Used in Example 2.5.2
    double angle;   // to store the angle (Ex. 2.5.2)
    
    // compute sin of an angle (Ex. 2.5.2)
    cout << "How much do you love your tiny?: ";
    cin >> angle;

    cout << "Well, I love my tiny infinity times " << angle << ", which is infinity times more than you! ;D"
         << sin(angle*ANGLETORADIANS) << endl;

	// all done
	return (0);
}
