/* EXAMPLE 10.3.1
   Copyright(c) 2005-10, S. D. Rajan
   Object-Oriented Numerical Analysis

  NOTES
   (1) Illustrates the development of a matrix toolbox
       based on the CVector and CMatrix classes
       developed earlier.
   (2) return value of most functions is either true or false.
       a true value signifies no error
       a false value indicates an input data error

*/
#ifndef __RAJAN_MATTOOLBOX_H__
#define __RAJAN_MATTOOLBOX_H__

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <string>
#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"

const int ELEMENTSPERLINE = 4;  // # of vector/matrix elements per line
const int FW = 16;              // field width

template <class T>
class CMatToolBox
{
    public:
        CMatToolBox ();
        ~CMatToolBox ();

        // vector-related functions
        void Display (const std::string& szMessage,
                      const CVector<T>& A) const;
        bool Add (const CVector<T>& A, const CVector<T>& B, 
                  CVector<T>& C);
        bool Subtract (const CVector<T>& A, const CVector<T>& B, 
                       CVector<T>& C);
        bool DotProduct (const CVector<T>& A,
                         const CVector<T>& B, T& product);
        bool Normalize (CVector<T>& A);
        bool Scale (CVector<T>& A, T factor);
        T    MaxValue (const CVector<T>& A);
        T    MinValue (const CVector<T>& A);
        T    TwoNorm (const CVector<T>& A);
        T    MaxNorm (const CVector<T>& A);
        bool CrossProduct (const CVector<T>& A,
                           const CVector<T>& B, CVector<T>& C);

        // matrix-related functions
        void Display (const std::string& szMessage,
                      const CMatrix<T>& A) const;
        bool Add (const CMatrix<T>& A, const CMatrix<T>& B, 
                  CMatrix<T>& C);
        bool Subtract (const CMatrix<T>& A,
                       const CMatrix<T>& B, CMatrix<T>& C);
        bool Multiply (const CMatrix<T>& A,
                       const CMatrix<T>& B, CMatrix<T>& C);
        bool Determinant (const CMatrix<T>& A, T& det);
        void Scale (CMatrix<T>& A, T scale);
        T    MaxNorm (const CMatrix<T>& A);
        bool Transpose (const CMatrix<T>& A,
                        CMatrix<T>& B);
        bool Inverse (const CMatrix<T>& A,
                      CMatrix<T>& B, T TOL);
        bool MatMultVec (const CMatrix<T>& A,
                         const CVector<T>& x,
                         CVector<T>& b);
        bool LUFactorization (CMatrix<T>& A, T TOL);
        bool LUSolve (const CMatrix<T>& A,
                      CVector<T>& x, const CVector<T>& b);
        bool AxEqb (CMatrix<T>& A,
                    CVector<T>& x,
                    CVector<T>& b,
                    T TOL);
        bool LDLTFactorization (CMatrix<T>& A, T TOL);
        bool LDLTSolve (const CMatrix<T>& A, CVector<T>& x,
                        const CVector<T>& b);

        // helper function
        void GetFLOPStats (double& dAS, double& dM, double& dD) const;

    private:
        // these are declared double to avoid integer overflow
        double m_dASOP; // # of floating point additions and subtractions
        double m_dMOP;  // # of floating point multiplications
        double m_dDOP;  // # of floating point divisions

    protected:
};

// ctor
template <class T>
CMatToolBox<T>::CMatToolBox ()
// ==================================================================
// Function: default constructor
//    Input: none
//   Output: none
// ==================================================================
{
    m_dASOP = m_dMOP = m_dDOP = 0.0;
}

// dtor
template <class T>
CMatToolBox<T>::~CMatToolBox ()
// ==================================================================
// Function: destructor
//    Input: none
//   Output: none
// ==================================================================
{
}

// ---------------------------------------------------------------
// ------------------------ vector functions ---------------------
// ---------------------------------------------------------------
template <class T>
void CMatToolBox<T>::Display (const std::string& szMessage,
                              const CVector<T>& A) const
// ==================================================================
// Function: displays a message and the elements of a vector
//    Input: message string and the vector 
//   Output: None
// ==================================================================
{
    std::cout << '\n' << szMessage << '\n';
    std::cout.setf(std::ios::left);
    for (int i=1; i <= A.GetSize(); i++)
    {
        std::cout << "(" << i << ") "
                  << std::setw(FW) << A(i) << " ";
        if ((i % ELEMENTSPERLINE) == 0)
            std::cout << '\n';
    }
}

template <class T>
bool CMatToolBox<T>::Add (const CVector<T>& A, const CVector<T>& B, 
                          CVector<T>& C)
// ==================================================================
// Function: adds two vectors and stores the result in the
//           third vector C = A + B
//    Input: vectors A and B 
//   Output: vector C
// ==================================================================
{
    // check for incompatible vectors
    int n = A.GetSize(); // returns column size
    if (n != B.GetSize() || n != C.GetSize())
        return false;

    // add
    for (int i=1; i <= n; i++)
        C(i) = A(i) + B(i);
    m_dASOP += static_cast<double>(n);

    return true;
}

template <class T>
bool CMatToolBox<T>::Subtract (const CVector<T>& A, 
                               const CVector<T>& B, CVector<T>& C)
// ==================================================================
// Function: subtracts one vector from another and stores the result
//           in the third vector C = A - B
//    Input: vectors A and B 
//   Output: vector C
// ==================================================================
{
    // check for incompatible vectors
	int n = A.GetSize(); // returns column size
    if (n != B.GetSize() || n != C.GetSize())
        return false;

	// subtract
	for (int i=1; i <= n; i++)
        C(i) = A(i) - B(i);
    m_dASOP += static_cast<double>(n);
	
	return true;
}

template <class T>
bool CMatToolBox<T>::DotProduct (const CVector<T>& A,
                                 const CVector<T>& B, T& product)
// ==================================================================
// Function: computes the dot product of two vectors such that
//           product = A dot B
//    Input: vectors A and B 
//   Output: product 
// ==================================================================
{ 
	// check for incompatible vectors
	int n = A.GetSize(); // returns column size
    if (n != B.GetSize())
        return false;

	// dot product of A and B
	product = 0.0;
	for (int i=1; i <= n; i++)
        product += A(i)*B(i);
    m_dMOP += static_cast<double>(n);
	m_dASOP += static_cast<double>(n);

    return true;
}

template <class T>
bool CMatToolBox<T>::Normalize (CVector<T>& A)
// ==================================================================
// Function: normalizes a vector
//    Input: vector A 
//   Output: normalized vector A 
// ==================================================================
{
    T sum(0);
    int n = A.GetSize();
    for (int i=1; i <= n; i++)
        sum += A(i)*A(i);

    m_dASOP += static_cast<double>(n);
    m_dMOP += static_cast<double>(n);

    if (sum == T(0))
        return false;
    else
    {
        sum = sqrt(sum); // magnitude
        for (int i=1; i <= n; i++)
            A(i) /= sum; // unit vector...
    }

    m_dDOP += static_cast<double>(n);

    return true;
}

template <class T>
bool CMatToolBox<T>::Scale (CVector<T>& A, T c)
// ==================================================================
// Function: scales a vector by a constant c such that A = c A
//    Input: vector A and constant c 
//   Output: scaled vector A
// ==================================================================
{
	// check magnitude of scale factor
	int n = A.GetSize();

	// scale vector
    for (int i=1; i <= n; i++)
	{
        A(i) = c*A(i);
	}

	m_dMOP += static_cast<double>(n);

	return true;
}

template <class T>
T CMatToolBox<T>::MaxValue (const CVector<T>& A)
// ==================================================================
// Function: finds the largest value among all the elements in A
//    Input: vector A 
//   Output: return value is the largest element in A
// ==================================================================
{
	int n = A.GetSize();	
	T max = A(1); // set initial max

	for (int i=1; i <= n; i++) 
	{
		if (A(i) >= max)
		{
			max = A(i); // set max if A(i) > than previous max
		}
	}
	return (max); // return max to client code
}

template <class T>
T CMatToolBox<T>::MinValue (const CVector<T>& A)
// ==================================================================
// Function: finds the smallest value among all the elements in A
//    Input: vector A 
//   Output: return value is the smallest element in A
// ==================================================================
{
	int n = A.GetSize();	
	T min = A(1); // set initial min

	for (int i=1; i <= n; i++) 
	{
		if (A(i) <= min)
		{
			min = A(i); // set min if A(i) < than previous min
		}
	}
	return (min); // return min to client code
}

template <class T>
T CMatToolBox<T>::TwoNorm (const CVector<T>& A)
// ==================================================================
// Function: computes the two norm of vector A
//    Input: vector A 
//   Output: return value is the two-norm
// ==================================================================
{
	T sum(0);
    int n = A.GetSize();
    for (int i=1; i <= n; i++)
        sum += A(i)*A(i);

    m_dASOP += static_cast<double>(n);
    m_dMOP += static_cast<double>(n);

    sum = sqrt(sum); // magnitude
	
	return sum;
}

template <class T>
T CMatToolBox<T>::MaxNorm (const CVector<T>& A)
// ==================================================================
// Function: computes the max norm of vector A
//    Input: vector A 
//   Output: return value is the max norm
// ==================================================================
{
	int n = A.GetSize();	
	T max = abs(A(1)); // set initial max (absolute)
	T maxi;
	for (int i=1; i <= n; i++) 
	{
		maxi = abs(A(i));
		if (maxi >= max)
		{
			max = maxi; // set max if A(i) > than previous max
		}
	}

	return (max); // return max to client code
}

template <class T>
bool CMatToolBox<T>::CrossProduct (const CVector<T>& A,
                                   const CVector<T>& B,
                                   CVector<T>& C)
// ==================================================================
// Function: computes the cross-product of two vectors and stores the
//           result in the third vector such that C = A x B
//           (3-dimensional space)
//    Input: vectors A, B and C
//   Output: vector C
// ==================================================================
{
	// check for incompatible vectors
	int n = A.GetSize(); // returns column size
	if (n < 3 || n > 3) // can only be three dimensions
		return false;
    if (n != B.GetSize() || n != C.GetSize()) // must be same size
        return false;

    for (int i=1; i <= n; i++) // populate cross product vector 
	{
		if (i == 1)
			C(i) = A(2)*B(3) - A(3)*B(2); 
		else if (i == 2)	
			C(i) = A(3)*B(1) - A(1)*B(3);
		else 
			C(i) = A(1)*B(2) - A(2)*B(1);	
	}
		
    m_dASOP += static_cast<double>(n);
    m_dMOP += static_cast<double>(2*n);

	return true;
}

// ---------------------------------------------------------------
// ------------------------ matrix functions ---------------------
// ---------------------------------------------------------------
template <class T>
void CMatToolBox<T>::Display (const std::string& szMessage,
                              const CMatrix<T>& A) const
// ==================================================================
// Function: displays a message and the elements of a matrix
//           rowwise
//    Input: message string and the matrix
//   Output: None
// ==================================================================
{
    std::cout << '\n' << szMessage << '\n';
    std::cout.setf(std::ios::left);
    for (int i=1; i <= A.GetRows(); i++)
    {
        int nC = 0;
        for (int j=1; j <= A.GetColumns(); j++)
        {
            ++nC;
            std::cout << "(" << i << "," << j << ") "
                      << std::setw(FW) << A(i,j) << " ";
            if ((nC % ELEMENTSPERLINE) == 0)
                std::cout << '\n';
        }
        std::cout << '\n';
    }
}

template <class T>
bool CMatToolBox<T>::Add (const CMatrix<T>& A, const CMatrix<T>& B, 
                          CMatrix<T>& C)
// ==================================================================
// Function: adds two matrices and stores the result in the
//           third matrix C = A + B
//    Input: matrices A and B 
//   Output: matrix C
// ==================================================================
{
	// check for incompatible matrices
	int m = A.GetRows(); 
	int n = A.GetColumns(); 
	if (m == B.GetRows() && n == B.GetColumns() && m == C.GetRows() && n == C.GetColumns())
	{
		// add
		for (int i = 1; i <= m; i++) // row indice 
		{
			for (int j = 1; j <= n; j++) // column indice
			{
				C(i,j) = A(i,j) + B(i,j);
			}
		}

		m_dASOP += static_cast<double>(n*m);

		return true;
	}
	else
		return false;
}

template <class T>
bool CMatToolBox<T>::Subtract (const CMatrix<T>& A,
                               const CMatrix<T>& B, CMatrix<T>& C)
// ==================================================================
// Function: subtracts one matrix from another and stores the result
//           in the third matrix C = A - B
//    Input: matrices A and B 
//   Output: matrix C
// ==================================================================
{
    // check for incompatible matrices
	int m = A.GetRows(); 
	int n = A.GetColumns(); 
	if (m == B.GetRows() && n == B.GetColumns() && m == C.GetRows() && n == C.GetColumns()) 
	{
		// subtract
		for (int i = 1; i <= m; i++) // row indice 
		{
			for (int j = 1; j <= n; j++) // column indice
			{
				C(i,j) = A(i,j) - B(i,j);
			}
		}

		m_dASOP += static_cast<double>(n*m);

		return true;
	}
	else
		return false;
}

template <class T>
bool CMatToolBox<T>::Multiply (const CMatrix<T>& A,
                               const CMatrix<T>& B, CMatrix<T>& C)
// ==================================================================
// Function: multiplies two matrices and stores the result
//           in the third matrix C = A * B
//    Input: matrices A and B 
//   Output: matrix C
// ==================================================================
{
	 // check for incompatible matrices
	T sum(0);
	int m = A.GetRows(); 
	int n = A.GetColumns(); 
	int nB = B.GetColumns();
	int mB = B.GetRows();
	int nC = C.GetColumns();
	int mC = C.GetRows();

	if (n == mB && m == mC && nB == nC)
	{
		// multiply
		for (int i = 1; i <= mC; i++) // row indice 
		{
			for (int j = 1; j <= nC; j++) // column indice
			{
				sum = 0;
				for (int k = 1; k <= n; k++)
				{
					sum += A(i,k)*B(k,j);
					C(i,j) = sum;
				}
			}
		}

		m_dMOP += static_cast<double>(mC*nC*n);
		m_dASOP += static_cast<double>((n-1)*mC*nC+1);

		return true;
	}
	else
		return false;
}

template <class T>
bool CMatToolBox<T>::Determinant (const CMatrix<T>& A, T& det)
// ==================================================================
// Function: computes the determinant of matrix A
//    Input: matrix A and variable to hold the determinant
//   Output: determinant
// ==================================================================
{
    return false;
}

template <class T>
void CMatToolBox<T>::Scale (CMatrix<T>& A, T c)
// ==================================================================
// Function: scales all the elements of a matrix by a constant c
//           such that A = c A
//    Input: matrix A and constant c
//   Output: scaled matrix A
// ==================================================================
{
	int m = A.GetRows(); 
	int n = A.GetColumns(); 

	// scale vector
	for (int i = 1; i <= m; i++) // row indice 
	{
		for (int j = 1; j <= n; j++) // column indice
		{
		   A(i,j) = c*A(i,j);
		}
	}

	m_dMOP += static_cast<double>(n*m);
}

template <class T>
T CMatToolBox<T>::MaxNorm (const CMatrix<T>& A)
// ==================================================================
// Function: computes the max norm of matrix A
//    Input: matrix A 
//   Output: return value is the max norm
// ==================================================================
{
	// get size of A
    int n = A.GetColumns();
	int m = A.GetRows();
	T max = abs(A(1,1));
	T maxi;

    for (int i=1; i <= m; i++) // loop thru rows
	{
		for (int j=1; j <= n; j++) // loop thru columns
		{
		  maxi = abs(A(i,j));
		  if (maxi > max) // checks if current max is larger than previous
			  max = maxi;
		}
	}

	return max; 
}

template <class T>
bool CMatToolBox<T>::Transpose (const CMatrix<T>& A,
                                CMatrix<T>& B)
// ==================================================================
// Function: computes the transpose of a matrix and stores the result
//           in another matrix B = A(T)
//    Input: matrices A and B
//   Output: matrix B
// ==================================================================
{
	// check for incompatible matrices
	int n = A.GetColumns();
	int m = A.GetRows();
	int nB = B.GetColumns();
	int mB = B.GetRows();
	if (n != m && n != mB) // checks case of rectangular matrix A versus C
		return false;
	else if (n != m && m != nB)
		return false;
	else if (n == m && n != nB) // checks case of square matrix A versus C
		return false;
	else if (n == m && m != mB)
		return false;
	else
		for (int i = 1; i <= m; i++) // row indice 
		{
			for (int j = 1; j <= n; j++) // column indice
			{
				B(j,i) = A(i,j);
			}
		}

	return true;
}

template <class T>
bool CMatToolBox<T>::MatMultVec (const CMatrix<T>& A,
                                 const CVector<T>& x,
                                 CVector<T>& b)
// ==================================================================
// Function: multiplies a matrix and a vector and stores the result
//           in a vector b = A * x
//    Input: vectors A and x 
//   Output: vector b
// ==================================================================
{
	T sum(0);
	// check for incompatible matrices
	int n = A.GetColumns();
	int m = A.GetRows();
	int nx = x.GetSize();
	int nb = b.GetSize();
	if (n != nx || m != nb) // check if A*x is defined, and if b is defined
		return false;
	else
		for (int i=1; i <= m; i++) // rows of A
		{
			sum = 0;
			for (int j=1; j <= n; j++) // columns of A
			{
				sum += A(i,j)*x(j);
				b(i) = sum;
			}
		}

	m_dMOP += static_cast<double>(n*m);
	m_dASOP += static_cast<double>(m*2+1);

	return true; 
}

template <class T>
bool CMatToolBox<T>::LUFactorization (CMatrix<T>& A, T TOL)
// ==================================================================
// Function: carries out LU factorization of matrix A
//           A is replaced with L and U
//    Input: matrix A and tolerance value to detect singular A
//   Output: matrix A 
// ==================================================================
{
    return false;
}

template <class T>
bool CMatToolBox<T>::LUSolve (const CMatrix<T>& A,
                              CVector<T>& x,
                              const CVector<T>& b)
// ==================================================================
// Function: carries out forward and backward substitution so as to
//           solve A x = b. A contains L and U terms.
//    Input: matrix A, vectors x and b
//   Output: vector x 
// ==================================================================
{
    return false;
}

template <class T>
bool CMatToolBox<T>::AxEqb (CMatrix<T>& A,
                            CVector<T>& x,
                            CVector<T>& b,
                            T TOL)
// ==================================================================
// Function: solves A x = b using Gaussian Elimination Technique
//           (this version only for one rhs vector)
//    Input: Matrices A and b
//   Output: Matrix x
//           A false return value indicates a singular A matrix.
// ==================================================================
{
    int i, j, k;   // loop indices
    T c;           // multiplier (Step 4)

    // number of equations to solve
    int n = A.GetRows();

    // x initially contains b
    x = b;

    // forward elimination
    for (k=1; k <= n-1; k++)              // Step 1
    {
        if (fabs(A(k,k)) <= TOL)          // Step 2
            return false;
        for (i=k+1; i <= n; i++)          // Step 3
        {
            c = A(i,k)/A(k,k);            // Step 4
            for (j=k+1; j <= n; j++)      // Step 5
                A(i,j) -= c * A(k,j);     // Step 6
            x(i) -= c * x(k);             // Step 8
        }                                 // Step 9
        int nC = n-k;
        if (nC > 0)
        {
            m_dDOP += static_cast<double>(nC);
            m_dMOP += static_cast<double>(nC*nC);
            m_dASOP += static_cast<double>(nC+nC*nC);
        }
    }                                     // Step 10 

    // back substitution
    if (fabs(A(n,n)) <= TOL)              
        return false;
    x(n) /= A(n,n);                       // Step 11

    for (i=n-1; i >= 1; i--)              // Step 12
    {
        T sum = T(0);
        for (j=i+1; j <= n; j++)
            sum += A(i,j) * x(j);         // Step 13
        if ((n-i) > 0)
        {
            m_dASOP += static_cast<double>(n-i);
            m_dMOP += static_cast<double>(n-i);
        }
        x(i) = (x(i) - sum)/A(i,i);       // Step 14
    }                                     // Step 15
    m_dASOP += static_cast<double>(n);
    m_dDOP += static_cast<double>(n+1);

    return true;
}

template <class T>
bool CMatToolBox<T>::LDLTFactorization (CMatrix<T>& A,
                                        T TOL)
// ==================================================================
// Function: carries out LDL(T) factorization of matrix A
//           A is replaced with L and D. A is a symmetric matrix.
//    Input: matrix A and tolerance value to detect singular A
//   Output: matrix A 
// ==================================================================
{
	T sum(0);
	// check for incompatible matrix (must be symmetric and square)
	int m = A.GetRows();
	int n = A.GetColumns();
	if (n != m) // check if square
		return false; 

	// check for positive definite (ev's < 0) in loop
	for (int i=1; i<=m; i++) 
	{
		for (int j=1; j<=n; j++)
		{
			if (A(i,j) != A(j,i)) // check for symmetry
				return false;
		}
				
	}

	// decompose
	for (int i=1; i<=m; i++)
	{
		sum = 0.0;
		for (int j=1; j<=i-1; j++) // sum loop
		{
			sum += A(i,j)*A(i,j)*A(j,j);
		}
		A(i,i) = A(i,i) - sum; // fill D
		if (A(i,i) < TOL)
			return false; // matrix is not positive definite
		for (int j=i+1; j<=m; j++)
		{
			sum = 0.0;
			for (int k=1; k<=i-1; k++) // sum loop
			{
				sum += A(j,k)*A(k,k)*A(i,k);
			}
			A(j,i) = (A(j,i) - sum)/A(i,i); // fill L
		}
	}
	return true;
}

template <class T>
bool CMatToolBox<T>::LDLTSolve (const CMatrix<T>& A,
                                CVector<T>& x,
                                const CVector<T>& b)
// ==================================================================
// Function: carries out forward and backward substitution so as to
//           solve A x = b. A contains L and D terms.
//    Input: matrix A, vectors x and b
//   Output: vector x 
// ==================================================================
{
	T sum(0);
	// check for incompatible matrix/vector (sizes)
	int m = A.GetRows();
	int n = A.GetColumns();
	int mb = b.GetSize();
	int mx = x.GetSize();
	if (n != mx || m != mb) // check if A*x is defined, and if b is defined
		return false;
	
	// forward substitution
	x(1) = b(1);
	for (int i=2; i<=m; i++)
	{
		sum = 0.0;
		for (int j=1; j<=i-1; j++)
		{
			sum += A(i,j)*x(j);
		}
		x(i) = b(i) - sum;
	} // end forward substitution

	// backward substitution
	x(m) = x(m)/A(m,m);
	for (int i=m-1; i>=1; i--)
	{
		sum = 0.0;
		for (int j=i+1; j<=m; j++)
		{
			sum += A(j,i)*x(j);
		}
		x(i) = x(i)/A(i,i) - sum;
	} // end backward substitution

    return true;
}

template <class T>
void CMatToolBox<T>::GetFLOPStats (double& dAS, double& dM, 
                                   double& dD) const
// ==================================================================
// Function: retrieves floating point operations
//    Input: variables to store +-, * and / operations
//   Output: variables with their values
// ==================================================================
{
    dAS = m_dASOP;
    dM  = m_dMOP;
    dD  = m_dDOP;
}

#endif
