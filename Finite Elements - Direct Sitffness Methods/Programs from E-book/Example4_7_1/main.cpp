/* EXAMPLE 4.7.1
   Copyright(c) 2008-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate exception handling in C++

*/
#include <iostream>
#include <string>
#include <cmath>

int main ()
{
    double dUser;   // user input value
    std::string szReadError ("Invalid user input.\n");

    // try-catch block to read and interpret user input
    try
    {
        std::cout << "Input a real positive number: ";
        std::cin >> dUser;

        if (std::cin.fail())
            throw szReadError;
        else if (dUser <= 0.0)
            throw dUser;

        // no errors encountered
        std::cout << " Reciprocal of " << dUser << " is " << 1.0/dUser   << '\n';
        std::cout << "Square root of " << dUser << " is " << sqrt(dUser) << '\n';
    }
    catch (double dX)
    {
        if (dX == 0.0)
            std::cout << "Cannot divide by zero.\n";
        else
            std::cout << "Cannot find square root of a negative number.\n";
    }
    catch (const std::string& szMessage)
    {
        std::cout << szMessage;
    }
    catch (...) // catch all block
    {
        std::cout << "Unknown error.\n";
    }

    return 0;
}