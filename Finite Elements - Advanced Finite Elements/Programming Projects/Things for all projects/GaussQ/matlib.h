/*********************************************
Design3D Program
Copyright(c) 2000,  Hawthorne & York, Inc.
All rights reserved
********************************************

Created On:         March 1, 2001
Created By:         S. D. Rajan
Last modified by:   S. D. Rajan
Last modified on:   Apr 18, 2001
Functionality:      Contains a library of class definitions and functions for 
					CVector type class CEngVector and CMatrix type class CEngMatrix
Input:              
Output:             
List of Changes:    
    4-18-2001  SDR  Documentation added
    7-02-2001  SDR  Added Reverse function
	7-30-2001  ADM  Unused code removed
	8-09-2001  ADM  Lint
	8-10-2001  ADM  Replaced CEngVector::Print, CEngVector::TwoNorm,
	                CEngVector::Reverse, CEngMatrix::Print, CEngMatrix::SetUpper,
					CEngMatrix::CholeskyDecompose, and CEngMatrix::CholeskySolve
					with non-member functions.
	8-10-2001  ADM  Changed CEngVector to contain, instead of derive from, CVector.
	8-29-2001  JEH	Completed Tony's changes.  CEngVector now maintains it's own vector,
					rather than using CVector.  Also added HYI_COLUMN_MAJOR macro, which
					if defined causes CEngMatrix to be configured in Column-major order,
					instead of row-major order.

********************************************/

#ifndef __HYI_MATLIB_H
#define __HYI_MATLIB_H

#include <math.h>

#include <iostream>
using std::cin;
using std::cout;
using std::ostream;

#include <iomanip>
using std::setiosflags;
using std::ios;
using std::left;
using std::setw;
using std::endl;

#include <bitset>

#include "vectortemplate.h"
#include "utilityfunctions.h"

const int NUM_ELEMENTS_PER_LINE = 4;
#ifdef _MSC_VER
template<class T>
const T& min(const T& a, const T& b) { return b < a ? b : a; } //lint !e1929
template<class T>
const T& max(const T& a, const T& b) { return a < b ? b : a; } //lint !e1929
#else
#include <algorithm>
using std::min;
using std::max;
#endif
const int ROWWISE = 0;

// ===========================================================================
// ===========================================================================
// ======================== vector class =====================================
// ===========================================================================
// ===========================================================================
template <class T>
class CEngVector
{
	public:
		CEngVector ();
		CEngVector (int);
		CEngVector(const CEngVector<T>&);
		~CEngVector ();

		// overloaded operators
		const CEngVector<T>& operator=(const CEngVector<T>&);
		__forceinline T& operator()(int index) 
		{
			//assert( (index >= 1) && (index <= m_nSize) );
			//return m_pCells[index-1]; 
			return m_pCells[index]; 
		}
		__forceinline const T& operator()(int index) const 
		{ 
			//assert( (index >= 1) && (index <= m_nSize) );
			//return m_pCells[index-1]; 
			return m_pCells[index]; 
		}

		void SetSize(int size);

		void Set(const T& value); // set all elements to the given value

		__forceinline int GetSize(void) const { return m_nSize; }

	private:
		T*          m_pCells;
		int			m_nSize;
};

template <class T>
CEngVector<T>::CEngVector ()	// default constructor
:   m_pCells(NULL),
    m_nSize(0)
{
}

template <class T>
CEngVector<T>::CEngVector (int nR)	// constructor
//:   m_pCells(static_cast<T*>(malloc(nR*sizeof(T)))),
:   m_pCells(new T[nR+1]),
    m_nSize(nR+1)
{
	assert (nR > 0);
	if(!m_pCells)
	{
		printf("PANIC.  CEngVector ctor FAILED to allocate m_pCells (nR=%d)\n", nR);
		printf("ABORTING.\n");
		exit(-1);
	}

}

template<class T>
CEngVector<T>::CEngVector(const CEngVector<T>& v)
:   m_pCells(NULL),
    m_nSize(0)
{
	*this = v;
}

template <class T>
CEngVector<T>::~CEngVector ()	// destructor
{
	//if(m_pCells) free(m_pCells);
	if(m_pCells) delete[] m_pCells;
}

template <class T>
const CEngVector<T>& CEngVector<T>::operator=(const CEngVector<T>& v)
{
	if(this != &v)
	{
		if(m_pCells != NULL) delete[] m_pCells; //free(m_pCells);
		if(v.m_pCells != NULL)
		{
			m_nSize = v.m_nSize;
			//m_pCells = static_cast<T*>(malloc(m_nSize*sizeof(T)));
			m_pCells = new T[m_nSize+1];
			if(!m_pCells)
			{
				printf("PANIC. CEngVector '=' operator: FAILED to allocate m_pCells (m_nSize=%d)\n",
					m_nSize);
				printf("ABORTING.\n");
				exit(-1);
			}
			memcpy(m_pCells,v.m_pCells,m_nSize*sizeof(T));
		}
	}
	return *this;
}

template<class T>
void CEngVector<T>::SetSize(int nSize)
{
	//m_pCells = static_cast<T*>(realloc(m_pCells,nSize*sizeof(T)));
	if(m_pCells) delete[] m_pCells;
	m_pCells = new T[nSize+1];
	if(!m_pCells)
	{
		printf("PANIC.  CEngVector::SetSize: FAILED to allocate memory.  nSize=%d\n",
			nSize);
		printf("ABORTING.\n");
		exit(-1);
	}
	m_nSize = nSize + 1;
}

template<class T>
void CEngVector<T>::Set(const T& value)
{
	if(m_pCells)
	{
		T * const end = m_pCells + m_nSize;
		for(T* begin = m_pCells; begin != end; ++begin)
			*begin = value;
	}
}



template <class T>
T TwoNorm(const CEngVector<T>& v)
{
	T SumOfSquares(0);
	for(int i = 1; i <= v.GetSize(); ++i)
		SumOfSquares += v(i)*v(i);
	return static_cast<T>(sqrt(SumOfSquares));
}

template <class T>
void PrintVector(const CEngVector<T>& v, const char* heading, ostream& Out)
{
	int i, j, k;
	
	Out << endl << heading << endl;
	for (i=1; i <= strlen(heading); i++)
		Out << '-';
	Out << endl;
	Out << setiosflags (ios::left);
	for (j=1; j <= v.GetSize(); j=j+NUM_ELEMENTS_PER_LINE)
	{
		for (k=j; k <= min(j+NUM_ELEMENTS_PER_LINE-1, v.GetSize()); k++) {
			Out << "[" << setw (4) << k << "]";
			Out << setw (15) << v(k) << " ";
		}
		Out << endl;
	}
}

template<class T>
void Reverse(CEngVector<T>& v, int nR)
{
	int k = nR/2;
	for(int i=1; i <= k; ++i)
	{
		const int iSwap = nR - (i-1);
		const T t = v(iSwap);
		v(iSwap) = v(i);
		v(i) = t;
	}
}


// ===========================================================================
// ===========================================================================
// ======================== matrix class =====================================
// ===========================================================================
// ===========================================================================

// Define this to organize your matrices in column-major order.
// Otherwise, they are defined in row-major order.
//#define HYI_COLUMN_MAJOR

template <class T>
class CEngMatrix
{
	private:
		int				nRows;
		int				nColumns;
		CEngVector<T>	data;
	public:
		CEngMatrix ();
		CEngMatrix (int nR, int nC);
		CEngMatrix (const CEngMatrix<T>&);
		~CEngMatrix ();

		__forceinline T& operator()(int, int);
		__forceinline const T& operator()(int, int) const;
		const CEngMatrix<T>& operator=(const CEngMatrix<T>&);

		__forceinline int GetRows(void) const { return nRows; }
		__forceinline int GetColumns(void) const { return nColumns; }

		void SetSize(int nR, int nC);
		void Set(const T& v);
};

template<class T>
void PrintMatrix (const CEngMatrix<T>& matrix, const char* heading, ostream& Out, int nType,
                  int nSRow, int nERow, int nSCol,
                  int nECol);

template <class T>
int CholeskyDecompose (CEngMatrix<T>& v, T ToleranceValue);

template <class T>
int CholeskySolve (const CEngMatrix<T>& U, CEngMatrix<T>& B, int nRHS);

template <class T>
CEngMatrix<T>::CEngMatrix ()	// default constructor
:   nRows(0),
    nColumns(0),
	data()
{
}

template <class T>
CEngMatrix<T>::CEngMatrix (int nR, int nC)	// constructor
:   nRows(nR),
    nColumns(nC),
	data(nR*nC)
{
	assert (nR > 0 && nC > 0);
}

template <class T>
CEngMatrix<T>::~CEngMatrix ()	// destructor
{
}

template<class T>
__forceinline T& CEngMatrix<T>::operator()(int i, int j)
{
	assert(i >= 1 && i <= nRows && j >= 1 && j <= nColumns);
#ifdef HYI_COLUMN_MAJOR
	return data((j-1)*nRows+i);
#else
	return data((i-1)*nColumns+j);
#endif /* !COLUMN_MAJOR */
}

template<class T>
__forceinline const T& CEngMatrix<T>::operator()(int i, int j) const
{
	assert(i >= 1 && i <= nRows && j >= 1 && j <= nColumns);
#ifdef HYI_COLUMN_MAJOR
	return data((j-1)*nRows+i);
#else
	return data((i-1)*nColumns+j);
#endif /* !COLUMN_MAJOR */
}

template<class T>
const CEngMatrix<T>& CEngMatrix<T>::operator=(const CEngMatrix<T>& m)
{
	if(this != &m)
	{
		nRows = m.nRows;
		nColumns = m.nColumns;
		data = m.data;
	}
	return *this;
}

template<class T>
void CEngMatrix<T>::SetSize(int nR, int nC)
{
	assert(nR >= 1 && nC >= 1);
	nRows = nR;
	nColumns = nC;
	data.SetSize(nR*nC);
}

template<class T>
void CEngMatrix<T>::Set(const T& v)
{
	assert(nRows >= 1 && nColumns >= 1);
	data.Set(v);
}

template <class T>
void PrintMatrix (const CEngMatrix<T>& matrix, const char* heading, ostream& Out, int nType,
                  int nSRow, int nERow, int nSCol,
                  int nECol)
{
	int i, j, k;
	
	Out << endl << heading << endl;
	for (i=1; i <= strlen(heading); i++)
		Out << '-';
	Out << endl;
	Out << setiosflags (ios::left);
	if (nType == ROWWISE)
	{
		for (i=nSRow; i <= nERow; i++)
		{
			Out << "Row No: " << i << endl;
			for (j=nSCol; j <= nECol; j=j+NUM_ELEMENTS_PER_LINE)
			{
				for (k=j; k <= min(j+NUM_ELEMENTS_PER_LINE-1, matrix.GetColumns()); k++) {
					Out << "[" << setw (4) << k << "]";
					Out << setw (15) << matrix(i,k) << " ";
				}
				Out << endl;
			}
		}
	}
	else {
		for (i=nSCol; i <= nECol; i++)
		{
			Out << "Column No: " << i << endl;
			for (j=nSRow; j <= nERow; j=j+NUM_ELEMENTS_PER_LINE)
			{
				for (k=j; k <= min(j+NUM_ELEMENTS_PER_LINE-1, matrix.GetRows()); k++) {
					Out << "[" << setw (4) << k << "]";
					Out << setw (15) << matrix(k,i) << " ";
				}
				Out << endl;
			}
		}
	}
}

// ==================== Generic Matrix Routines ========================
// Cholesky Decomposition A=U(T)T 
template <class T>
int CholeskyDecompose (CEngMatrix<T>& v, T ToleranceValue)
{
	int i,j,jk,k,l,m,n;
	T sum, dummy;

	// check legality of data
	if (v.GetColumns() > v.GetRows()) return (-1);

	// set problem size
	const int nEquations = v.GetRows();
	const int nHBW = v.GetColumns();

	// loop through all rows
	for (i=1; i <= nEquations; i++)
	{	
		if ((i % 1000) == 0)
        {
            CUtility::DisplayOnConsole ("\nReducing equation ", i);
        }
        l = (nEquations-i)+1;
		if (nHBW < l) l = nHBW;
		for (j=1; j <= l; j++)
		{
			m = nHBW-j;
			if ((i-1) < m) m=i-1;
			sum=v(i,j);
			if (m >= 1) {
				for (k=1; k <= m; k++)
				{
					n = i-k;
					jk = j+k;
					sum = sum-v(n,k+1)*v(n,jk);
				}
			}
			if (j <= 1) {
				if (sum < ToleranceValue) { 
					return (i);		// non-positive definite matrix
				}
				dummy = 1.0/sqrt(sum);
				v(i,j) = dummy;
			}
			else {
				// LINT SUPPRESSION: dummy is always initialized the first
				// time through this loop (unless it returns, in which case we
				// never get here), so dummy must be initialized if we're here.
				// Lint evidently doesn't see that, so we suppress the spurious
				// message about possibly using an uninitialized dummy.
				v(i,j) = sum*dummy; //lint !e644
			}
		}
	}

	// no errors
	return (0);
}


// solves AX=b (A is banded matrix)
// U is the decomposition of A
// the "nRHS"th column of B contains b on the way in, and x on the way out
// (B is an in/out parameter).
template <class T>
int CholeskySolve (const CEngMatrix<T>& U, CEngMatrix<T>& B, int nRHS)
{
	int i,ii,j,k,kl,l,lk,m,n;
	T sum;

	// check legality of data
	if (U.GetColumns() > U.GetRows()) return (-1);
	if (U.GetRows() != B.GetRows()) return (-1);
	if (nRHS <= 0 && nRHS > B.GetColumns()) return (-1);

	// set problem size
	const int nEquations = U.GetRows();
	const int nHBW = U.GetColumns();

	// forward substitution
	B(1,nRHS) *= U(1,1);
	for (i=2; i <= nEquations; i++)
	{
		l = i+1;
		j = l-nHBW;
		if (l <= nHBW) j=1;
		sum = B(i,nRHS);
		m = i-1;
		for (k=j; k <= m; k++)
		{
			lk = l-k;
			sum -= U(k,lk)*B(k,nRHS);
		} 
		B(i,nRHS) = sum*U(i,1);
	} 

	// back substitution 
	B(nEquations,nRHS) *= U(nEquations,1);
	ii = nEquations+1;
	for (n=2; n <= nEquations; n++)
	{
		i = ii-n;
		l = i-1;
		j = l+nHBW;
		if (j > nEquations) j=nEquations;
		sum = B(i,nRHS);
		m = i+1;
		for (k=m; k <= j; k++)
		{
			kl = k-l;
			sum -= U(i,kl)*B(k,nRHS);
		}
		B(i,nRHS) = sum*U(i,1);
	}

	return (0);
}

template<class T>
void SetUpper(CEngMatrix<T>& v, const T& t)
{
	for (int i=1; i <= v.GetRows(); i++) {
		for (int j=i; j <= v.GetColumns(); j++) {
			v(i,j) = t;
		}
	}
}

// JEH: I tried to make a column-major optimized version of SetUpper, but it actually
// seemed to slow things down.
//template<class T>
//void SetUpper(CEngMatrix<T>& v, const T& t)
//{
//	int nrows = v.GetRows();
//	int ncols = v.GetColumns();
//	int i,j,ilimit;
//
//	for(j=1; j<=ncols; j++)
//	{
//		ilimit = min(j,nrows);
//		for(i=1; i<=ilimit; i++)
//		{
//			v(i,j) = t;
//		}
//	}
//}


#if 0

JEH: I inherited CBigEngVector, but it does not appear to be used anywhere.  We will
		 put it here for reference purposes only; it will not be compiled.

// ===========================================================================
// ===========================================================================
// ======================== big vector class =================================
// ===========================================================================
// ===========================================================================

template <class T>
class CBigEngVector
{
	public:
		CBigEngVector();
		CBigEngVector(int);
		~CBigEngVector();
		__forceinline T& operator()(int index) { return GetElement(index); }
		__forceinline const T& operator()(int index) const { return GetElement(index); }
		void SetSize(int);
		void SetToZero(void);
		__forceinline int GetSize(void) const { return m_nSize; }
	private:
		typedef unsigned long PageNum_t;

		CBigEngVector(const CBigEngVector<T>&);
		const CBigEngVector<T>& operator=(const CBigEngVector<T>&);

		__forceinline T& GetElement(int) const;
		__forceinline static T* PageStart(PageNum_t iPage)
		{
			return reinterpret_cast<T*>(iPage<<12);
		}
		__forceinline static T* PageEnd(PageNum_t iPage) { return PageStart(iPage+1); }

		T*									m_pCells;
		int									m_nSize;
		PageNum_t							m_iFirstPage;
		mutable std::bitset<0x000FFFFFUL>	m_vbPageNeedsToBeZeroed;
};

	 template<class T>
CBigEngVector<T>::CBigEngVector()
:   m_pCells(NULL),
    m_nSize(0),
	m_iFirstPage(0),
	m_vbPageNeedsToBeZeroed()
{
}

template<class T>
CBigEngVector<T>::CBigEngVector(int nR)
:   m_pCells(NULL),
    m_nSize(0),
	m_iFirstPage(0),
	m_vbPageNeedsToBeZeroed()
{
	SetSize(nR);
}

template<class T>
CBigEngVector<T>::~CBigEngVector()
{
	if(m_pCells) free(m_pCells);
}

template<class T>
void CBigEngVector<T>::SetSize(int nR)
{
	if(!(m_pCells == NULL && m_nSize == 0 && m_iFirstPage == 0 && m_vbPageNeedsToBeZeroed.none()))
	{
		cout << "CBigEngVector::SetSize() called on tainted vector." << endl;
		exit(1);
	}

	m_pCells = static_cast<T*>(malloc(nR*sizeof(T)));
	if(m_pCells == NULL)
	{
		cout << "Memory allocation failed." << endl;
		exit(1);
	}

    m_nSize = nR;
	m_iFirstPage = reinterpret_cast<unsigned long>(m_pCells) >> 12;
}

template<class T>
void CBigEngVector<T>::SetToZero(void)
{
	m_vbPageNeedsToBeZeroed.set();
}

template<class T>
__forceinline T& CBigEngVector<T>::GetElement(int i) const
{
	T* const pElement = m_pCells + i - 1;
	const PageNum_t iPage = reinterpret_cast<unsigned long>(pElement) >> 12;
	if(m_vbPageNeedsToBeZeroed.test(iPage-m_iFirstPage))
	{
		m_vbPageNeedsToBeZeroed.reset(iPage-m_iFirstPage);
		T* const pEnd = min(m_pCells+m_nSize,PageEnd(iPage));
		for(T* p = max(m_pCells,PageStart(iPage)); p != pEnd; ++p)
			*p = T(0);
	}
	return *pElement;
}

#endif /* 0 */

#endif


