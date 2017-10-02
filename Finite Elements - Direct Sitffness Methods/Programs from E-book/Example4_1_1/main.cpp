/* EXAMPLE 4.1.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate the use of a function in a program.

*/
#include <iostream>
bool IsEven (int);	// function prototype

int main ()
{
	int nNumber;    // to store user input

    std::cout << "Type an integer value: ";
    std::cin >> nNumber;

    if (IsEven(nNumber))
        std::cout << nNumber << " is an even number.\n";
    else
        std::cout << nNumber << " is an odd number.\n";

    return 0;
}

bool IsEven (int n)	// determines if n is even or not
{
	if ((n % 2) == 0)   // compute remainder
		return true;    // even number
	else
		return false;   // odd number
}
