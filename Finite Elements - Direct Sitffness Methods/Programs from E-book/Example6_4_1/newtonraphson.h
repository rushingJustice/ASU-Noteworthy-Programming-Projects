/* Example 6.4.1
   Copyright(c) 2008, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
#ifndef __RAJAN__NEWTONRAPHSON_H__
#define __RAJAN__NEWTONRAPHSON_H__

bool NewtonRaphson (double& dRoot, const int nMaxIterations, 
                    const double dConvTol,
                    void(*userfunc)(double dX, double& dFX, double& dDX));

#endif
