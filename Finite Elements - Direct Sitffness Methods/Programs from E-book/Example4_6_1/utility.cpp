/* EXAMPLE 4.5.2
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis
*/

#include <iostream>
#include <sstream>              // string stream class
using std::ostringstream;       // for formatted output

#include "utility.h"
#include "..\library\getinteractive.h"	// a more robust way to obtain
										// user input

void ShowBanner ()
{
	std::cout << "\t\tWelcome to a 4-Function Calculator\n";
	std::cout << "\t\t          Version 1.0             \n";
}

void ShowGoodbye ()
{
	std::cout << "\n\tGoodbye .....\n";
}

int UserCommand (double& dNext, const double dMemory)
{
	bool bError = true;         // tracks error in user input
	int nCommand = -1;          // command #
    std::string szCommand;      // stores user input

	do
	{
        ostringstream szPrompt;     // sets up formatted user prompt
		// format the user prompt
        szPrompt << "\nEnter +-*/CS or value [" << dMemory
                  << "] ";

		// get the user command
        GetInteractive (szPrompt.str(), szCommand, 10);

        // check the user command
		if (szCommand == "+")
			nCommand = 1;
		else if (szCommand == "-")
			nCommand = 2;
		else if (szCommand == "*")
			nCommand = 3;
		else if (szCommand == "/")
			nCommand = 4;
		else if (szCommand == "C" || szCommand == "c")
			nCommand = 5;
		else if (szCommand == "S" || szCommand == "s")
			nCommand = 0;
        // is the input a number?
        else
        {
            double dV;
            if (GetDoubleValue (szCommand, dV) == 0) // valid number?
            {
                dNext = dV;
                nCommand = 6;
            }
        }

		// illegal input
        if (nCommand == -1)
            std::cout << "\nInvalid input.";
        else
            bError = false;

	} while (bError == true); // loop until input is legal

	return nCommand;
}

// these functions implement binary + - * /
double Add (const double dN1, const double dN2)
{
	return (dN1 + dN2);
}

double Subtract (const double dN1, const double dN2)
{
	return (dN1 - dN2);
}

double Multiply (const double dN1, const double dN2)
{
	return (dN1 * dN2);
}

double Divide (const double dN1, const double dN2)
{
    // check for divide by zero
    double dResult = (dN2 != 0.0? dN1 / dN2 : 0.0);
    return (dResult);
}
