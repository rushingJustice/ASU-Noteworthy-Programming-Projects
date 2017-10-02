/*********************************************
Example 13.1.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include "Wizard.h"
#include <string>

#include "../library/simpleio.h"

CWizard::CWizard ()
{
    cout << "\t\t\tI-Section Selection\n";
    cout << "\t\t\t(c)2005, S. D. Rajan\n\n";
}

CWizard::~CWizard ()
{
}

int CWizard::GetUserInput ()
{
    std::string szSelection;

    // initialize
    m_nFound = 0;
    
    // process the request
    std::cout << "\nAny more requests to process (Y or N)? ";
    std::cin >> szSelection;
    if (szSelection == "N")
    {
        // no more input
        return (0);
    }
    else
    {
        std::cout << "Input positive values only.\n";
        GetInteractive ("Bending moment (lb-in): ", m_fBMMax);
        GetInteractive ("Tensile Force (lb): ", m_fTFMax);
        GetInteractive ("Compressive Force (lb): ", m_fCFMax);
        return (1);
    }
}

void CWizard::ProcessUserInput ()
{
    m_nFound = m_SelectLightest.GetXSection (m_DBISection,
                    m_ISection, m_fBMMax, m_fTFMax, m_fCFMax);
}

void CWizard::DisplayResults ()
{
    if (!m_nFound)
        std::cout << std::endl
                  << "No section satisfies the requirements.\n";
    else
    {
        std::cout << std::endl << "Lightest section:";
        m_ISection.Display ();
    }
}

