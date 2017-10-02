/* Example 13.4.4
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis

*/
#include <cmath>
#include "NewtonRaphson.h"

CNewtonRaphson::CNewtonRaphson (const int& nMaxIterations, const double& dTolerance)
{
    m_nMAXITERATIONS = nMaxIterations;
    m_dCONVTOL = dTolerance;
    m_TOL = 1.0e-13;
}

CNewtonRaphson::~CNewtonRaphson ()
{
}

bool CNewtonRaphson::Compute (double& dRoot, int& nIterations, const CVector<double>& dVB,
                              const CMatrix<double>& dMA,
                              void(*ptr)(const CVector<double>&, const CMatrix<double>&,
                                         const double& dX, double& dFX, double& dDX))
{
    int i;

    // initialize
    m_dRoot = dRoot;

    for (i=1; i <= m_nMAXITERATIONS; i++)
    {
        // compute f, df at current point
        ptr (dVB, dMA, m_dRoot, m_dFX, m_dDFDX);

        // if derivative is near zero, return with error message
        if (fabs(m_dDFDX) < m_TOL) 
            return false;

        // compute new point
        double m_dNewRoot = m_dRoot - (m_dFX/m_dDFDX);

        // convergence check
        if (fabs(m_dNewRoot - m_dRoot) <= m_dCONVTOL)
        {
            dRoot = m_dNewRoot;
            nIterations = i;
            return true;
        }

        // update estimate of root
        m_dRoot = m_dNewRoot;
    }

    // did not converge
    return false;
}

