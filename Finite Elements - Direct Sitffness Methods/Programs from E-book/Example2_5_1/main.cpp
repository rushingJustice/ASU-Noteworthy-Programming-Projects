/* EXAMPLE 2.5.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of different data types.
   (2) Illustrate program output formatting.

*/

#include <iostream> // header file 
using std::cout;    // standard output class
using std::endl;    // standard newline character

#include <iomanip>          // header file
using std::setprecision;    // set precision
using std::setw;            // set width

int main()
{

    // illustrate integer arithmetic
	int nA, nB, nC;
    nA = 100000; nB = 200000;
    nC = nA + nB;
    cout << " int: " << nA << " + " << nB << " = " << nC << endl;

    long lA, lB, lC;
    lA = -64000L; lB = -12800L;
    lC = lA * lB;
    cout << "long: " << lA << " * " << lB << " = " << lC << endl;

    // illustrate floating point arithmetic
	cout << setprecision(12);   // 12 decimal places
    float  fX, fY, fZ;
    fX = 1.0f/3.0f; fY = 1.0f/16.0f;
    fZ = fX/fY;
    cout << " float: " << setw(2) << fX << " / " << setw(1)
         << fY << " = " << setw(2) << fZ << endl;

	cout << setprecision(15);   // 15 decimal places
    double dX, dY, dZ;
    dX = 1.0/3.0; dY = 1.0/16.0;
    dZ = dX/dY;
    cout << "double: " << setw(2) << dX << " / " << setw(1)
         << dY << " = " << setw(2) << dZ << endl;
    
	// all done
	return (0);
}
