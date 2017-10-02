/*********************************************
Example 13.1.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_CWIZARD_H__
#define __RAJAN_CWIZARD_H__

#include "../Example10_6_1/ISection.h"
#include "../Example10_6_2/XSDatabase.h"

class CWizard
{
	public:
		CWizard ();
		~Wizard ();
		// accessor functions
		int GetUserInput () const;
		void ProcessUserInput () const;
		void DisplayResults () const;

	private:
		CXSDatabase DBISection; 
		CISection ISelectedISection;
};

#endif