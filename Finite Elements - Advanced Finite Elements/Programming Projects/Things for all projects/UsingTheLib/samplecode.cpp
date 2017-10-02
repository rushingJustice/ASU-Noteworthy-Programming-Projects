/*********************************************
UseTheLib Program
Copyright(c) 2006, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "..\library\fileio.h"
#include "..\library\getinteractive.h"
#include "..\library\clock.h"
#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "..\library\textfile.h"
#include "..\library\parser.h"

void PassByRef (CMatrix<double>& dMA,
                CVector<int>& nVA)
// -----------------------------------------------------------
// Function : Shows how to pass CVector and CMatrix objects
//            via reference and change their values
// Input    : matrix dMA and vector nVA
// Output   : selective changes to the matrix and vector
// -----------------------------------------------------------
{
    dMA(1,1) = 0.0f;
    nVA(1) = 1000;
}

void PassByValue (CMatrix<double> dMA,
                  CVector<int> nVA)
// -----------------------------------------------------------
// Function : Illustrates INCORRECT USAGE of 
//            CVector and CMatrix objects as function arguments
//            do NOT pass them as values 
// Input    : matrix dMA and vector nVA
// Output   : selective changes do NOT take place as intended
// -----------------------------------------------------------
{
    dMA(1,1) = 0.0f;
    nVA(1) = 1000;
}

void IllustrateGetInteractive ()
// -----------------------------------------------------------
// Function : shows how to use the overloaded function
//            getinteractive to obtain values from 
//            keyboard input
// Input    : none
// Output   : none
// -----------------------------------------------------------
{
    // -------------------------
    // integer input with prompt
    // -------------------------
    int nChars;
    GetInteractive ("How many characters in your first name? ",
                    nChars);
    std::cout << "You typed : " << nChars << '\n';

    // ----------------------------
    // float input with value check
    // ----------------------------
    const float TMIN = -200.0, TMAX = 140.0;
    float fTemperature;
    GetInteractive ("What is today's temperature (F)? ",
                    fTemperature, TMIN, TMAX);
    std::cout << "You typed : " << fTemperature << '\n';

    // ----------------------------
    // string input
    // ----------------------------
    const int MAXCHARS = 10;
    std::string szName;
    GetInteractive ("What is your name? ", szName, MAXCHARS);
    std::cout << "You typed : " << szName << '\n';

    // -----------------------------------------
    // converting a string into an integer value
    // -----------------------------------------
    std::string szIntValue ("62890");
    int nState, nV;
    if ((nState = GetIntValue (szIntValue, nV)) != 0)
        std::cout << "Invalid integer value.\n";
    else
        std::cout << "Value of string " << szIntValue << " is "
                  << nV << '\n';

    // -----------------------------------------
    // converting a string into a float value
    // -----------------------------------------
    std::string szFloatValue ("-45.0098");
    float fV;
    if ((nState = GetFloatValue (szFloatValue, fV)) != 0)
        std::cout << "Invalid float value.\n";
    else
        std::cout << "Value of string " << szFloatValue << " is "
                  << fV << '\n';
}

void IllustrateFileIO ()
// -----------------------------------------------------------
// Function : shows how to use the CTextFile class to
//            carry out file operations
// Input    : none
// Output   : none
// -----------------------------------------------------------
{
    int   n1 = 100,  n2 = -333;
    float f1 = 3.3f, f2 = -5.09f;

    // Step 1: open a text file for writing
    CTextOFile OFile;
    OFile.OpenOutputFileByName ("Output file name: ", std::ios::out);

    // Step 2: write two lines 
    //         first line contains two integers
    OFile << n1 << " " << n2 << '\n';
    //         second line contains two floats
    OFile << f1 << " " << f2 << '\n';
    // Step 3: close the file
    OFile.close ();

    // Step 4: open a text file for reading
    n1 = n2 = 0;
    f1 = f2 = 0.0f;
    CTextIFile IFile;
    IFile.OpenInputFileByName ("Retype that file name: ", std::ios::in);
    // Step 5: read the two integers
    IFile >> n1 >> n2;
    // Step 6: read the two floats
    IFile >> f1 >> f2;

    // Step 7: rewind the file and read the two lines again
    IFile.Rewind ();
    int   n11, n12;
    float f11, f12;
    IFile >> n11 >> n12;
    IFile >> f11 >> f12;
    std::cout << "Expecting 100 and -333. Found "
              << n11 << " and " << n12 << '\n';
    std::cout << "Expecting 3.3 and -5.09. Found "
              << f11 << " and " << f12 << '\n';
    // Step 8: close the file
    IFile.close ();
}

void IllustrateTiming ()
// -----------------------------------------------------------
// Function : shows how to time a part of a program using
//            wall clock timing
// Input    : none
// Output   : none
// -----------------------------------------------------------
{
    const int ROWS = 30, COLUMNS = 40;
    CMatrix<double> dMA(ROWS, COLUMNS); dMA.Set (0.0);
    CVector<int>    nVA(ROWS);          nVA.Set (0);
    std::string szDateTime;
    int nMenuSelection;

    // ask user which type of function call to make
    GetInteractive ("1 for pass-by-value\n"
                    "2 for pass-by-reference ... ",
                    nMenuSelection, 1, 2);

    // start the clock
    CClock Timer;

    // get the current date and time
    Timer.GetDateTime (szDateTime);
    std::cout << "\nStarting out at : " << szDateTime << "\n";

    const int LOOPS = 1000000; // one million!
    // the functions will be called million times
    if (nMenuSelection == 1)
    {
        for (int i=1; i <= LOOPS; i++)
            PassByValue (dMA, nVA);
    }
    else
    {
        for (int i=1; i <= LOOPS; i++)
            PassByRef (dMA, nVA);
    }

    // get the current date and time
    Timer.GetDateTime (szDateTime);
    std::cout << "\nEnding at : " << szDateTime << "\n";
    // compute the elapsed time
    std::cout << "Elapsed wall clock time: " << Timer.DiffTime ()
              << " seconds\n";
}

void IllustrateSTLUsage ()
// -----------------------------------------------------------
// Function : shows how to use the STL sort function
// Input    : none
// Output   : none
// -----------------------------------------------------------
{
	const int SIZE = 6;
	CVector<double> dVData(SIZE);
	dVData(1) = -1.3;   dVData(2) = 133.4; dVData(3) = -55.3;
	dVData(4) = 1982.4; dVData(5) = 0.0;   dVData(6) = -45.3;
	std::cout << "Before sort ...\n";
	for (int i=1; i <= SIZE; i++)
		std::cout << i << ": " << dVData(i) << '\n';
	std::cout << '\n';
    std::sort(&dVData(1), 
              &dVData(SIZE)+1, std::less<double>());
	std::cout << "After sort in ascending order ...\n";
	for (int i=1; i <= SIZE; i++)
		std::cout << i << ": " << dVData(i) << '\n';
	std::cout << '\n';
}

void IllustrateCParser ()
// -----------------------------------------------------------
// Function : shows how to use the CParser class
// Input    : none
// Output   : none
// -----------------------------------------------------------
{
    CTextIFile IFile;
    IFile.OpenInputFileByName ("Data file name: ", std::ios::in);

    CParser Parser;                        // the parser
    std::vector<std::string> strVTokens;   // stores the tokens read
    std::string strDelimiters (" ,");      // field delimiters
    int nTokens;                           // # of tokens read
    int nLineCounter = 0;                  // line counter

    for (;;)
    {
        ++nLineCounter;
        if (!Parser.GetTokens (IFile, strVTokens, nTokens, strDelimiters))
        {
            std::cout << "Error reading line " << nLineCounter << "\n";
            break;
        }
        else
        {
            if (strVTokens[0].substr(0,1) == "$")
            {
                std::cout << "Line " << nLineCounter << " is a comment line.\n";
                continue;
            }
            else if (strVTokens[0] == "*end")
            {
                std::cout << "All done. Read " << nLineCounter << " lines.\n";
                break;
            }
        }
    }

    IFile.close ();
}