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

	//--------- SET ROWS & COLUMNS------------------------------------//
    const int ROWS=3, COLUMNS=2;

	//----------------- VECTOR FUNCTIONS------------------------------//
    CVector<float> fVA(COLUMNS), fVB(COLUMNS), 
                   fVC(COLUMNS), fVx(COLUMNS), fVb(ROWS);

	float fDot,fmax,fmin,f2Norm,fMaxNorm; // change to double if needed

	const float fc = 1.0f; // scale. change to double if needed

    for (int i=1; i <= COLUMNS; i++) // generate some values for A and B Vectors (for testing...)
    {
        fVA(i) = static_cast<float>(i*-1);
        fVB(i) = static_cast<float>(i*i);
		fVx(i)= static_cast<float>(i);
    }

	//fVA(1) = 0; fVA(2) = 0;

    // vector addition
    std::cout << "\n\nVector Addition .....\n";
    if (MTB.Add (fVA, fVB, fVC)) // if this is true, do this...
    {
        MTB.Display ("Vector A", fVA);
        MTB.Display ("Vector B", fVB);
        MTB.Display ("Vector C = A + B", fVC);
    }
    else // if return false, do this!
        std::cout << "Vector Add error.\n";

	// vector subtraction
    std::cout << "\n\nVector Subtraction .....\n";
    if (MTB.Subtract (fVA, fVB, fVC)) // if this is true, do this...
    {
        MTB.Display ("Vector A", fVA);
        MTB.Display ("Vector B", fVB);
        MTB.Display ("Vector C = A + B", fVC);
    }
    else // if return false, do this!
        std::cout << "Vector Subtract error.\n";

	// vector dot product
    std::cout << "\n\nVector Dot Product .....\n";
    if (MTB.DotProduct (fVA, fVB, fDot)) // if this is true, do this...
    {
        MTB.Display ("Vector A", fVA);
        MTB.Display ("Vector B", fVB);
		std::cout << "\nDot Product of A and B = " << std::setw(FW) << fDot;
    }
    else // if return false, do this!
        std::cout << "Vector Dot Product error.\n";

	// vector normalize
	std::cout << "\n\nVector Normalize .....\n";
	if (MTB.Normalize (fVA)) // if this is true, do this...
    {
        MTB.Display ("Normalized Vector A = ", fVA);
    }
    else // if return false, do this!
        std::cout << "Vector Normalize error.\n";

	// vector scale
    std::cout << "\n\nVector Scale .....\n";
    if (MTB.Scale (fVA, fc)) // if this is true, do this...
    {
        MTB.Display ("Vector A", fVA);
		std::cout << "\nScale Factor = " << std::setw(FW) << fc;
    }
    else // if return false, do this!
        std::cout << "Vector Scale error.\n";

	// vector max value
    std::cout << "\n\nVector Max .....\n";
    fmax = MTB.MaxValue (fVA); // l-value is max
    MTB.Display ("Vector A", fVA);
	std::cout << "\nMax Value of Vector A is = " << std::setw(FW) << fmax;

	// vector min value
    std::cout << "\n\nVector Min .....\n";
    fmin = MTB.MinValue (fVA); // l-value is min
    MTB.Display ("Vector A", fVA);
	std::cout << "\nMin Value of Vector A is = " << std::setw(FW) << fmin;

	// vector TwoNorm value
    std::cout << "\n\nVector TwoNorm Value .....\n";
    f2Norm = MTB.TwoNorm (fVA); // l-value is two-norm
    MTB.Display ("Vector A", fVA);
	std::cout << "\nTwoNorm of Vector A is = " << std::setw(FW) << f2Norm;

	// vector MaxNorm value
    std::cout << "\n\nVector MaxNorm Value .....\n";
    fMaxNorm = MTB.MaxNorm (fVA); // l-value is max norm 
    MTB.Display ("Vector A", fVA);
	std::cout << "\nMaxNorm of Vector A is = " << std::setw(FW) << fMaxNorm;

	// vector cross-product
    std::cout << "\n\nVector CrossProduct .....\n";
    if (MTB.CrossProduct (fVA, fVB, fVC)) // if this is true, do this...
    {
        MTB.Display ("Vector A", fVA);
		MTB.Display ("Vector B", fVB);
		MTB.Display ("Cross Product of A X B ", fVC);
    }
    else // if return false, do this!
        std::cout << "Vector Cross Product error.\n";


	// --------------------MATRIX FUNCTIONS----------------------------//
    CMatrix<float> fMA(ROWS,COLUMNS), fMB(COLUMNS,ROWS), fMC(COLUMNS, ROWS);
    for (int i=1; i <= ROWS; i++)
    {
        for (int j=1; j <= COLUMNS; j++) // generate Matrix A and B
        {
            fMA(i,j) = static_cast<float>(i+j); 
            fMB(j,i) = static_cast<float>(i-j);
        }
    }

	MTB.Display ("Matrix A", fMA);
	MTB.Display ("Matrix B", fMB);

	// matrix addition
    std::cout << "\n\nMatrix Addition .....\n";
    if (MTB.Add(fMA, fMB, fMC))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Matrix B", fMB);
        MTB.Display ("Matrix C = A + B", fMC);
    }
    else
        std::cout << "\nMatrix Addition error.\n";

	// matrix subtraction
    std::cout << "\n\nMatrix Subtraction .....\n";
    if (MTB.Subtract(fMA, fMB, fMC))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Matrix B", fMB);
        MTB.Display ("Matrix C = A - B", fMC);
    }
    else
        std::cout << "\nMatrix Subtraction error.\n";

	// matrix multiplication
    std::cout << "\n\nMatrix Multiplication .....\n";
    if (MTB.Multiply(fMA, fMB, fMC))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Matrix B", fMB);
        MTB.Display ("Matrix C = A*B", fMC);
    }
    else
        std::cout << "\nMatrix multiplication error.\n";

	// matrix scale
    std::cout << "\n\nMatrix Scale .....\n";
    MTB.Scale(fMA, fc);
    MTB.Display ("Matrix A", fMA);
    std::cout << "\nScale Factor = " << std::setw(FW) << fc;

	// matrix maxnorm (largest absolute element in A)
    std::cout << "\n\nMatrix MaxNorm .....\n";
    fMaxNorm = MTB.MaxNorm(fMA);
    MTB.Display ("Matrix A", fMA);
    std::cout << "\nMaxNorm of Matrix A = " << std::setw(FW) << fMaxNorm;

    // matrix transpose
    std::cout << "\n\nMatrix Transpose .....\n";
    if (MTB.Transpose(fMA, fMB))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Matrix B = A_T", fMB);
    }
    else
        std::cout << "\nMatrix Transpose error.\n";

	// matrix vector multiply 
    std::cout << "\n\nMatrix Vector Multiply .....\n";
    if (MTB.MatMultVec(fMA, fVx, fVb))
    {
        MTB.Display ("Matrix A", fMA);
        MTB.Display ("Vector x", fVx);
		MTB.Display ("Vector b = A*x", fVb);
    }
    else
        std::cout << "\nMatrix Vector Multiply error.\n";

	// ------DEFINE MATRIX A FOR SOLVER FUNCTIONS----------//
    const int NUMEQNS=5;
    const double TOL = 1.0e-6; // tolerance for positive definite check
    CMatrix<double> dMA(NUMEQNS,NUMEQNS); // initialize matrix A
    CVector<double> dVx(NUMEQNS), dVb(NUMEQNS), dVc(NUMEQNS), dVr(NUMEQNS); // initialize vector x, b (and c, r for residual)
	// -----------------------MATRIX VALUES FOR GAUSSIAN------------------------------------------//
    dMA(1,1) = 3.5120;  dMA(1,2) = 0.7679; dMA(1,3) = 0.0;     dMA(1,4) =  0.0;    dMA(1,5) = 0.0;
    dMA(2,1) = 0.7679;  dMA(2,2) = 3.1520; dMA(2,3) = 0.0;     dMA(2,4) = -2.0;    dMA(2,5) = 0.0;
    dMA(3,1) = 0.0;     dMA(3,2) =  0.0;   dMA(3,3) = 3.5120;  dMA(3,4) = -0.7679; dMA(3,5) = 0.7679;
	dMA(4,1) = 0.0;     dMA(4,2) = -2.0;   dMA(4,3) = -0.7679; dMA(4,4) = 3.1520;  dMA(4,5) = -1.1520;
	dMA(5,1) = 0.0;     dMA(5,2) =  0.0;   dMA(5,3) = 0.7679;  dMA(5,4) = -1.1520; dMA(5,5) = 3.1520;

    dVb(1) = 0.0;      dVb(2) = 0.0;      dVb(3) = 0.0;
	dVb(4) = -0.04;     dVb(5) = 0.0;

	// solution to Ax = b (Gaussian Elimination)
    if (MTBDP.AxEqb (dMA, dVx, dVb, TOL))
    {
		// display solution
        MTBDP.Display ("Vector x in Ax = b", dVx);
		// display residual
		MTBDP.MatMultVec (dMA, dVx, dVc);
		MTBDP.Subtract (dVc, dVb, dVr);
		f2Norm = MTBDP.TwoNorm (dVr);
		std::cout << "\nResidual of Ax - b = " << f2Norm;
    }
    else
        std::cout << "Error in AxEqb.\n";

	// -----------------------MATRIX VALUES FOR LDL(T)------------------------------------------//
	dMA(1,1) = 3.5120;  dMA(1,2) = 0.7679; dMA(1,3) = 0.0;     dMA(1,4) =  0.0;    dMA(1,5) = 0.0;
    dMA(2,1) = 0.7679;  dMA(2,2) = 3.1520; dMA(2,3) = 0.0;     dMA(2,4) = -2.0;    dMA(2,5) = 0.0;
    dMA(3,1) = 0.0;     dMA(3,2) =  0.0;   dMA(3,3) = 3.5120;  dMA(3,4) = -0.7679; dMA(3,5) = 0.7679;
	dMA(4,1) = 0.0;     dMA(4,2) = -2.0;   dMA(4,3) = -0.7679; dMA(4,4) = 3.1520;  dMA(4,5) = -1.1520;
	dMA(5,1) = 0.0;     dMA(5,2) =  0.0;   dMA(5,3) = 0.7679;  dMA(5,4) = -1.1520; dMA(5,5) = 3.1520;

    dVb(1) = 0.0;      dVb(2) = 0.0;      dVb(3) = 0.0;
	dVb(4) = -0.04;     dVb(5) = 0.0;

	//LDLT Factorization ADD COUNTS
    std::cout << "\n\nLDLT Factorization of Matrix A .....\n";
	if (MTBDP.LDLTFactorization (dMA, TOL))
    {
        MTBDP.Display ("Decomposed Matrix A ", dMA);
    }
    else
        std::cout << "Error in LDLT Fact.\n";

	//LDLT Solve ADD COUNTS
    std::cout << "\n\nLDLT Solve of Ax = b .....\n";
	if (MTBDP.LDLTSolve (dMA, dVx, dVb))
    {
		// display solution
        MTBDP.Display ("Vector x in Ax = b ", dVx);
		// display residual
		MTBDP.MatMultVec (dMA, dVx, dVc);
		MTBDP.Subtract (dVc, dVb, dVr);
		f2Norm = MTBDP.TwoNorm (dVr);
		std::cout << "\nResidual of Ax - b = " << f2Norm;
    }
    else
        std::cout << "Error in LDLT Solve.\n";

	double dAS, dM, dD; // define double values for operation count
    MTB.GetFLOPStats (dAS, dM, dD);
    std::cout << "\n\n"; // begin operations display
    std::cout << "Single Precision Floating-point Operations\n"
              << "  # of additions/subtractions: " << dAS << '\n'
              << "         # of multiplications: " << dM  << '\n'
              << "               # of divisions: " << dD  << '\n';

    MTBDP.GetFLOPStats (dAS, dM, dD);
    std::cout << "\n\n"; // begin operations display
    std::cout << "Double Precision Floating-point Operations\n"
              << "  # of additions/subtractions: " << dAS << '\n'
              << "         # of multiplications: " << dM  << '\n'
              << "               # of divisions: " << dD  << '\n';

    return 0; // end main
}
