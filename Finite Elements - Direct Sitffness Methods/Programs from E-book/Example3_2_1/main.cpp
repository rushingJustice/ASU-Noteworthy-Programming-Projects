/* EXAMPLE 3.2.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

   OBJECTIVES
   ----------
   Illustrate use of while statement

*/

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main ()
{
    int i=1;    // loop counter
    int nN;     // to store N
    int nSum=0; // to store the sum

    cout << "What is N? ";
    cin >> nN;

    while (i <= nN)
    {
        nSum = nSum + i;
        i = i+1;
    }

    cout << "The sum of first " << nN << " integers is "
         << nSum << "." << endl;
    
    return (0);
}