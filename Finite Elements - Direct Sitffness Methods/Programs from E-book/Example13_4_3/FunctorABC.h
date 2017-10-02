/*********************************************
Example 13.4.3
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_FUNCTORABC_H__
#define __RAJAN_FUNCTORABC_H__

// abstract base class
template <class Type> class FunctorABC
{
    public:
    // operator () overloading 
    // virtual since derived classes will use a pointer to an object
    // and a pointer to a member function to make the function call
    virtual Type operator()(Type A, Type B) = 0;
};

#endif
