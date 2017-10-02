#include <cmath>
#include "..\library\matlib.h"
#include "numericalintegration.h"
#include "GaussLegendreTests.h"

const double TOLERANCE = 1.0e-3;

double OneDExample (double x)
{ 
	return 1.0;
    //return (1.0/x);
}

double TriDomainExample (double x, double y)
{
	return 1.0;
    //return (x*x + x*y*y);
}

double QuadDomainExample (double x, double y)
{
	return 1.0;
    //return (x*x + x*y*y);
}

double CircleExample (double x, double y, void* data)
{
	return 1.0;
    //return sin(x*x + y*y);
}

double TetraDomainExample (double x, double y, double z)
{
	return 1.0;
    //return (x*x + x*y*y + x*y*z);
}

double WedgeDomainExample (double x, double y, double z)
{
	return 1.0;
    //return (x*x + x*y*y + x*y*z);
}

double HexDomainExample (double x, double y, double z)
{
	return 1.0;
    //return (x*x + x*y*y + x*y*z);
}

CGaussLegendreTests::CGaussLegendreTests()
{
    //m_Problem = ONEDIMENSIONAL;
    //m_Problem = QUADRILATERAL;
    //m_Problem = CIRCLE;
    //m_Problem = TRIANGLE;
    //m_Problem = TETRAHEDRON;
    m_Problem = WEDGE; 
    //m_Problem = HEXAHEDRON;
    //m_Problem = TRUSS;
    //m_Problem = ALL;
}

CGaussLegendreTests::~CGaussLegendreTests()
{
}

void CGaussLegendreTests::RunTests ()
{
    // example 1 (1D)
    // example 2 (quadrilateral domain with straight sides)
    // example 3 (circular domain)
    // example 4 (triangular domain with straight sides)
    // example 5 (tetrahedral domain with planar surfaces)
    // example 6 (wedge domain with planar surfaces)
    // example 7 (hexahedral domain with planar surfaces)
    // example 8 (planar truss element)
    double dResult;
    bool bAnswer;

    // ------------------------------------------------------------------
    // -------------------------- 1D domain -----------------------------
    // ------------------------------------------------------------------
	if (m_Problem == ONEDIMENSIONAL || m_Problem == ALL)
    {
        // integration order and problem domain
	    int nIntegOrder = 2;
	    double dx1 = 1.0, dx2 = 7.0;

	    std::cout << "\n1D integration with order " << nIntegOrder 
                  <<   " from " << dx1 << " to " << dx2 << ": \n"; 
        bAnswer = m_TestCases.GQ1D (nIntegOrder, OneDExample, dx1, dx2, dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }

    // ------------------------------------------------------------------
    // ----------------------- quadrilateral domain ---------------------
    // ------------------------------------------------------------------
    if (m_Problem == QUADRILATERAL || m_Problem == ALL)
    {
        // integration order and problem domain
	    int nIntegOrder = 2;
        CVector<double> dXC(4), dYC(4);
	    dXC(1) =  1.0; dYC(1) =  1.0;
	    dXC(2) =  5.0; dYC(2) =  1.0;
	    dXC(3) =  6.0; dYC(3) =  6.0;
	    dXC(4) =  1.0; dYC(4) =  4.0;

	    std::cout << "\nIntegration over a quadrilateral domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQQuadDomain (nIntegOrder, QuadDomainExample, 
                                            dXC, dYC, dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ----------------------- circular domain ------------------------
    // ------------------------------------------------------------------
    if (m_Problem == CIRCLE || m_Problem == ALL)
    {
        // integration order and problem domain
	    int nIntegOrder = 10;
        double dRadius = 1.0;
        double dXC = 0.0, dYC = 0.0;
	    std::cout << "\nIntegration over a circular domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQSphere (nIntegOrder, CircleExample,
                                        NULL, 
                                        dRadius, dXC, dYC, dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ----------------------- triangular domain ------------------------
    // ------------------------------------------------------------------
    if (m_Problem == TRIANGLE || m_Problem == ALL)
    {
        // integration order and problem domain
	    int nIntegOrder = 2;
        CVector<double> dXC(3), dYC(3);
	    dXC(1) =  1.0; dYC(1) =  1.0;
	    dXC(2) =  5.0; dYC(2) =  1.0;
	    dXC(3) =  3.0; dYC(3) =  6.0;

	    std::cout << "\nIntegration over a triangular domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQTriDomain (nIntegOrder, TriDomainExample, 
                                           dXC, dYC, dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ----------------------- tetrahedral domain -----------------------
    // ------------------------------------------------------------------
    if (m_Problem == TETRAHEDRON || m_Problem == ALL)
    {
        // integration order and problem domain
        int nIntegOrder = 2;
        CMatrix<double> XL(3,4);
	    XL(1,1) =  0.0; XL(2,1) =  0.0; XL(3,1) = 0.0;
	    XL(1,2) =  1.0; XL(2,2) =  0.0; XL(3,2) = 0.0;
	    XL(1,3) =  0.0; XL(2,3) =  1.0; XL(3,3) = 0.0;
	    XL(1,4) =  0.0; XL(2,4) =  0.0; XL(3,4) = 1.0;

	    std::cout << "\nIntegration over a tetrahedral domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQTetraDomain (nIntegOrder, 
                                             TetraDomainExample, XL,
                                             dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ----------------------- wedge domain -----------------------------
    // ------------------------------------------------------------------
    if (m_Problem == WEDGE || m_Problem == ALL)
    {
        // integration order and problem domain
        int nIntegOrder = 2;
        CMatrix<double> XL(3,6);
        /*
	    XL(1,1) =  0.0; XL(2,1) =  0.0; XL(3,1) = -1.0;
	    XL(1,2) =  1.0; XL(2,2) =  0.0; XL(3,2) = -1.0;
	    XL(1,3) =  0.0; XL(2,3) =  1.0; XL(3,3) = -1.0;
	    XL(1,4) =  0.0; XL(2,4) =  0.0; XL(3,4) = 1.0;
	    XL(1,5) =  1.0; XL(2,5) =  0.0; XL(3,5) = 1.0;
	    XL(1,6) =  0.0; XL(2,6) =  1.0; XL(3,6) = 1.0;
        */
	    XL(1,1) =  0.0; XL(2,1) =  0.0; XL(3,1) = 0.0;
	    XL(1,2) =  0.0; XL(2,2) =  3.0; XL(3,2) = 0.0;
	    XL(1,3) =  0.0; XL(2,3) =  0.0; XL(3,3) = 2.0;
	    XL(1,4) =  6.0; XL(2,4) =  0.0; XL(3,4) = 0.0;
	    XL(1,5) =  6.0; XL(2,5) =  3.0; XL(3,5) = 0.0;
	    XL(1,6) =  6.0; XL(2,6) =  0.0; XL(3,6) = 2.0;

	    std::cout << "\nIntegration over a wedge domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQWedgeDomain (nIntegOrder, 
                                             WedgeDomainExample, XL,
                                             dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ----------------------- hexahedral domain ------------------------
    // ------------------------------------------------------------------
    if (m_Problem == HEXAHEDRON || m_Problem == ALL)
    {
        // integration order and problem domain
        int nIntegOrder = 2;
        CMatrix<double> XL(3,8);
	    XL(1,1) =  0.0; XL(2,1) =  0.0; XL(3,1) = 0.0;
	    XL(1,2) =  1.0; XL(2,2) =  0.0; XL(3,2) = 0.0;
	    XL(1,3) =  1.0; XL(2,3) =  1.0; XL(3,3) = 0.0;
	    XL(1,4) =  0.0; XL(2,4) =  1.0; XL(3,4) = 0.0;

	    XL(1,5) =  0.0; XL(2,5) =  0.0; XL(3,5) = 1.0;
	    XL(1,6) =  1.0; XL(2,6) =  0.0; XL(3,6) = 1.0;
	    XL(1,7) =  1.0; XL(2,7) =  1.0; XL(3,7) = 1.0;
	    XL(1,8) =  0.0; XL(2,8) =  1.0; XL(3,8) = 1.0;

	    std::cout << "\nIntegration over a hexahedral domain with order " 
                  << nIntegOrder << ": \n";
        bAnswer = m_TestCases.GQHexaDomain (nIntegOrder, HexDomainExample, 
                                            XL, dResult);
        // display the result
        if (bAnswer)
            std::cout << "Final Answer: " << dResult << std::endl;
        else
            std::cout << "Error during integration. Check data.\n";
    }
    // ------------------------------------------------------------------
    // ------------------ PLANAR TRUSS ELEMENT --------------------------
    // ------------------------------------------------------------------
    if (m_Problem == TRUSS || m_Problem == ALL)
    {
        // units: kg, m, s
        CMatrix<double> klocal(2,2); klocal.Set(0.0);
        CMatrix<double> B(1,2), BT(2,1), ktemp(2,2);
        double E = 200.0e9; // E steel, A = 0.01, L = 10
        double A = 0.01;
        double L = 10.0;

        CGaussLegendre GQ;
        int nIntegOrder = 1;

        // note k = integ(B(T)*EA*B, x=0..L)
        //        = sum over i (w(i)*[B(T)*EA*B*detJac @x(i)])

        // Step 1: loop thro' all integration points
        for (int nI=1; nI <= nIntegOrder; nI++)
        {
            // Step 2: get location and weight
            double dLocation = GQ.GetLocation (nIntegOrder, nI);
            double dWeight = GQ.GetWeight (nIntegOrder, nI);

            // Step 3: compute determinant of the jacobian
            double detJac = L/2.0;

            // Step 4: compute matrices at the current location
            B(1,1) = -1.0/L;  B(1,2) = 1.0/L;
            BT(1,1) = B(1,1); BT(2,1) = B(1,2);

            // Step 5: now update the stiffness matrix
            double dConst = E*A*dWeight;
            // compute B(T)*EA*B
            if (!Mult (ktemp, BT, B))
                std::cout << "Matrix multiplication error.\n";
            // update matrix
            for (int nRow=1; nRow <= 2; nRow++)
            {
                for (int nCol=1; nCol <= 2; nCol++)
                {
                    klocal(nRow,nCol) += ktemp(nRow,nCol)*dConst;
                }
            }
        }
        PrintMatrixRowWise (klocal, "truss local stiffness matrix", std::cout);
    }
}

