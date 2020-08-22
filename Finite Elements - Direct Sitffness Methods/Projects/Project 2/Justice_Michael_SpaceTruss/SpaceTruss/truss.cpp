/*********************************************
Planar Truss Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CTruss class implementation.

*********************************************/
#include <iomanip>
#include <sstream>
#include "truss.h"
#include "matlib.h"
#include "MatToolBox.h"
#include "..\library\fileio.h"
#include "..\library\parser.h"

const int MAXCHARS = 80;
CMatToolBox<float> MTB;       // single precision version
CMatToolBox<double> MTBDP;    // double precision version
std::string szInputString;
std::string szComment ("**");

/* ==================================================================
   ======================= CTruss class =============================
   ================================================================== */

CTruss::CTruss ()
// ---------------------------------------------------------------------------
// Function: default ctor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nNodes = m_nElements = m_nDOF = m_nCount = 0;
    m_nDebugLevel = m_nLineNumber = 0;
	m_dNorm = m_drelNorm = 0.0;
}

CTruss::~CTruss ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CTruss::Banner (std::ostream& OF) const
// ---------------------------------------------------------------------------
// Function: prints the program banner on the output stream
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    OF << '\n';
    OF << "\t\t--------------------------------------------" << '\n';
    OF << "\t\t        Space Truss Analysis Program       " << '\n';
    OF << "\t\tIntroduction to Structural Analysis & Design" << '\n';
    OF << "\t\t          (c) 2000-15, S. D. Rajan          " << '\n';
    OF << "\t\t        Enhanced By: Michael Justice        " << '\n';
    OF << "\t\t--------------------------------------------" << '\n';
}

void CTruss::PrepareIO (int argc, char* argv[])
// ---------------------------------------------------------------------------
// Function: opens the input and output files
// Input:    command line arguments (currently unused)
// Output:   none
// ---------------------------------------------------------------------------
{
    if (argc == 1)
    {
        // open the input file
        OpenInputFileByName ("Complete input file name: ", m_FileInput,
                             std::ios::in);

        // open the output file
        OpenOutputFileByName ("Complete output file name: ", m_FileOutput,
                              std::ios::out);
    }
    else if (argc == 3) // spacetruss input_file output_file
    {
        m_FileInput.open (argv[1], std::ios::in);
        if (!m_FileInput)
            ErrorHandler (ERRORCODE::CANNOTOPENIFILE);
        m_FileOutput.open (argv[2], std::ios::out);
        if (!m_FileOutput)
            ErrorHandler (ERRORCODE::CANNOTOPENOFILE);
        std::cout << "\n";
        std::cout << argv[1] << " opened as input file.\n";
        std::cout << argv[2] << " opened as output file.\n";
    }
	else
    {
        ErrorHandler (ERRORCODE::INVALIDCOMMANDLINE);
    }

    // print banner
    Banner (m_FileOutput);
}

void CTruss::ReadProblemSize ()
// ---------------------------------------------------------------------------
// Function: Reads the size of the problem being solved
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    CParser Parse; // checks each line for information

    // header line
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
    // read the problem description
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);

    // header line
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
    // read number of nodes, elements and debug level
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
    std::istringstream szFormatString (szInputString);
    szFormatString >> m_nNodes >> m_nElements >> m_nDebugLevel; // in order (see input)
    if (szFormatString.fail() || szFormatString.bad())
        ErrorHandler (ERRORCODE::INVALIDINPUT);

    // check data for validity
    if (m_nNodes <= 1) ErrorHandler (ERRORCODE::INVALIDNUMNODES);
    if (m_nElements <= 0) ErrorHandler (ERRORCODE::INVALIDNUMELEMENTS);
    if (m_nDebugLevel < 0 || m_nDebugLevel > 1) ErrorHandler (ERRORCODE::INVALIDDEBUGCODE);

    // dynamic memory allocations for arrays
    SetSize ();
}

void CTruss::SetSize ()
// ---------------------------------------------------------------------------
// Function: Carries out memory allocation for all the major arrays
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // allocate space for nodal data
    m_NodalData.SetSize (m_nNodes);
    // allocate space for nodal response data
    m_NodalResponseData.SetSize (m_nNodes);
    // allocate space for element data
    m_ElementData.SetSize (m_nElements);
    // allocate space for element response data
    m_ElementResponseData.SetSize (m_nElements);

    // allocate and initialize major matrices
    m_nDOF = 3*m_nNodes;
    m_dSSM.SetSize (m_nDOF, m_nDOF); // dynamically allocated
    m_dSND.SetSize (m_nDOF, 1);
    m_dSNF.SetSize (m_nDOF, 1);
    m_dSSM.Set (0.0);
    m_dSND.Set (0.0);
    m_dSNF.Set (0.0);
}

void CTruss::ReadTrussModel ()
// ---------------------------------------------------------------------------
// Function: Read the truss model data from the input file
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // read problem size
    ReadProblemSize ();
    
    int i, nN;
	int nc = 0;
    float fX, fY, fZ;
    CParser Parse;
    
    // header line (nodal coordinates...*)
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
    // read nodal coordinates
    for (i=1; i <= m_nNodes; i++)
    {
        if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                                 MAXCHARS, szComment))
            ErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            std::istringstream szFormatString (szInputString);
            szFormatString >> nN >> fX >> fY >> fZ; // again, in order...
            if (szFormatString.fail() || szFormatString.bad())
                ErrorHandler (ERRORCODE::INVALIDINPUT);
        }
        if (nN <= 0 || nN > m_nNodes) 
            ErrorHandler (ERRORCODE::INVALIDNODENUM);
        m_NodalData(nN).SetCoords (fX, fY, fZ);
    }

    // header line (nodal fixity)
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
	
    // read nodal fixity conditions and initial displacements. initialize all nodes to begin.
    float fidx, fidy, fidz; fidx = fidy = fidz = 0.0;
    int nXFC, nYFC, nZFC; nXFC = nYFC = nZFC = 0;

	for (i=1; i <= m_nNodes; i++) 
	{
	    m_NodalData(i).SetFixity (nXFC, nYFC, nZFC);
		m_NodalData(i).SetIDisp (fidx, fidy, fidz);
	}

    for (i=1; i <= m_nNodes; i++) // add check if less than number of nodes in input
    {
        if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
            ErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            std::istringstream szFormatString (szInputString);
            szFormatString >> nN >> nXFC >> nYFC >> nZFC >> fidx >> fidy >> fidz; // note that 0 = free
            if (szFormatString.fail() || szFormatString.bad())
			{
				break;
                ErrorHandler (ERRORCODE::INVALIDINPUT);
			}
        }
        if (nN <= 0 || nN > m_nNodes) ErrorHandler (ERRORCODE::INVALIDNODENUM);
        if (nXFC < 0 || nXFC > 1) ErrorHandler (ERRORCODE::INVALIDNODALFIXITY);
        if (nYFC < 0 || nYFC > 1) ErrorHandler (ERRORCODE::INVALIDNODALFIXITY);
		if (nZFC < 0 || nZFC > 1) ErrorHandler (ERRORCODE::INVALIDNODALFIXITY);
		if (nXFC == 0 && fidx != 0.0) ErrorHandler (ERRORCODE::INVALIDNODALDISP);
		if (nYFC == 0 && fidy != 0.0) ErrorHandler (ERRORCODE::INVALIDNODALDISP);
		if (nZFC == 0 && fidz != 0.0) ErrorHandler (ERRORCODE::INVALIDNODALDISP);
        m_NodalData(nN).SetFixity (nXFC, nYFC, nZFC);
		m_NodalData(nN).SetIDisp (fidx, fidy, fidz);
		nc += 1; // counter for parse
    }

    // header line (nodal loads)
	if (nc == m_nNodes)
	{
		if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
							      MAXCHARS, szComment))
           ErrorHandler (ERRORCODE::INVALIDINPUT);
	}
    // read nodal loads such as conc. nodal forces and nodal temperature changes. Initialize to begin.
	float fXF, fYF, fZF, fDTemp; fXF = fYF = fZF = fDTemp = 0.0;
	nc = 0; // initlialize counter

	for (i=1; i <= m_nNodes; i++) 
	{
		m_NodalData(i).SetLoads (fXF, fYF, fZF, fDTemp);
	}

    for (i=1; i <= m_nNodes; i++)
    {
        if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
            ErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            std::istringstream szFormatString (szInputString);
            szFormatString >> nN >> fXF >> fYF >> fZF >> fDTemp; 
            if (szFormatString.fail() || szFormatString.bad())
			{
				break;
                ErrorHandler (ERRORCODE::INVALIDINPUT);
			}
        }
        if (nN <= 0 || nN > m_nNodes) ErrorHandler (ERRORCODE::INVALIDNODENUM);
        m_NodalData(nN).SetLoads (fXF, fYF, fZF, fDTemp);
		nc += 1; // counter for parse
    }

    // header line (element data...)
	if (nc == m_nNodes)
	{
	   if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                                MAXCHARS, szComment))
          ErrorHandler (ERRORCODE::INVALIDINPUT);
	}

    // read element data
    int nE, nSN, nEN;
    float fA, fE, fCTE;
    for (i=1; i <= m_nElements; i++)
    {
        if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
            ErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            std::istringstream szFormatString (szInputString);
            szFormatString >> nE >> nSN >> nEN >> fA >> fE >> fCTE;
            if (szFormatString.fail() || szFormatString.bad())
                ErrorHandler (ERRORCODE::INVALIDINPUT);
        }
        if (nE <= 0 || nE > m_nElements) ErrorHandler (ERRORCODE::INVALIDELEMENTNUM);
        if (nSN <= 0 || nSN > m_nNodes) ErrorHandler (ERRORCODE::INVALIDNODENUM);
        if (nEN <= 0 || nEN > m_nNodes) ErrorHandler (ERRORCODE::INVALIDNODENUM);
        if (fA <= 0.0f) ErrorHandler (ERRORCODE::INVALIDCSAREA);
        if (fE <= 0.0f) ErrorHandler (ERRORCODE::INVALIDYM);
        m_ElementData(i).SetData (nSN, nEN, fA, fE, fCTE); // change i to nE for out-of-order
    }

    // end keyword?
    if (!Parse.ReadNextLine (m_FileInput, m_nLineNumber, szInputString, 
                             MAXCHARS, szComment))
        ErrorHandler (ERRORCODE::INVALIDINPUT);
    if (szInputString.substr(0,4) != "*end")
        ErrorHandler (ERRORCODE::MISSINGEND);

    // construct structural nodal load vector. 
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetLoads (fXF, fYF, fZF, fDTemp);
        m_dSNF(3*i-2,1) = fXF; 
        m_dSNF(3*i-1,1) = fYF; 
		m_dSNF(3*i,1)   = fZF;
    }
}

void CTruss::Analyze ()
// ---------------------------------------------------------------------------
// Function: Implements the FEA steps
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	// construct structural stiffness matrix
	ConstructK ();

	// impose boundary conditions
	ImposeBC (); // step 4

	// solve for the nodal displacements
	Solve (); // step 5

	// compute element response
	Response (); // step 6 (and final)
	
    // create output file
	CreateOutput ();
}

void CTruss::ConstructK ()
// ---------------------------------------------------------------------------
// Function: Constructs the structural stiffness matrix
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	//change these values for 3d
    int i, j, k;
    CVector<int> nE(6);
    CMatrix<double> dkl(2,2), dkg(6,6);
    CMatrix<double> dT(2,6), dTT(6,2);
    CMatrix<double> dTemp(2,6);
	CVector<double> dq(2), dQ(6); // force vector due to temp changes in element
    float fX1, fX2, fY1, fY2, fZ1, fZ2, fL; // coordinates and length
	float fXF, fYF, fZF; // X and Y force at node
    float fA, fE, fCTE, fDTempS, fDTempE; // A, E, CTE, temp changes at nodes
	float fEleDTemp; // temp change at element
    int nSN, nEN; // start and end node
    
    // initialize
    dT.Set (0.0);
	m_de.SetSize (m_nElements); // make size of DOF for selection
	m_de.Set(0.0);
    
    // loop thro' all elements
    for (i=1; i <= m_nElements; i++)
    {
        // construct k(4x4) in two steps
        m_ElementData(i).GetData (nSN, nEN, fA, fE, fCTE); // assign element its characteristics.
        m_NodalData(nSN).GetCoords (fX1, fY1, fZ1);
        m_NodalData(nEN).GetCoords (fX2, fY2, fZ2);
        fL = sqrt((fX2-fX1)*(fX2-fX1) +
                  (fY2-fY1)*(fY2-fY1) + 
				  (fZ2-fZ1)*(fZ2-fZ1));
        // form AE/L (stiffness)
        double dAEOL = double(fA*fE/fL); 
        // construct klocal (2x2) (kprime...)
        dkl(1,1) = dAEOL;
        dkl(1,2) = -dAEOL;
        dkl(2,1) = dkl(1,2);
        dkl(2,2) = dkl(1,1);
        // local-to-global transformation matrix (directional cosines...)
        double dl = double((fX2-fX1)/fL); 
        double dm = double((fY2-fY1)/fL);
		double dn = double((fZ2-fZ1)/fL);

        dT(1,1) = dl; dT(1,2) = dm; dT(1,3) = dn;
        dT(2,4) = dl; dT(2,5) = dm; dT(2,6) = dn;

        // transpose of the T matrix.
		MTBDP.Transpose(dT, dTT);
        // form k'*T
        MTBDP.Multiply (dkl, dT, dTemp);
        // construct kglobal=T(T)*k'*T
		MTBDP.Multiply (dTT, dTemp, dkg);

        // assemble into structural K 
        nE(1) = 3*nSN-2; nE(2) = 3*nSN-1; nE(3) = 3*nSN;
        nE(4) = 3*nEN-2; nE(5) = 3*nEN-1; nE(6) = 3*nEN;

		// loop through 6 times, not 4 (for 3d). this forms stiffness matrix for one element
        for (j=1; j <= 6; j++)
        {
            int nRow = nE(j);
            for (k=1; k <= 6; k++)
            {
                int nCol = nE(k);
                m_dSSM(nRow, nCol) += dkg(j,k); // form structural stiffness matrix 
            }
        }

		// apply thermal loading to SNF
		m_NodalData(nSN).GetLoads (fXF, fYF, fZF, fDTempS);
		m_NodalData(nEN).GetLoads (fXF, fYF, fZF, fDTempE);

		fEleDTemp = (fDTempS + fDTempE)/2; // average temperature change at element
		m_de(i) = fCTE*fEleDTemp; // thermal strain of element
		dq(1) = fE*fA*m_de(i)*-1; dq(2) = -1*dq(1); // form applied local thermal force vector
		MTBDP.MatMultVec (dTT, dq, dQ); // form global thermal force vector

		for (int i=1; i<=6;i++) // add thermal forces to applied forces
		{
			m_dSNF(nE(i),1) += dQ(i);
		}

        // debug?
        if (m_nDebugLevel == 1)
        {
            std::ostringstream szPrompt;
            szPrompt << "Stiffness Matrix for element " << i;
            PrintMatrixRowWise (dkg, szPrompt.str(), m_FileOutput);
        }
    }

    // debug?
    if (m_nDebugLevel == 1)
    {
        PrintMatrixRowWise (m_dSSM, "Structural Stiffness (Before BCs)",
                            m_FileOutput);
    }
}

void CTruss::ImposeBC ()
// ---------------------------------------------------------------------------
// Function: Imposes the essential boundary conditions
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    int i;
    int nXFC, nYFC, nZFC;
	float fidx, fidy, fidz;
    // loop thro' all nodes
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (nXFC, nYFC, nZFC);
		m_NodalData(i).GetIDisp (fidx, fidy, fidz);
        if (nXFC == 1) // if fixed in this direction
        {
            int nGDOF = 3*i-2; 
			m_nCount += 1;
            SuppressDOF (nGDOF, fidx);
        }
        if (nYFC == 1)
        {
            int nGDOF = 3*i-1;
			m_nCount += 1;
            SuppressDOF (nGDOF, fidy);
        }
		if (nZFC == 1)
        {
            int nGDOF = 3*i;
			m_nCount += 1;
            SuppressDOF (nGDOF, fidz);
        }
    }

    // debug?
    if (m_nDebugLevel == 1)
    {
        PrintMatrixRowWise (m_dSSM, "Structural Stiffness (After BCs)",
                            m_FileOutput);
        PrintMatrixColumnWise (m_dSNF, "Structural Nodal Forces (After BCs)",
                               m_FileOutput);
    }
}

void CTruss::SuppressDOF (const int nEqn, const float fc)
// ---------------------------------------------------------------------------
// Function: Imposes the essential boundary conditions
// Input:    Equation number, initial displacement
// Output:   none
// ---------------------------------------------------------------------------
{

    for (int j=1; j <= m_nDOF; j++)
    {
		// modify RHS based on initial displacement (if applicable)
		if (fc != 0.0) 	
		{
			m_dSNF(j,1) = m_dSNF(j,1) - m_dSSM(j, nEqn)*fc;
		}
        // zero out the row
        m_dSSM(nEqn, j) = 0.0;
        // zero out the column
        m_dSSM(j, nEqn) = 0.0;
    }

    // set diagonal to 1
    m_dSSM(nEqn, nEqn) = 1.0;

	// set RHS to c (if applicable)
	if (fc != 0.0) 
	{
		m_dSNF(nEqn,1) = fc;
	}
	// set RHS to zero
	else
    m_dSNF(nEqn, 1) = 0.0; 
}

void CTruss::Solve ()
// ---------------------------------------------------------------------------
// Function: Solves the system equations for the nodal displacements.
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // implement LDLT Factorization. Decompose SSM then solve for displacements.
	double TOL = 1.0e-6;
	int i,j;
	CVector<double> dVc(m_nDOF), dRes(m_nDOF), dVSNF(m_nDOF), dVSND(m_nDOF); 
	CMatrix<double> dSSMOG(m_nDOF,m_nDOF);
	dVc.Set(0.0);
    dRes.Set(0.0);
	dVSNF.Set(0.0);
	dVSND.Set(0.0);
	dSSMOG.Set(0.0);

	// create vector of specific load case for use in LDLT
	for (i=1; i<=m_nDOF; i++)
	{
		dVSNF(i) = m_dSNF(i,1);
	}

	// create copy of original matrix for use in norm
	for (i=1; i<=m_nDOF; i++)
	{
		for (j=1; j<=m_nDOF; j++)
		{
			dSSMOG(i,j) = m_dSSM(i,j);
		}
	}

	// LDLT Factorization
	if (MTBDP.LDLTFactorization (m_dSSM, TOL))
	{
		//LDLT Solve 
		if (MTBDP.LDLTSolve (m_dSSM, dVSND, dVSNF))
		{
			// calculate norm
			MTBDP.MatMultVec (dSSMOG, dVSND, dVc);
			MTBDP.Subtract (dVc, dVSNF, dRes);
			m_dNorm = MTBDP.TwoNorm (dRes);
			m_drelNorm = m_dNorm/MTBDP.TwoNorm(dVSNF);
		}
		else
			ErrorHandler (ERRORCODE::UNSTABLETRUSS);
	}
    else
        ErrorHandler (ERRORCODE::UNSTABLETRUSS);

	// populate m_dSND back to matrix form (for simplicity...)
	for (i=1; i<=m_nDOF; i++)
	{
		m_dSND(i,1) = dVSND(i);
	}

	// populate member variables
	for (i=1; i <= m_nNodes; i++)
      {
         float m_fXDisp = static_cast<float>(m_dSND(3*i-2,1));
         float m_fYDisp = static_cast<float>(m_dSND(3*i-1,1));
		 float m_fZDisp = static_cast<float>(m_dSND(3*i,1));
         m_NodalResponseData(i).SetDisplacements (m_fXDisp, m_fYDisp, m_fZDisp);
      }

   // GAUSS ELIMINATION to remain for testing purposes...

   //implement Gauss Elimination Technique. USE LDLT
   /*double TOL = 1.0e-6;
   int nError = GaussElimination (m_dSSM, m_dSND, m_dSNF, TOL); // take vector of SND and SNF for LDLT..LOAD CASES!

    if (nError != 0)
    {
        ErrorHandler (ERRORCODE::UNSTABLETRUSS);
    }
    else
    {
       for (int i=1; i <= m_nNodes; i++)
        {
            float m_fXDisp = static_cast<float>(m_dSND(2*i-1,1));
            float m_fYDisp = static_cast<float>(m_dSND(2*i,1));
           m_NodalResponseData(i).SetDisplacements (m_fXDisp, m_fYDisp);
        }
    }*/
}

void CTruss::Response ()
// ---------------------------------------------------------------------------
// Function: Computes the element response (STRAIN)
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    int i, j;
    CVector<int> nE(6); // stores the element dof
	CVector<double> dTf(6); // stores product of trans matrix and local force vector
	CVector <double> df(2); // stores f'
    CMatrix<double> dT(2,6), dTT(6,2); // transformation matrices
    CMatrix<double> dLD(2,1), dND(6,1); // nodal displacements
    float fX1, fX2, fY1, fY2, fZ1, fZ2, fL;
    float fA, fE, fCTE;
    int nSN, nEN, nXFC, nYFC, nZFC;
	
  	// begin_Michael Justice
	// dynamically allocated arrays based on fixity
	m_dA.SetSize (m_nCount, 6); 
	m_dR.SetSize (m_nCount); 
	m_dRTemp.SetSize (m_nCount);
	m_nfixStore.SetSize (m_nCount);

    // initialize
    dT.Set (0.0);
	m_dA.Set (0.0);
	m_dR.Set(0.0);
	m_nfixStore.Set(0);

    // compute reactions
	m_nCount = 0;

	// get equations and count fixities 
	for (int i=1; i <= m_nNodes; i++)
	{
		m_NodalData(i).GetFixity (nXFC, nYFC, nZFC);
		if (nXFC == 1) // if fixed in this direction
			{
				int nGDOF = 3*i-2; 
				m_nCount += 1;
				m_nfixStore(m_nCount) = nGDOF;
			}
		if (nYFC == 1)
			{
				int nGDOF = 3*i-1;
				m_nCount += 1;
				m_nfixStore(m_nCount) = nGDOF;
			}
		if (nZFC == 1)
			{
				int nGDOF = 3*i;
				m_nCount += 1;
				m_nfixStore(m_nCount) = nGDOF;
			}
	}

    // loop thro' all elements
    for (int i=1; i <= m_nElements; i++)
    {
		// initialize
		m_dRTemp.Set (0.0);	m_dA.Set (0.0); 

        // form strain, stress and force in two steps
        m_ElementData(i).GetData (nSN, nEN, fA, fE, fCTE);
        m_NodalData(nSN).GetCoords (fX1, fY1, fZ1);
        m_NodalData(nEN).GetCoords (fX2, fY2, fZ2);
        fL = sqrt((fX2-fX1)*(fX2-fX1) +
                  (fY2-fY1)*(fY2-fY1) + 
				  (fZ2-fZ1)*(fZ2-fZ1));
        // local-to-global transformation matrix
		double dl = double((fX2-fX1)/fL); 
        double dm = double((fY2-fY1)/fL);
		double dn = double((fZ2-fZ1)/fL);
        dT(1,1) = dl; dT(1,2) = dm; dT(1,3) = dn;
        dT(2,4) = dl; dT(2,5) = dm; dT(2,6) = dn;

        // get element nodal displacements
        nE(1) = 3*nSN-2; nE(2) = 3*nSN-1; nE(3) = 3*nSN;
        nE(4) = 3*nEN-2; nE(5) = 3*nEN-1; nE(6) = 3*nEN;

		// form binary matrix A for reaction computation
		for (int i=1; i <= m_nCount; i++) // rows of A
		{
			for (int j=1; j <= 6; j++) // columns of A
			{
				if (m_nfixStore(i) == nE(j))
				{
					m_dA(i,j) = 1;
				}
			}
		}

        for (j=1; j <= 6; j++)
        {
            dND(j,1) = m_dSND(nE(j),1);
        }

        // form d'=T*d
        MTBDP.Multiply (dT, dND, dLD);
		
        // strain
        float fStrain = float(dLD(2,1) - dLD(1,1))/fL - m_de(i);
        // stress
        float fStress = fE*fStrain;
        // force
        float fForce = fStress*fA;

		// form R vector
		df(1) = fForce; df(2) = -fForce;
		MTBDP.Transpose (dT, dTT);
		MTBDP.MatMultVec (dTT, df, dTf);
		MTBDP.MatMultVec (m_dA, dTf, m_dRTemp);
		MTBDP.Add (m_dRTemp, m_dR, m_dR);

        // update with the computed values
        m_ElementResponseData(i).SetData (fStrain,
                                          fStress, fForce);

		// create vector for reactions of size DOF (for printing purposes)
		m_dReac.SetSize(m_nDOF);
		m_dReac.Set (0.0);
		for (int i=1; i<=m_nDOF; i++)
		{
			for (int j=1; j<=m_nCount; j++)
			{
				if (i == m_nfixStore(j))
				{
					m_dReac(i) = -1*m_dR(j); 
				}
			}
		}

    }
}

void CTruss::CreateOutput ()
// ---------------------------------------------------------------------------
// Function: Creates the output file containing the results.
//           Currently incomplete.
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    int i;
    
    // print the problem size
    m_FileOutput << '\n';
	float fX, fY, fZ;
    m_FileOutput << "PROBLEM SIZE" << '\n';
    m_FileOutput << "------------" << '\n';
    m_FileOutput << "   Number of nodes : " << m_nNodes << '\n';
    m_FileOutput << "Number of elements : " << m_nElements << '\n';
	m_FileOutput << "     Number of DOF : " << m_nDOF-m_nCount << '\n';

	// begin_Michael Justice
    // print the nodal coordinates
    m_FileOutput << '\n';
    m_FileOutput << "NODAL COORDINATES" << '\n';
    m_FileOutput << "-----------------" << '\n';
    m_FileOutput << "Node" << "  " << std::setw(15) << "X-Coordinate" 
                           << "  " << std::setw(16) << "Y-Coordinate"
						   << "  " << std::setw(16) << "Z-Coordinate" << '\n';
    m_FileOutput << "----" << "  " << std::setw(15) << "------------"
                 << "     " << std::setw(13) << "------------"
				 << "      " << std::setw(12) << "------------" <<'\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetCoords (fX, fY, fZ);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fX << "    " 
                     << std::setw(14) << fY << "    "
					 << std::setw(14) << fZ << '\n';
    }
	// end_Michael Justice


	// begin_Michael Justice
    // print the nodal fixities
    m_FileOutput << '\n';
	int nXFC, nYFC, nZFC;
	float fidx, fidy, fidz; 
    m_FileOutput << "NODAL FIXITIES" << '\n';
    m_FileOutput << "--------------" << '\n';
    m_FileOutput << "Node" << "         " << "X-Fixity"
                           << "         " << "X-Disp" 
						   << "     " << "Y-Fixity"
						   << "         " << "Y-Disp" 
						   << "     " << "Z-Fixity" 
						   << "         " << "Z-Disp" << '\n';
    m_FileOutput << "----" << "          " <<  "-------"
						   << "        " << "-------" 
						   << "      " << "-------" 
				           << "        " << "-------" 
				           << "      " << "-------" 
				           << "        " << "-------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (nXFC, nYFC, nZFC);
		m_NodalData(i).GetIDisp (fidx, fidy, fidz);
		m_FileOutput << std::setw(4) << i << "   ";
		if (nXFC != 0 && nYFC != 0 && nZFC != 0)
		{
			m_FileOutput << std::setw(14) << "Known" 
				<< std::setw(14) << fidx 
				<< std::setw(14) << "Known" 
				<< std::setw(14) << fidy 
				<< std::setw(14) << "Known"
				<< std::setw(14) << fidz <<'\n';
		}
		else if (nXFC != 0 && nYFC == 0 && nZFC == 0)
		{
			m_FileOutput << std::setw(14) << "Known" 
				<< std::setw(14) << fidx
				<< std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Free"
				<< std::setw(14) << "" << '\n';
		}
		else if (nXFC == 0 && nYFC != 0 && nZFC == 0)
		{
			m_FileOutput << std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Known" 
				<< std::setw(14) << fidy 
				<< std::setw(14) << "Free"
				<< std::setw(14) << "" << '\n';
		}
		else if (nXFC == 0 && nYFC == 0 && nZFC != 0)
		{
			m_FileOutput << std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Free" 
				<< std::setw(14) << "" 
				<< std::setw(14) << "Known"
				<< std::setw(14) << fidz << '\n';
		}
		else if (nXFC != 0 && nYFC == 0 && nZFC != 0)
		{
			m_FileOutput << std::setw(14) << "Known" 
				<< std::setw(14) << fidx
				<< std::setw(14) << "Free" 
				<< std::setw(14) << "" 
				<< std::setw(14) << "Known"
				<< std::setw(14) << fidz << '\n';
		}
		else if (nXFC != 0 && nYFC != 0 && nZFC == 0)
		{
			m_FileOutput << std::setw(14) << "Known" 
				<< std::setw(14) << fidx
				<< std::setw(14) << "Known" 
				<< std::setw(14) << fidy
				<< std::setw(14) << "Free"
				<< std::setw(14) << "" << '\n';
		}
		else if (nXFC == 0 && nYFC != 0 && nZFC != 0)
		{
			m_FileOutput << std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Known" 
				<< std::setw(14) << fidy
				<< std::setw(14) << "Known"
				<< std::setw(14) << fidz << '\n';
		}
		else
			m_FileOutput << std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Free" 
				<< std::setw(14) << ""
				<< std::setw(14) << "Free"
				<<std::setw(14) << "" <<'\n';           
    }
	// end_Michael Justice

	// begin_Michael Justice
    // print the nodal forces
    m_FileOutput << '\n';
	float fXF, fYF, fZF, fDTemp;
    m_FileOutput << "NODAL FORCES" << '\n';
    m_FileOutput << "------------" << '\n';
    m_FileOutput << "Node" << "  " << std::setw(10) << "X-Force" 
                           << "   " << std::setw(10) <<"Y-Force" 
						   << "   " << std::setw(10) << "Z-Force" 
						   << "   " << std::setw(10) << "Delta T" << '\n';
    m_FileOutput << "----" << "   " << std::setw(9) << "-------" 
                 << "    " << std::setw(9) << "-------" 
				 << "    " << std::setw(9) << "-------"
				 << "    " << std::setw(9) << "-------"
				 << "    " << '\n';
    for (i=1; i <= m_nNodes; i++)
    {
	    m_NodalData(i).GetLoads (fXF, fYF, fZF, fDTemp);
		if (fXF != 0 || fYF != 0 || fZF != 0 || fDTemp != 0)
		{
			m_FileOutput << std::setw(4) << i << "   "
                         << std::setw(9) << fXF << "    " 
						 << std::setw(9) << fYF <<  "    "
						 << std::setw(9) << fZF << "    "
						 << std::setw(9) << fDTemp << '\n';
		}
    }

	// print the element data
    m_FileOutput << '\n';
	float fA, fE, fCTE;
	int nSN, nEN;
    m_FileOutput << "ELEMENT DATA" << '\n';
    m_FileOutput << "----------------" << '\n';
    m_FileOutput << "Element" << "    " << "SN" 
                 << "            " << "EN" << "               " << "Area"
				 << "             " << "Modulus" << "            " 
				 << "CTE" << '\n';
	m_FileOutput << "-------" << "    " << "-----" << "         " 
                 << "-----" << "            " << "-----"  
				 << "            " << "-------" 
				 << "            " << "-----" <<'\n';

	 for (i=1; i <= m_nElements; i++)
    {
		m_ElementData(i).GetData(nSN, nEN, fA, fE, fCTE);
        m_FileOutput << std::setw(4) << i << "       "
                     << nSN << "             "
                     << nEN << "                "
					 << fA <<  "              "
					 << fE <<  "           "
					 << fCTE << '\n';
    } 
	// end_Michael Justice

    // print the nodal displacements
    m_FileOutput << '\n';
    float fXDisp, fYDisp, fZDisp;
	m_FileOutput << "====================== FE RESULTS ======================" << '\n';
    m_FileOutput << "\nNODAL DISPLACEMENTS" << '\n';
    m_FileOutput << "-------------------" << '\n';
    m_FileOutput << "Node" << "   " 
				 << "X-Displacement" << "    " 
				 << "Y-Displacement" << "    "
				 << "Z-Displacement" << '\n';
    m_FileOutput << "----" << "   " << "--------------"
                 << "    " << "--------------" 
				 << "    " << "--------------" << '\n';
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalResponseData(i).GetDisplacements (fXDisp, fYDisp, fZDisp);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fXDisp 
                     << "    " << std::setw(14) << fYDisp
					 << "    " << std::setw(14) << fZDisp << '\n';
    }
    
	// begin_Michael Justice
    // print the element strain, stress and force
    m_FileOutput << '\n';
	float fStrain, fStress, fForce;
    m_FileOutput << "ELEMENT RESPONSE  (Tension is positive)" << '\n';
    m_FileOutput << "----------------" << '\n';
    m_FileOutput << "Element" << "    " << std::setw(10) << "Strain" 
                 << std::setw(13) << "Stress" << "  " << std::setw(11) << "Force" << '\n';
	m_FileOutput << "-------" << std::setw(8) << "   " << "------" << std::setw(7) << "  " 
                 << "------" << std::setw(8) << "  " << "-----" << '\n';

	 for (i=1; i <= m_nElements; i++)
    {
        m_ElementResponseData(i).GetData (fStrain, fStress, fForce);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fStrain << "   "
                     << std::setw(10) << fStress << "   "
					 << std::setw(10) << fForce << '\n';
    }

	// print the nodal reactions
    m_FileOutput << '\n';
    m_FileOutput << "NODAL REACTIONS" << '\n';
    m_FileOutput << "---------------" << '\n';
    m_FileOutput << "Node" << "    " 
				 << "X-Reaction" << "    " 
				 << "Y-Reaction" << "    " 
				 << "Z-Reaction" << '\n';
	m_FileOutput << "----" << "    "  << "---------" <<  "     "  
                 << "---------" << "     "  << "---------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (nXFC, nYFC, nZFC);
        m_FileOutput << std::setw(4) << i;
		if (nXFC != 0 && nYFC != 0 && nZFC != 0)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (nXFC != 0 && nYFC == 0 && nZFC == 0)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (nXFC == 0 && nYFC != 0 && nZFC == 0)
		{
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (nXFC == 0 && nYFC == 0 && nZFC != 0)
		{
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (nXFC != 0 && nYFC == 0 && nZFC != 0)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (nXFC != 0 && nYFC != 0 && nZFC == 0)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (nXFC == 0 && nYFC != 0 && nZFC != 0)
		{
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << "" <<'\n';
    }

	// print error norms
    m_FileOutput << '\n';
	m_FileOutput << "Absolute Error Norm: " << m_dNorm << '\n'
				 << "Relative Error Norm: " << m_drelNorm;
	// end_Michael Justice

}

void CTruss::TerminateProgram ()
// ---------------------------------------------------------------------------
// Function: Closes input and output files
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // close the input and output files
    m_FileInput.close ();
    m_FileOutput.close ();

    std::cout << "\nExecution completed successfully." << std::endl;
}

void CTruss::ErrorHandler (ERRORCODE nCode) const
// ---------------------------------------------------------------------------
// Function: Displays the error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    std::cerr << '\n';

    if (nCode == ERRORCODE::INVALIDNUMNODES)                                 // invalid number of nodes
        std::cerr << "Number of nodes must be >= 2.";
    else if (nCode == ERRORCODE::INVALIDNUMELEMENTS)                         // invalid number of elements
        std::cerr << "Number of elements must be >= 1.";
    else if (nCode == ERRORCODE::INVALIDDEBUGCODE)                           // invalid debug level
        std::cerr << "Debug level must be 0 or 1.";
    else if (nCode == ERRORCODE::INVALIDNODENUM)                             // invalid node number
        std::cerr << "Invalid node number";
    else if (nCode == ERRORCODE::INVALIDELEMENTNUM)                          // invalid element number
        std::cerr << "Invalid element number";
    else if (nCode == ERRORCODE::INVALIDCSAREA)                              // invalid x/s area
        std::cerr << "Area must be positive.";
    else if (nCode == ERRORCODE::INVALIDYM)                                  // invalid E
        std::cerr << "Modulus of elasticity must be positive.";
    else if (nCode == ERRORCODE::UNSTABLETRUSS)                              // unstable structure
        std::cerr << "Unstable truss.";
    else if (nCode == ERRORCODE::INVALIDINPUT)                               // invalid input
        std::cerr << "Input file need *end as last line.";
    else if (nCode == ERRORCODE::INVALIDNODALFIXITY)                         // invalid fixity code
        std::cerr << "Nodal fixity code must be 0 or 1.";
    else if (nCode == ERRORCODE::MISSINGEND)                                 // missing end statement
        std::cerr << "Missing *END statement in input file.";
    else if (nCode == ERRORCODE::CANNOTOPENIFILE)                            // cannot open input file
        std::cerr << "Cannot open specified input file.";
    else if (nCode == ERRORCODE::CANNOTOPENOFILE)                            // cannot open output file
        std::cerr << "Cannot open specified output file.";
    else if (nCode == ERRORCODE::INVALIDCOMMANDLINE)                         // need 1 or 3 command line arguments
        std::cerr << "Invalid number of command line arguments.";
	else if (nCode == ERRORCODE::INVALIDNODALDISP)                           // invalid displacement
		std::cerr << "Node to be displaced must have a fixity code of 1.";

    if (nCode != ERRORCODE::UNSTABLETRUSS && nCode != ERRORCODE::INVALIDCOMMANDLINE)
        std::cerr << '\n' << "Error in input file line : " 
                  << m_nLineNumber;

    std::cerr << std::endl;

    // fatal error, exit the program
    exit (1);
}