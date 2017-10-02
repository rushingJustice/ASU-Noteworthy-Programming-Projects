/* (c) 2001-13, S. D. RAJAN
   ASU
   TEMPE, AZ 85287

   PROGRAM TO INTEGRATE NUMERICALLY nD FUNCTIONS
   USING NUMERICAL INTEGRATION APPROACH
*/

#include <iostream>
#include "GaussLegendreTests.h"
#include "GaussLaguerreTests.h"

double MOIC (double x, double y, void* data)
{
	return y*y;
}
    
int main ()
{
    int WHATTORUN = 1;

    if (WHATTORUN == 1)
    {
        CGaussLegendreTests TestGQ;
        TestGQ.RunTests();
    }
    else
    {
        CNIonKnownShapes SMOA;
        int nIntegOrder = 10;        // integration order
        double dRadius = 2.2;        // radius
        double dXC = 0.0, dYC = 0.0; // coordinates of the center
        double dResult;              // final integration value
	    std::cout << "\nSecond moment of area of a circular cross-section of radius " 
                  << dRadius << " using integration order " << nIntegOrder << ": \n";

        // compute and display the result
        if (SMOA.GQSphere (nIntegOrder, MOIC, NULL, dRadius, dXC, dYC, dResult))
            std::cout << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }

	return 0;
}