/* Example 13.4.4
   Copyright(c) 2005-12, S. D. Rajan
   Object-Oriented Numerical Analysis

   Newton-Raphson technique to compute a root of a
   polynomial.

*/
#ifndef __RAJAN__NEWTONRAPHSON__H__
#define __RAJAN__NEWTONRAPHSON__H__

#include "../library/matrixtemplate.h"
#include "../library/vectortemplate.h"

class CNewtonRaphson
{
    public:
        CNewtonRaphson (const int& nMaxIterations, const double& dTolerance);
        ~CNewtonRaphson ();

        bool Compute (double& dRoot, int& nIterations, const CVector<double>&,
                      const CMatrix<double>&,
                      void(*ptr)(const CVector<double>&, const CMatrix<double>&,
                                 const double& dX, double& dFX, double& dDX));

    private:
        double  m_TOL;                  // derivative value check
        int     m_nMAXITERATIONS;       // max # of iterations
        double  m_dCONVTOL;             // convergence tolerance
        double  m_dRoot;                // current estimate of root, x
        double  m_dFX;                  // f(x)
        double  m_dDFDX;                // df(x)/dx
};

#endif