#include "roots.h"

CRoots::CRoots ()
{
}

CRoots::~CRoots ()
{
}

bool CRoots::FindRoots (const CQuadPoly& QP, double dVRoots[]) const
{
    return (QP.Solve (dVRoots));
}

bool CRoots::FindRoots (const CCubicPoly& CP, double dVRoots[]) const
{
    return (CP.Solve (dVRoots));
}
