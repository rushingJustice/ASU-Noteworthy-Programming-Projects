/* Example 6.4.1
   Copyright(c) 2008, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
#include <cmath>
#include "newtonraphson.h"

const double TOL = 1.0e-6; // limit on derivative

bool NewtonRaphson (double& dRoot, const int nMaxIterations, 
                    const double dConvTol,
                    void(*userfunc)(double dX, double& dFX, double& dDX))
{
    int i;
    double dFX, dDX; // function and first derivative at current point

    // loop thro' all iterations
    for (i=1; i <= nMaxIterations; i++)
    {
        // compute f, df at current point
        userfunc (dRoot, dFX, dDX);

        // if derivative is near zero, return with error message
        if (fabs(dDX) < TOL) 
            return false;

        // compute new point
        double dNewRoot = dRoot - (dFX/dDX);

        // convergence check
        if (fabs(dNewRoot - dRoot) <= dConvTol ||
            fabs(dFX) <= dConvTol)
        {
            dRoot = dNewRoot;
            return true;
        }

        // update estimate of root
        dRoot = dNewRoot;
    }

    // did not converge
    return false;
}

