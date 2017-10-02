/*********************************************
Example 13.1.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include "Wizard.h"

int main ()
{
    CWizard theWizard;

    while (theWizard.GetUserInput())
    {
        theWizard.ProcessUserInput();
        theWizard.DisplayResults();
    }

    return 0;
}