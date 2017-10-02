/* EXAMPLE 9.4.2
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate usage of CMatrix class.
   (2) Show how function calls can be made
   (3) Develop a non-class functions for matrix display and addition

*/

#include <iostream>
#include <string>
#include <iomanip>
#include "../library/matrixtemplate.h"

void Display (const std::string& szHeader, const CMatrix<float>& fM)
{
    std::cout << szHeader << "\n";
    for (int i=1; i <= fM.GetRows(); i++)
    {
        for (int j=1; j <= fM.GetColumns(); j++)
        {
            std::cout << "(" << std::setw(2) << i << "," 
                             << std::setw(2) << j
                      << "): " << fM(i,j) << "\n";
        }
    }
}

int AddMatrices (const CMatrix<float>& fM1, const CMatrix<float>& fM2, 
                 CMatrix<float>& fM3)
{
    if ((fM1.GetRows() != fM2.GetRows()) ||
        (fM1.GetRows() != fM3.GetRows()))
        return 1;
    if ((fM1.GetColumns() != fM2.GetColumns()) ||
        (fM1.GetColumns() != fM3.GetColumns()))
        return 1;

    for (int i=1; i <= fM1.GetRows(); i++)
    {
        for (int j=1; j <= fM1.GetColumns(); j++)
        {
            fM3(i,j) = fM1(i,j) + fM2(i,j);
        }
    }

    return 0;
}

int main ()
{
    CMatrix<float> fMA(3,2), fMB(3,2); // 3 rows and 2 columns
    fMA.Set (0.0f);                // clear the matrix to zero
    fMB.Set (0.0f);                // clear the matrix to zero

    CMatrix<float> fMC(3,2);  // define the matrix to hold the result

    // set the values for matrices A and B
    for (int i=1; i <= 3; i++)
    {
        for (int j=1; j <= 2; j++)
        {
            fMA(i,j) = static_cast<float>(i+j);
            fMB(i,j) = static_cast<float>(i+2*j);
        }
    }

    Display ("Matrix A", fMA);
    Display ("Matrix B", fMB);

    // add the matrices with the result in matrix C
    AddMatrices (fMA, fMB, fMC);
    Display ("Matrix C", fMC);

    // the following statement will lead to 
    // an assertion failure during execution (debug version)
    fMC(4,1) = 10.0f;

    return 0;
}
