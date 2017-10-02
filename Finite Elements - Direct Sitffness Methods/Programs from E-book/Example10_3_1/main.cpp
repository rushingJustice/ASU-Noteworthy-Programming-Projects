/* EXAMPLE 10.3.1
   Copyright(c) 2005-10, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate how to use and test the matrix toolbox

*/
#include <iostream>
#include "MatToolBox.h"

int main ()
{
    CMatToolBox<float> MTB;       // single precision version
    CMatToolBox<double> MTBDP;    // double precision version
    const int ROWS=2, COLUMNS=3;
    CVector<float> fVA(COLUMNS), fVB(COLUMNS), 
                   fVC(COLUMNS);

    for (int i=1; i <= COLUMNS; i++)
    {
        fVA(i) = static_cast<float>(i);
        fVB(i) = static_cast<float>(i*i);
    }

    // vector addition
    std::cout << "\n\nVector Addition .....\n";
    if (MTB.Add (fVA, fVB, fVC))
    {
        MTB.Display ("Vector A", fVA);
        MTB.Display ("Vector B", fVB);
        MTB.Display ("Vector C = A + B", fVC);
    }
    else
        std::cout << "Vector Add error.\n";

    // matrix-matrix multiplication
    std::cout << "\n\nMatrix-Matrix Multiplication .....\n";
    CMatrix<float> fMA(ROWS,COLUMNS), fMB(COLUMNS,ROWS), fMC(ROWS, ROWS);
    for (int i=1; i <= ROWS; i++)
    {
        for (int j=1; j <= COLUMNS; j++)
        {
            fMA(i,j) = static_cast<float>(i+j);
            fMB(j,i) = static_cast<float>(i-j);
        }
    }
    if (MTB.Multiply(fMA, fMB, fMC))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Matrix B", fMB);
        MTB.Display ("Matrix C = A*B", fMC);
    }
    else
        std::cout << "\nMatrix multiplication error.\n";

    // solution to Ax = b
    const int NUMEQNS=3;
    const double TOL = 1.0e-6;
    CMatrix<double> dMA(NUMEQNS,NUMEQNS);
    CVector<double> dVx(NUMEQNS), dVb(NUMEQNS);
    dMA(1,1) = 10.0;   dMA(1,2) = -5.0;    dMA(1,3) = 2.0;
    dMA(2,1) =  3.0;   dMA(2,2) = 20.0;    dMA(2,3) = 5.0;
    dMA(3,1) = -2.0;   dMA(3,2) =  7.0;    dMA(3,3) = 15.0;

    dVb(1) = 6.0;      dVb(2) = 58.0;      dVb(3) = 57.0;
    if (MTBDP.AxEqb (dMA, dVx, dVb, TOL))
    {
        MTBDP.Display ("Vector x in Ax = b", dVx);
    }
    else
        std::cout << "Error in AxEqb.\n";

    double dAS, dM, dD;
    MTBDP.GetFLOPStats (dAS, dM, dD);
    std::cout << "\n\n";
    std::cout << "Double Precision Floating-point Operations\n"
              << "  # of additions/subtractions: " << dAS << '\n'
              << "         # of multiplications: " << dM  << '\n'
              << "               # of divisions: " << dD  << '\n';

    return 0;
}
