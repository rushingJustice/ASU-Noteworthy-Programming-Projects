/*********************************************
Example Program 12.3.3
Copyright(c) 2008, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis

Objective: read an input file that contains sales record
for textbooks and compute the total sales for just
engineering textbooks
*********************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "..\library\fileio.h"
#include "..\library\getinteractive.h"
#include "..\library\parser.h"


void ErrorHandler (int nCode)
{
    switch (nCode)
    {
        case 1:
            std::cerr << "Invalid integer input.\n";
        break;

        case 2:
            std::cerr << "Invalid float input.\n";
        break;

        case 3:
            std::cerr << "Invalid double input.\n";
        break;

        default:
            std::cerr << "Unknown error.\n";
        break;
    }
}

int main ()
{
    CParser Parse;
    
    // variables to store tokens
    std::vector<std::string> szVTokens;
    int nTokens;
    std::string szDelimiters (",");

    // open the input file
    std::ifstream IFile;
    OpenInputFileByName ("Sales Report Filename: ", IFile, std::ios::in);

    // prepare to read input file
    int  nTotalLines = 0, nEngineeringBooks = 0;
    bool bStartComputing = false;
    std::string szBookName;
    float fUnitPrice;
    int   nBooksSold;
    float fTotalSales = 0.0f;

    // loop until end-of-file
    for (;;)
    {
        // end-of-file
        if (!Parse.GetTokens (IFile, szVTokens, nTokens, szDelimiters))
            break;
        ++nTotalLines;
        // look for keyword *Engineering on a line
        // next line contains author name, book title, unit price and
        // number sold
        if (szVTokens[0] == "*Engineering")
        {
            bStartComputing = true;
            continue;
        }
        else if (szVTokens[0].substr(0,1) == "*")
            bStartComputing = false;
        if (bStartComputing)
        {
            ++nEngineeringBooks;
            if (GetFloatValue (szVTokens[2], fUnitPrice) != 0)
                ErrorHandler (2);
            if (GetIntValue (szVTokens[3], nBooksSold) != 0)
                ErrorHandler (1);
            fTotalSales += fUnitPrice*static_cast<float>(nBooksSold);
        }
    }

    std::cout << "\n\nShort Report\n------------\n"
              << "File had " << nTotalLines << " lines of input.\n"
              << "Engineering books: " << nEngineeringBooks << '\n'
              << "      Total sales: " << fTotalSales       << "\n\n";

    // close the input file
    IFile.close ();

    return 0;
}