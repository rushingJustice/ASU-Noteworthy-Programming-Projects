/* EXAMPLE 3.1.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

   OBJECTIVES
   (1) Use of if .. else ...
   (2) Use of relational operators
*/

#include <iostream>
using std::cout;
using std::cin;

int main ()
{
	int nAge;                   // to store the age

    cout << "Input the age: ";   
	cin >> nAge;                // get the age

    if (nAge < 0) {
        cout << "Invalid age " << nAge << ".\n";
	}
    else if (nAge <= 10)
	{ 
        cout << "Person " << nAge << " years old is a child.\n";
	}
    else if (nAge <= 18)
	{
        cout << "Person " << nAge  << " years old is a juvenile.\n";
	}
    else if (nAge <= 59)
	{
        cout << "Person " << nAge << " years old is an adult.\n"; 
	}
    else {
        cout << "Person " << nAge << " years old is a senior citizen.\n";
    }

	// all done
	return (0);

}

