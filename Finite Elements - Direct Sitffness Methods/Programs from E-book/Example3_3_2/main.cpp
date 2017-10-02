/* EXAMPLE 3.3.2
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate the use of different control statements.
   (2) Show healthy programming styles.

*/

#include <iostream>
#include <iomanip>
#include <ios>
#include <cmath>
#include <string>

int main ()
{
    const int MAXVALUES = 11;   // maximum # of storage locations
                                // for load and deflection values
    double dVLoadData[MAXVALUES];
    double dVDeflectionData[MAXVALUES];
    double dVSlopes[MAXVALUES];
    std::string szVPrompts[] = {"first", "second", "third",   "fourth",
                                "fifth", "sixth",  "seventh", "eighth",
                                "ninth", "tenth"};

    // obtain user input
    double dLoad;
    double dDeflection;

    // initialize
    int nPoint=0;                   // number of data points
    dVLoadData[nPoint] = 0.0;       // first point at (0,0)
    dVDeflectionData[nPoint++] = 0.0;

    // loop until no more input
    for (;;)
    {
        std::cout << "Input " << szVPrompts[nPoint-1] <<
                     " load and deflection values (0 load to end): ";
        std::cin >> dLoad >> dDeflection;

        // zero load signifies end of input
        if (dLoad == 0.0)
            break;
        // load or deflection cannot be negative or zero
        else if (dLoad < 0.0 || dDeflection <= 0.0)
            std::cout << "Load and deflection values must be positive.\n";
        // ordered input?
        else if (dLoad <= dVLoadData[nPoint-1])
            std::cout << "Load values must be in ascending order.\n";
        else
        {
            // save the values and update counter
            dVLoadData[nPoint] = dLoad;
            dVDeflectionData[nPoint++] = dDeflection;
            
            // no more storage space?
            if (nPoint == (MAXVALUES-1))
                break;
        }
    }

    // compute slope of each segment
    int i;                          // loop index
    int nSegments = nPoint-1;
    for (i=0; i < nSegments; i++) 
    {
        dVSlopes[i] = (dVLoadData[i+1] - dVLoadData[i]) /
                      (dVDeflectionData[i+1] - dVDeflectionData[i]);
    }

    // find first load level at which response is nonlinear
    double dTolerance = 1.0e-3;     // tolerance
    double dNonlinLoadValue = 0.0;  // (nonlinear) load level
    for (i=0; i < nSegments-1; i++) 
    {
        if (fabs(dVSlopes[i+1] - dVSlopes[i]) > dTolerance)
        {
            dNonlinLoadValue = dVLoadData[i+1];
            break;
        }
    }
    
    // output the results
    std::cout << std::endl <<"    FINAL REPORT";
    std::cout << std::endl <<"    ------------" << std::endl;
    std::cout << "Load   " << "  " << "Deflection" << "  "
              << "Slope\n";
    std::cout << "----   " << "  " << "----------" << "  "
              << "-----\n";
    std::cout << std::setiosflags(std::ios::left) 
              << std::setw(7)  << dVLoadData[0] << "  "
              << std::setw(10) << dVDeflectionData[0]
              << std::endl;

    for (i=1; i < nPoint; i++)
    {
        std::cout << std::setiosflags(std::ios::left) 
                  << std::setw(7)  << dVLoadData[i]       << "  "
                  << std::setw(10) << dVDeflectionData[i] << "  "
                  << std::setw(10) << dVSlopes[i-1] << std::endl;
    }

    if (dNonlinLoadValue == 0.0)
        std::cout << "\nResponse is entirely linear.\n";
    else
        std::cout << "\nResponse becomes nonlinear at " << 
             dNonlinLoadValue << std::endl;

    return 0;
}