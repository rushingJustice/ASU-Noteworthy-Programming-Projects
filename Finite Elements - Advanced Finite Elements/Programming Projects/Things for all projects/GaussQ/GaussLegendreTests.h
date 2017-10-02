#ifndef __RAJAN_GaussLegendreTests_H__
#define __RAJAN_GaussLegendreTests_H__

#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "NumericalIntegration.h"

class CGaussLegendreTests
{
    public:
        enum LISTOFPROBLEMS {ALL, ONEDIMENSIONAL, QUADRILATERAL, 
                             CIRCLE, TRIANGLE,
                             TETRAHEDRON, WEDGE, HEXAHEDRON,
                             TRUSS};

        CGaussLegendreTests();
        ~CGaussLegendreTests();

        void RunTests ();

    private:
        LISTOFPROBLEMS     m_Problem;
        CNIonKnownShapes   m_TestCases;
};


#endif
