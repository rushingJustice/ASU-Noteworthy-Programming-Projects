/*********************************************
Example 13.4.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef _RAJAN_MATH2LIB_H__
#define _RAJAN_MATH2LIB_H__

#include "FunctorABC.h"

// derived template class
template <class TClass, class Type> class CMath2Lib : public FunctorABC<Type>
{
    public:
        // constructor - takes pointer to an object and pointer to a member
        // and stores them in two private variables
        CMath2Lib (TClass* _pt2Object, Type(TClass::*_fpt)(Type, Type))
        { m_pt2Object = _pt2Object; m_fpt=_fpt; };

        // override operator "()"
        virtual Type operator()(Type a, Type b)
        { return (*m_pt2Object.*m_fpt)(a, b);}; 

    private:
        Type (TClass::*m_fpt)(Type a, Type b); // pointer to member function
        TClass* m_pt2Object;                   // pointer to object
};

// specific functions in the library follow
template <class Type> class CAddSimple2
{
    public:
        CAddSimple2(){};
        Type AddThem (Type a, Type b) { return (a+b); };
};

template <class Type> class CAddSquare2
{
    public:
        CAddSquare2(){};
        Type AddThem (Type a, Type b) { return (a*a + b*b); };
};

#endif