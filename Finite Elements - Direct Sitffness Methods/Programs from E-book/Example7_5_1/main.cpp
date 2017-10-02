/* EXAMPLE 7.5.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of standard string class.

*/
#include <iostream>
#include <string>
#include <math.h> // for atof function
#include "..\library\getinteractive.h"

int main ()
{
    std::string szUserInput;
    std::string szOpers ("+-*/");
    bool bError;

    // loop until user input is stop
    for (;;)
    {
        // initialize
        bError = false;
        
        // get user input
		GetInteractive ("Input expression to evaluate: ", szUserInput, 50);

        // terminate the program if input is "stop"
        if (szUserInput == "stop")
            break;
        else
        {
            // find string length
            int nLength = szUserInput.length();

            // find the operator
            int nPos = szUserInput.find_first_of (szOpers, 0); 
            if (nPos > 0)
            {
                // obtain the first number
                std::string szLeftNumber = szUserInput.substr (0, nPos);
                double dLeftNumber = atof(szLeftNumber.c_str());

                // operator
                char szOperator = szUserInput[nPos];

                // obtain the second number
                std::string szRightNumber = szUserInput.substr (nPos+1, nLength);
                double dRightNumber = atof(szRightNumber.c_str());

                double dResult;

                switch (szOperator)
                {
                    case '+': dResult = dLeftNumber + dRightNumber;
                        break;
                    case '-': dResult = dLeftNumber - dRightNumber;
                        break;
                    case '*': dResult = dLeftNumber * dRightNumber;
                        break;
                    case '/': dResult = dLeftNumber / dRightNumber;
                        break;
                }

                // display the result
                std::cout << szUserInput << " = " << dResult << "\n";
            }
            else
                std::cout << "Unable to find operator.\n";
        }
    }

    return 0;
}
