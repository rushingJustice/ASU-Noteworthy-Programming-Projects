/* EXAMPLE 3.4.2
   Copyright(c) 2005-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate various output formatting options

*/
#include <iostream>    // also contains ios definitions
#include <iomanip>     // to define setw manipulator

int main ()
{
    const int MAXVALUES = 10;
    float fVX[MAXVALUES];       // to store the user input
    int nUserValues;            // # of user-defined values
    int i;                      // loop index
    
    // how many values to store and display?
    do
    {
        std::cout << "How many values are there? ";
        std::cin >> nUserValues;
    } while (nUserValues <= 0 || nUserValues > MAXVALUES);

    // get the values from the user
    for (i=0; i < nUserValues; i++)
    {
        std::cout << "Input value " << i+1 << ": ";
        std::cin >> fVX[i];
    }

    // set output style
    std::cout.setf (std::ios::scientific);
    // table column headings
    std::cout << "   " << "Index" << "       " << "Value" << '\n';
    std::cout << "   " << "-----" << "       " << "-----" << '\n';
    for (i=0; i < nUserValues; i++)
    {
        std::cout << "   "  << std::setw(5)  << i+1;
        std::cout.setf (std::ios::showpos);
        std::cout << "    " << std::setw(10) << fVX[i] << '\n';
        std::cout.unsetf (std::ios::showpos);
    }

    return 0;
}
