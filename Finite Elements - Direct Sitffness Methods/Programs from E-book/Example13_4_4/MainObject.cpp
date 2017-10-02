/* Example 13.4.4
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis

*/
#include <iostream>
#include "MainObject.h"
#include "NewtonRaphson.h"

int CMainObject::m_nCount = 0;

CMainObject::CMainObject ()
{
    // the coefs. of the polynomial equation whose roots
    // are being computed, is calculated as A * b
    m_dMA.SetSize (3,3);
    m_dVB.SetSize (3);
    
    // initialize matrix A
    m_dMA(1,1) = 5.0;
    m_dMA(1,2) = -2.0;
    m_dMA(1,3) = 0.0;

    m_dMA(2,1) = 4.0;
    m_dMA(2,2) = 2.0;
    m_dMA(2,3) = 0.0;

    m_dMA(3,1) = -2.0;
    m_dMA(3,2) = 0.0;
    m_dMA(3,3) = -6.0;

    // initialize vector b
    m_dVB(1) = 1.0;
    m_dVB(2) = 2.0;
    m_dVB(3) = 3.0;
}

CMainObject::~CMainObject ()
{
}

void CMainObject::GoCompute ()
{
    const int MAXITERATIONS = 50;
    const double CONVTOL = 1.0e-6;

    double dRoot;           // to store the root
    int    nIterations;     // to obtain # of iterations it took

    std::cout << "Program Newton-Raphson\n"
              << "----------------------\n";
    
    // get initial guess
    std::cout << "Initial guess for root: ";
    std::cin >> dRoot;

    // initialize the Newton-Raphson procedure
    CNewtonRaphson NR (MAXITERATIONS, CONVTOL);

    // now compute the root
    if (!NR.Compute (dRoot, nIterations, m_dVB, m_dMA, UserFunctionEvaluation))
    {
        std::cout << "Convergence failure\n";
    }
    else
        std::cout << "Computed root : " << dRoot << " in " << nIterations
                  << " iterations \n";

}

void CMainObject::UserFunctionEvaluation (const CVector<double>& dVB,
                         const CMatrix<double>& dMA,
                         const double& dX, double& dFX,
                         double& dDX)
//
// the function evaluates the function f(x) value
// and its derivative df(x)/dx at the current point given in dX
//
// this example uses f(x) = x**2 + 8x - 20
//                   df(x)/dx = 2*x + 8
//                   roots are -10, 2
//
{
    int i, j;
    static CVector<double> dCoef(3);
    
    if (m_nCount == 0)
    {
        // this is done only once and stored
        for (i=1; i <= 3; i++)
        {
            double dSum = 0.0;
            for (j=1; j <= 3; j++)
            {
                dSum += dMA(i,j)* dVB(j);
            }
            dCoef(i) = dSum;
        }
    }

    // counter to see how many function evaluations take place
    ++m_nCount;

    // function value
    dFX = 0.0;
    for (i=1; i <= 3; i++)
        dFX += dCoef(i)*pow(dX, double(3-i));

    // gradient value
    dDX = 0.0;
    for (i=1; i <= 2; i++)
        dDX += double(3-i)*dCoef(i)*pow(dX, double(2-i));
}
