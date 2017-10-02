/* EXAMPLE 4.5.2
   CASE STUDY: A 4-function calculator

   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate a simple but modular program.
   (2) Show a simple menuing and I/O system.

*/

#include "utility.h"        // contains function prototypes

int main()
{
	// show program banner
	ShowBanner ();

	// initialize
    double dMemory = 0.0;   // contains current displayed value
    double dNext = 0.0;     // new user input value
    int    nOper = 0;       // operation to be performed

	// loop until user exits the program
	for (;;)
	{
		// get user command
		int nCommand = UserCommand (dNext, dMemory);

		// stop the program?
		if (nCommand == 0)
			break;

		// branch on user command
		switch (nCommand)
		{
			case 1:	// addition
                nOper = 1;
			break;
			case 2: // subtraction
                nOper = 2;
			break;
			case 3: // multiplication
                nOper = 3;
			break;
			case 4: // division
                nOper = 4;
			break;
            case 5: // clear the memory
                dMemory = 0.0;
                nOper = 0;
            break;
            case 6: // a number
                // operation defined in previous input?
                // yes.
                if (nOper == 1)
                    dMemory = Add (dMemory, dNext);
                else if (nOper == 2)
                    dMemory = Subtract (dMemory, dNext);
                else if (nOper == 3)
                    dMemory = Multiply (dMemory, dNext);
                else if (nOper == 4)
                    dMemory = Divide (dMemory, dNext);
                else // no. store the number
                    dMemory = dNext;

                // invalidate current operation in anticipation
                // of a new operation
                if (nOper >= 1 && nOper <= 4)
                    nOper = 0;
		}
	}

	// sign off
    ShowGoodbye ();

    return 0;
}
