/*********************************************
Design3D Program
Copyright(c) 2000,  Hawthorne & York, Inc.
All rights reserved
********************************************

Created On:         March 1, 2001
Created By:         S. D. Rajan
Last modified by:   S. D. Rajan
Last modified on:   Apr 18, 2001
Functionality:      Contains class definitions for vector template class CVector
					Implements vector as a simple container
					Indexing starts at 1
Input:              
Output:             
List of Changes:    
    4-18-2001  SDR  Documentation added
	8-09-2001  ADM  Lint

********************************************/

#ifndef __HYI_VECTORTEMPLATE_H
#define __HYI_VECTORTEMPLATE_H

#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::ostream;

#include <vector>

#include <assert.h>

static const int FATAL = 0;
static const int NOTFATAL = 1;

// defines the vector template class
template <class T>
class CVector {
	private:
		std::vector<T>	cells;

		void ErrorHandler (int nErrorCode,int nR=0) const;
#ifdef _DEBUG
		void RangeCheckIfDbg(int index) const { if (index < 1 || index > GetSize()) ErrorHandler(2,index); }
#else
		void RangeCheckIfDbg(int) const {}
#endif

	public:						
		CVector(void) : cells() {}
		CVector(int size) : cells() { SetSize(size); }
		CVector(const CVector<T>& that) : cells(that.cells) {}
		~CVector(void) {}

		const CVector& operator=(const CVector<T>& that) {if(this!=&that){cells=that.cells;} return *this;}

		// Note that CVector::SetSize() calls std::vector::resize(), which,
		// if it grows the vector, initializes all new cells as a copy of an
		// instance of T constructed with T's default constructor.  In the case
		// of primitive numeric types, the value of this instance is undefined;
		// hence the behavior of, say, "CVector<float> fV; fV.SetSize(5000);"
		// is to create a vector of 5000 floats all initialized to the same
		// undefined value.  Heh.  If this has any noticeable impact on
		// performance, we may want to partially specialize CVector over such
		// type not to use std::vector<>; or, we may find that we can modify
		// the client code either to avoid the call to SetSize(), or to avoid
		// redundant setting of values by combining the call to SetSize() with
		// a subsequent call to Set().
		// LINT SUPPRESSION: assigning address of auto variable to static; this
		// simply mimics the signature of std::vector::resize()
		void SetSize(int size, const T& v=T()) { cells.resize(size,v); } //lint !e789

		void Set(const T& value) // set all elements to the given value
			{ for(std::vector<T>::iterator p = cells.begin(); p != cells.end(); ++p) *p=value; }

		__forceinline int GetSize(void) const { return cells.size(); }

		// overloaded operators
		__forceinline T& operator()(int index) { RangeCheckIfDbg(index); return cells[index-1]; }
		__forceinline const T& operator()(int index) const { RangeCheckIfDbg(index); return cells[index-1]; }
};

// ==================== Error Handler ========================
template <class T>
void CVector<T>::ErrorHandler (int nErrorCode, int /*nR*/) const
{
#ifdef _DEBUG
	int errorLevel = FATAL;

	errorLevel = NOTFATAL;
	switch (nErrorCode)
	{
		case 1:
			std::cerr << "\nVector:: Memory allocation failure.\n";
			errorLevel = FATAL;
		break;
		case 2:
			std::cerr << "\nVector::Row index is out of bounds.\n";
			errorLevel = FATAL;
		break;
		case 3:
			std::cerr << "\nVector:: Incompatible matrices.\n";
		break;
		case 4:
			std::cerr << "\nVector::Constructor. Invalid number of rows or columns.\n";
			errorLevel = FATAL;
		break;
	}
	assert (errorLevel == NOTFATAL);
#else
	// TODO: How do we want to handle this situation?  Memory allocation
	// failure seems like a potential run-time error, so we probably want to
	// have a strategy for handling errors in release mode.  If we don't
	// handle errors in the release code, then we may as well save some
	// processor cycles by not checking for them.  Either way, this needs
	// work.
	(void) nErrorCode;
#endif
}

#endif
