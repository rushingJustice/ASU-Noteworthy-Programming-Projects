/* Example 13.4.4
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis

*/
#ifndef __RAJAN__MAINOBJECT__H__
#define __RAJAN__MAINOBJECT__H__

#include "../library/matrixtemplate.h"
#include "../library/vectortemplate.h"

class CMainObject
{
    public:
        CMainObject ();
        ~CMainObject ();

        void GoCompute ();
        static void UserFunctionEvaluation (const CVector<double>& dVB,
                         const CMatrix<double>& dMA,
                         const double& dX, double& dFX,
                         double& dDX);

    private:
        static int m_nCount;
        CMatrix<double> m_dMA;   // matrix A
        CVector<double> m_dVB;   // vector B
};

#endif