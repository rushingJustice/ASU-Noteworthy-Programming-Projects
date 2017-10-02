/*********************************************
Example 13.2.1
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_CWIZARD_H__
#define __RAJAN_CWIZARD_H__

#include "../Example13_1_1\ISection.h"
#include "../Example13_1_2\XSDatabase.h"

class CWizard
{
    public:
        CWizard ();
        ~Wizard ();
        // accessor functions
        int GetUserInput ();
        void ProcessUserInput ();
        void DisplayResults ();
        // modifier functions

    private:
        CXSDatabase DBISection; 
        CISection ISelectedISection;
};

#endif