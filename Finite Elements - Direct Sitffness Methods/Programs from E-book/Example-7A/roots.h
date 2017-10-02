#ifndef __RAJAN_ROOTS_H__
#define __RAJAN_ROOTS_H__

#include "QuadPoly.h"
#include "CubicPoly.h"

class CRoots
{
    public:
        // ctor
        CRoots ();
        ~CRoots ();
        bool FindRoots (const CQuadPoly&, double dVRoots[]) const;
        bool FindRoots (const CCubicPoly&, double dVRoots[]) const;

    private:
        
};

#endif
