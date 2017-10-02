/* EXAMPLE 3.2.3
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

   OBJECTIVES
   ----------
   Illustrate use of for statement

*/

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main ()
{
    int i;      // loop counter
    int nN;     // to store N
    int nSum;   // to store the sum

    cout << "What is N? ";
    cin >> nN;

    nSum = 0; // initialize
    for (i=1; i <= nN; i++)
    {
        nSum = nSum + i;
    }

    cout << "The sum of first " << nN << " integers is "
         << nSum << "." << endl;
    
    return (0);
}