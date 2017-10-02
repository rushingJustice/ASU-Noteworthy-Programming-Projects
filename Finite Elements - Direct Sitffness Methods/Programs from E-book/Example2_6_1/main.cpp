/* EXAMPLE 2.6.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of vector data types.
   (2) Illustrate simple input and output.

*/

#include <iostream> // header file 
using std::cin;     // standard input class
using std::cout;    // standard output class
using std::endl;    // standard newline character

int main()
{

    const int MAXINPUT = 20;     // maximum number of input values
    int nVNumbers[MAXINPUT];    // vector of numbers
    int nSum;                   // to store the sum of all numbers
    
    // get the user input
    cout << " Input the first number: "; cin >> nVNumbers[0];
    cout << "Input the second number: "; cin >> nVNumbers[1];
    cout << " Input the third number: "; cin >> nVNumbers[19];

    // compute the sum
    nSum = nVNumbers[0] + nVNumbers[1] + 2.0*nVNumbers[19]; 

    // display the output
    cout << "The sum of " << nVNumbers[0] << endl
         << "           " << nVNumbers[1] << endl 
         << "           " << nVNumbers[19] << endl 
         << "       is: " << nSum << endl;

	// all done
	return (0);
}
