/* EXAMPLE 3.2.2
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

   OBJECTIVES
   ----------
   Illustrate use of do .. while statement

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

    do 
    {
        nSum = nSum + i;
        i = i+1;
    } while (i <= nN);

    cout << "The sum of first " << nN << " integers is "
         << nSum << "." << endl;
    
    return (0);
}