/*********************************************
Example 16.8.1
Object-Oriented Numerical Analysis
Copyright(c) 2000-08, S. D. Rajan
All rights reserved

  OBJECTIVES
   (1) Show creation of a template matrix library
   (2) Illustrate how to use the matrix library
       to solve eigenproblems.

*********************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "..\library\matlib.h"
#include "..\library\fileio.h"

void ErrorHandler (int nCode)
{
    switch (nCode)
    {
        case 1:
            std::cout << "Invalid # of command line arguments.\n";
            break;
        case 2:
            std::cout << "Cannot open input file.\n";
            break;
        case 3:
            std::cout << "Cannot open output file.\n";
            break;
        case 4:
            std::cout << "Error reading from input file.\n";
            break;
        default:
            std::cout << "Unknown error.\n";
            break;
    }

    exit (1);
}

int main (int argc, char* argv[])
{
    enum Methods {INVITERATION, JACOBI, GENERALIZEDJACOBI, 
                  SUBSPACEITERATION, LANCZOS};

    // set method to be used
    Methods Method = INVITERATION;

    // input and output files
    std::ifstream m_FileInput;
    std::ofstream m_FileOutput;

    std::cout << "Generalized Eigenproblem Solver\n"
              << "-------------------------------\n\n";

    if (argc <= 1)
    {
        // open the input file
        OpenInputFileByName ("Complete input file name: ", 
                             m_FileInput, std::ios::in);

        // open the output file
        OpenOutputFileByName ("Complete output file name: ", 
                              m_FileOutput, std::ios::out);
    }
    // user has specified command line arguments
    else if (argc == 3)
    {
        m_FileInput.open (argv[1], std::ios::in); 
        if (!m_FileInput)
            ErrorHandler (2);
        m_FileOutput.open (argv[2], std::ios::out); 
        if (!m_FileOutput)
            ErrorHandler (3);
        std::cout << "\nReading input data from " << argv[1] << '\n';
        std::cout << "\nCreating results in   " << argv[2] << '\n';
    }
    else
    {
        ErrorHandler (1);
    }

    // read header line
    std::string szHeader;
    m_FileInput >> szHeader;

    // define stiffness and mass matrices to be used
    CMatrix<double> K, M;

    // read stiffness matrix
    m_FileInput >> szHeader;
    if (K.Read (m_FileInput) != 0)
        ErrorHandler (4);
    
    // read mass matrix
    m_FileInput >> szHeader;
    if (M.Read (m_FileInput) != 0)
        ErrorHandler (4);

    if (Method == INVITERATION)
    {
        // read initial guess for eigenvectors
        CMatrix<double> u;
        m_FileInput >> szHeader;
        if (u.Read (m_FileInput) != 0)
            ErrorHandler (4);
    
        // use inverse iteration method
        double dLambda, DTOL = 1.0e-5;
        int IterationMax = 100;
        InverseIteration (K, M, dLambda, u, IterationMax, DTOL,
                          true, m_FileOutput);
        m_FileOutput << "\n\nInverse Iteration RESULTS ================\n";
        m_FileOutput << "Eigenvalue: " << dLambda;
        PrintMatrixColumnWise (u, "Eigenvector: ", m_FileOutput);
    }
    else if (Method == GENERALIZEDJACOBI)
    {
        // use generalized jacobi method
        double DTOL = 1.0e-5;
        int IterationMax = 50;
        const int SIZE = K.GetRows();
        CMatrix<double> X(SIZE,SIZE);
        CVector<double> EIGV(SIZE);
        int nError = GeneralizedJacobi (K, M, X, EIGV, IterationMax, DTOL,
                                        true, m_FileOutput);
        if (nError != 0)
            m_FileOutput << "ERROR. UNABLE TO CONVERGE.\n";
        m_FileOutput << "\n\nGeneralized Jacobi RESULTS =================\n";
        for (int i=1; i <= SIZE; i++)
        {
            m_FileOutput << "Eigenvalue: " << EIGV(i);
            PrintMatrixColumn (X, "Eigenvector: ", i, m_FileOutput);
            m_FileOutput << '\n';
        }
    }
    else if (Method == SUBSPACEITERATION)
    {
        std::cout << "Subspace Iteration: Unimplemented technique.\n";
    }
    else if (Method == LANCZOS)
    {
        std::cout << "Lanczos Method: Unimplemented technique.\n";
    }

    // close input and output files
    m_FileInput.close();
    m_FileOutput.close();

    std::cout << "All done ....\n";

    return 0;
}