/* EXAMPLE 3.4.1
   Copyright(c) 2005-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate various output formatting options

*/
#include <iostream>    // also contains ios definitions

int main ()
{
    float fX;       // to store the user input
    int nPrecision; // to store the user-specified precision
    
    for (;;)
    {
        // get the number from the user
        std::cout << "Input a floating point number plus precision: ";
        std::cin >> fX >> nPrecision;

        // display values in default settings
        std::cout << "You have input (default): " << fX << '\n';

        // set output style 1 (precision, not e-format, show decimal
        // point and trailing zeros)
        std::cout.precision (nPrecision);
        std::cout.setf (std::ios::fixed | std::ios::showpoint);
        std::cout << "You have input (format style 1): " << fX << '\n';

        // modify - style 2 (in addition, show plus sign, if applicable)
        std::cout.setf (std::ios::showpos);
        std::cout << "You have input (format style 2): " << fX << '\n';

        // modify - style 3 (invoke scientific format)
        std::cout.unsetf (std::ios::fixed);
        std::cout.setf (std::ios::scientific);
        std::cout << "You have input (format style 3): " << fX << '\n';

        // restore default settings
        std::cout.unsetf (std::ios::showpoint
                          | std::ios::showpos | std::ios::scientific);
        std::cout.setf (0, std::ios::floatfield);
    }

    return 0;
}
