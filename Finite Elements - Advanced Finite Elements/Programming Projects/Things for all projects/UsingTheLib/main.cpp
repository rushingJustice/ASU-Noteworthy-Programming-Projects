/*********************************************
UseTheLib Program
Copyright(c) 2006-13, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <iostream>
#include "..\library\arraybase.h"
#include "..\library\getinteractive.h"

// function prototypes
void IllustrateGetInteractive ();
void IllustrateFileIO ();
void IllustrateTiming ();
void IllustrateSTLUsage ();
void IllustrateCParser ();

int main (int argc, char **argv)
{
    int nMenuSelection;

    // present user with menu selection
    for (;;)
    {
        GetInteractive ("\n"
                        "----------------------------------\n"
                        "    program UsingTheLib           \n"
                        "----------------------------------\n"
                        "Type 1 for command line arguments example\n"
                        "Type 2 for getinteractive examples\n"
                        "Type 3 for file I/O examples\n"
                        "Type 4 for matrix and timing examples\n"
                        "Type 5 for STL example\n"
                        "Type 6 for parser example\n"
                        "Type 0 to exit the program .................... ",
                        nMenuSelection, 0, 6);
        std::cout << "\n\n";

        if (nMenuSelection == 0)
            break;
        else if (nMenuSelection == 1)
        {
            // ---------------------------------------------------
            // Command line arguments
            // ---------------------------------------------------
            // first argument (#0) is always the program name (executable file name).
            std::cout << "Executing program " << argv[0] << "\n";
            // check OTHER command line arguments
            for (int i=1; i < argc; i++)
            {
                std::cout << "Argument " << i+1 << " is: " 
                          << argv[i] << '\n';
            }
        }
        else if (nMenuSelection == 2)
        {
            // ---------------------------------------------------
            // GetInteractive: obtaining values via keyboard input
            // ---------------------------------------------------
            IllustrateGetInteractive ();
        }
        else if (nMenuSelection == 3)
        {
            // ---------------------------------------------------------
            // OpenOutputFileByName: opening a file for output and input
            // ---------------------------------------------------------
            IllustrateFileIO ();
        }
        else if (nMenuSelection == 4)
        {
            // ---------------------------------------------------
            // Using CVector, CMatrix and CTimer classes
            // ---------------------------------------------------
            IllustrateTiming ();
            CArrayBase::ShowStatistics ();
        }
        else if (nMenuSelection == 5)
        {
            // ---------------------------------------------------
            // Using CVector with STL sort function
            // ---------------------------------------------------
            IllustrateSTLUsage ();
            CArrayBase::ShowStatistics ();
        }
        else if (nMenuSelection == 6)
        {
            // ---------------------------------------------------
            // Using CParser to read the input file
            // ---------------------------------------------------
            IllustrateCParser ();
        }
    }

    return 0;
}
