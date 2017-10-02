/* EXAMPLE 4.2.2
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of multiple source files.
   (2) Shows how to time a program.

*/
#include <iostream>
#include <cmath>
#include <time.h>       // for time-related functions

// function prototypes
double UserFunction (double dP, double dA, double dH, double dAlpha);
void UpdateError (double dError, double& dSmallestError,
                  double dA, double& dAAnswer,
                  double dAlpha, double& dAlphaAnswer);

int main ()
{
    time_t StartTime, EndTime;
    double dElapsedTime;

    // get the start time for the program
    time (&StartTime);

    // initialize
    double dPerimeter = 5.0;
    double dH = 1.0;
    double dAlpha;                      // unknown
    double dA;                          // unknown
    double dLowerBoundAlpha = 5.0;
    double dUpperBoundAlpha = 85.0;
    double dAlphaIncrement = 0.01;
    double dLowerBoundArea = 0.5;
    double dUpperBoundArea = 2.0;
    double dAreaIncrement = 0.001;
    double dSmallestError = HUGE_VAL;   // smallest error so far
    double dError;                      // current error

    // solution
    double dAlphaAnswer;
    double dAAnswer;

    // search for the solution
    for (dAlpha=dLowerBoundAlpha; dAlpha <= dUpperBoundAlpha;
         dAlpha += dAlphaIncrement)
    {
        for (dA=dLowerBoundArea; dA <= dUpperBoundArea;
             dA += dAreaIncrement)
        {
            dError = fabs(UserFunction (dPerimeter, dA, dH, dAlpha));
            UpdateError (dError, dSmallestError,
                dA, dAAnswer, dAlpha, dAlphaAnswer);
        }
    }

    // display the solution
    std::cout << "The obtained solution is as follows.\n";
    std::cout << "Alpha = " << dAlphaAnswer << " degrees\n";
    std::cout << " Area = " << dAAnswer << " m^2\n";

    // get the end time for the program
    time (&EndTime);

    dElapsedTime = difftime (EndTime, StartTime);
    std::cout << "  Start time in seconds since UTC 1/1/70: " << StartTime << "\n"
              << "    End time in seconds since UTC 1/1/70: " << EndTime   << "\n"
              << "Elapsed time is : " << dElapsedTime << " s.\n";

    return 0;
}

double UserFunction (double dP, double dA, double dH, double dAlpha)
{
    const double ANGLES_TO_RADIANS = 0.0174533;

    dAlpha = ANGLES_TO_RADIANS*dAlpha;
    double dRHS = (dA/dH) - dH/tan(dAlpha) + (2.0*dH/sin(dAlpha));

    return (dP - dRHS);
}

void UpdateError (double dError, double& dSmallestError,
                  double dA, double& dAAnswer,
                  double dAlpha, double& dAlphaAnswer)
{
    if (dError < dSmallestError)
    {
        dAlphaAnswer = dAlpha;
        dAAnswer = dA;
        dSmallestError = dError;
    }
}
