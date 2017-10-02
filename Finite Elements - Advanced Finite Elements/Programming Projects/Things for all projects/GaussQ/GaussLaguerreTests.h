#ifndef __RAJAN_GaussLaguerreTests_H__
#define __RAJAN_GaussLaguerreTests_H__

#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "NumericalIntegration.h"
#include "GaussLaguerreTests.h"

class CGaussLaguerreTests
{
    public:
        enum LISTOFPROBLEMS {ALL, ONEDIMENSIONAL, QUADRILATERAL, 
                             CIRCLE, TRIANGLE,
                             TETRAHEDRON, WEDGE, HEXAHEDRON};

        CGaussLaguerreTests();
        ~CGaussLaguerreTests();

        void RunTests ();

    private:
        LISTOFPROBLEMS     m_Problem;
        CNIonKnownShapes   m_TestCases;
};


#endif
