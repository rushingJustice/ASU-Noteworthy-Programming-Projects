/*********************************************
Example 10.6.3
Copyright(c) 2000-08, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
*********************************************/

#ifndef __RAJAN_CWIZARD_H__
#define __RAJAN_CWIZARD_H__

#include "../Example13_1_1/ISection.h"
#include "../Example13_1_2/XSDatabase.h"
#include "XSSelector.h"

class CWizard
{
	public:
		CWizard ();
		~CWizard ();
		// helper functions
		int GetUserInput ();
		void ProcessUserInput ();
		void DisplayResults ();

	private:
		CXSDatabase m_DBISection;
		CISection m_ISection;
		CXSSelector m_SelectLightest;
		float m_fBMMax, m_fTFMax, m_fCFMax;
		int m_nFound;
};

#endif