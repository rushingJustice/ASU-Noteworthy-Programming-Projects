/*********************************************
Planr Stress/Strain T6 FE Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "frame.h"
#include "matlib.h"
#include "MatToolBox.h"

CMatToolBox<float> MTB;       // single precision version
CMatToolBox<double> MTBDP;    // double precision version

/* ==================================================================
   ======================= CFrame class =============================
   ================================================================== */

CFrame::CFrame ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_nNodes = m_nElements = m_nEPGroups = m_nMatGroups
             = m_nDOF = m_nDebugLevel = m_nLineNumber
             = m_nElementLoads = m_nCount = 0;
    m_strDelimiters = "\t, "; // tab space and comma delimited file
    m_strComment    = "**";
	m_nGaussQR = 2; // set for quad rule
	m_nET = 1; // element int check
}

CFrame::~CFrame ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // deallocate ... deallocate ... deallocate
    for (int i=1; i <= m_nEPGroups; i++)
    {
        delete m_EPData(i); 
    }
}

void CFrame::Analyze ()
// ---------------------------------------------------------------------------
// Function: Reads the input data and analyzes the frame
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	// read the problem size
	ReadProblemSize ();

	// set problem size
	SetSize ();

	// read nodal and element data
	ReadFrameModel ();

	// construct system equations
	ConstructK ();
	ConstructF ();

	// impose boundary conditions
	ImposeBC ();

	// solve for the nodal displacements
	Solve ();

	// compute secondary unknowns
	Response ();
	// create output file
	CreateOutput ();

	// Close input and output files
	TerminateProgram ();
}

void CFrame::SetSize ()
// ---------------------------------------------------------------------------
// Function: memory allocation for all major arrays in the program
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // allocate space for nodal data
    m_NodalData.SetSize (m_nNodes);
    // allocate space for nodal loads data
    m_NodalLoadData.SetSize (m_nNodes);
	// allocate space for nodal loads data
    m_ElementLoadData.SetSize (m_nElementLoads);
    // allocate space for nodal response data
    m_NodalResponseData.SetSize (m_nNodes);
    // allocate space for element data
    m_ElementData.SetSize (m_nElements);
    // allocate space for element response data
    m_ElementResponseData.SetSize (m_nElements);
    // allocate space for element loads, if required
    if (m_nElementLoads > 0)
    {
        m_ELL.SetSize (DOFPE, m_nElements); m_ELL.Set(0.0f);
        m_ELG.SetSize (DOFPE, m_nElements); m_ELG.Set(0.0f);
    }

    // allocate and initialize major matrices
    m_nDOF = DOFPN*m_nNodes;
    m_SSM.SetSize (m_nDOF, m_nDOF);  m_SSM.Set (0.0);
    m_SND.SetSize (m_nDOF, 1);       m_SND.Set (0.0);
    m_SNF.SetSize (m_nDOF);       m_SNF.Set (0.0);
}

void CFrame::ConstructF ()
// ---------------------------------------------------------------------------
// Function: constructs the system load vector
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{ 
	int i,j,k;
	int nSN, nEN, nE, nSide;
	float fL, fLoad;
	int nGaussPts; // number of gauss points to use
	double dxi, deta, dw; // xi, eta mapping values and weights
	double dJ11, dJ12, dJ21, dJ22; // elements of jacobian matrix
	double dqx, dqy; // quanity for use in gauss loop
	CVector<float> fFNL(m_nDOF); fFNL.Set(0.0);
	CVector<double> dphi(6), dVEP(5); // vector to store all phi's/vector to store elem props
	CMatrix<double> dTLG(6,6); dTLG.Set(0.0);
	CMatrix<double> dTGL(6,6); dTGL.Set(0.0); // global-local
	CVector<float> fV1(3), fV2(3), fVC1(2), fVC2(2), fVLoads(3);
	CVector<int> nD(DOFPE), nVCTri(6);
	CVector<double> delphi(12);

	//initialize variables
	fVLoads.Set(0.0f);

	// set gauss quad rule and points
	if (m_nGaussQR == 1)
		nGaussPts = 1;
	if (m_nGaussQR == 2)
		nGaussPts = 3;
	if (m_nGaussQR == 3)
		nGaussPts = 3;
	if (m_nGaussQR == 4)
		nGaussPts = 4;

	// begin element load loop
	for (i=1; i <= m_nElementLoads; i++)
    {
		// begin loop on side for EQNL's
		for (k=1; k <= 3; k++)
		{
			dqx = dqy = 0.0;
			//begin GQ at each node
			for (j=1; j <= nGaussPts; j++)
			{
				if (m_nGaussQR == 1)
				{
					dxi = 1.0/3.0; 
					deta = dxi;
					dw = 1.0;
				}
				if (m_nGaussQR == 2)
				{
					if (j==1)
					{
						dxi = 2.0/3.0; deta = 1.0/6.0;
					}
					if (j==2)
					{
						dxi = 1.0/6.0; deta = 2.0/3.0;
					}
					if (j==3)
					{
						dxi = 1.0/6.0; deta = dxi;
					}
					dw = 1.0/3.0;
				}
				if (m_nGaussQR == 3)
				{
					if (j==1)
					{
						dxi = 1.0/2.0; deta = dxi;
					}
					if (j==2)
					{
						dxi = 1.0/2.0; deta = 0.0;
					}
					if (j==3)
					{
						dxi = 0.0; deta = 1.0/2.0;
					}
					dw = 1.0/3.0;
				}
				if (m_nGaussQR == 4)
				{
					if (j==1)
					{
						dxi = 1.0/3.0; deta = dxi;
						dw = (-9.0/16.0);
					}
					if (j==2)
					{
						dxi = 3.0/5.0; deta = 1.0/5.0;
					}
					if (j==3)
					{
						dxi = 1.0/5.0; deta = 3.0/5.0;
					}
					if (j==4)
					{
						dxi = 1.0/5.0; deta = 1.0/5.0;
					}
					dw = (25.0/48.0);
				}

				// transform xi, eta for proper gauss-legendre
				dxi = 0.5*(dxi+1); deta = 0.5*(deta+1);
				
				// fetch element data
				m_ElementData(i).GetEProperties(dVEP);
				m_ElementLoadData(i).GetValues(nE, nSide, fLoad);
				m_ElementData(nE).GetENodes (nVCTri);

				// set EQNL loop to proper node on side
				int nn;
				if (k==1)
				{
					if (nSide==1)
					nn=1;
					else if (nSide==2)
					nn=2;
					else if(nSide==3)
					nn=3;
				}
				else if (k==2)
				{
					if (nSide==1)
					nn=4;
					else if (nSide==2)
					nn=5;
					else if(nSide==3)
					nn=6;
				}
				else
				{
					if (nSide==1)
					nn=2;
					else if (nSide==2)
					nn=3;
					else if(nSide==3)
					nn=1;
				}
				
				// check sides
				if (nSide==1) //142
				{
					deta = 0.0; // zero because side is in xi dir.only
					// form phi vector elements for use in gauss loop
					dphi(1) = (1.0-dxi-deta)*(1.0-2.0*dxi-2.0*deta);
					dphi(4) = (4.0*dxi*(1.0-dxi-deta));
					dphi(2) = (dxi*(2.0*dxi-1.0));

					// fetch nodal data
					m_NodalData(nVCTri(1)).GetCoords (fVC1); 
					float fx1 = fVC1(1); float fy1 = fVC1(2);

					m_NodalData(nVCTri(4)).GetCoords (fVC1); 
					float fx4 = fVC1(1); float fy4 = fVC1(2);

					m_NodalData(nVCTri(2)).GetCoords (fVC1); 
					float fx2 = fVC1(1); float fy2 = fVC1(2);

					// derivatives of phi for side
					delphi(1) = (-3.0 + 4.0*deta + 4.0*dxi);
					delphi(3) = (4.0*dxi - 1.0);
					delphi(7) = (4.0 - 8.0*dxi - 4.0*deta);

					// form jacobian elements for side
					dJ11 = fx1*delphi(1) + fx2*delphi(3) + fx4*delphi(7);
					dJ12 = fy1*delphi(1) + fy2*delphi(3) + fy4*delphi(7);
					dJ21 = 0.0; dJ22 = 0.0;	

					// quantity at gauss point 
					dqx = dw*fLoad*dphi(nn)*(dJ12+dJ22);
					dqy = -dw*fLoad*dphi(nn)*(dJ11+dJ21);

					// add loads to structural nodal load vector and sum over gauss points
					m_SNF(nVCTri(nn)*2-1) += dqx*0.5*dVEP(1);
					m_SNF(nVCTri(nn)*2)   += dqy*0.5*dVEP(1);
				}
					
				else if (nSide==2) //253
				{
					// form phi vector elements for use in gauss loop
					dphi(2) = (dxi*(2.0*dxi-1.0));
					dphi(5) = (4.0*deta*dxi);
					dphi(3) = (deta*(2.0*deta-1.0));

					// fetch nodal data
					m_NodalData(nVCTri(2)).GetCoords (fVC1); 
					float fx2 = fVC1(1); float fy2 = fVC1(2);

					m_NodalData(nVCTri(5)).GetCoords (fVC1); 
					float fx5 = fVC1(1); float fy5 = fVC1(2);

					m_NodalData(nVCTri(3)).GetCoords (fVC1); 
					float fx3 = fVC1(1); float fy3 = fVC1(2);

					// derivatives of phi for side
					delphi(3) = (4.0*dxi - 1.0); delphi(4) = 0.0;
				    delphi(5) = 0.0; delphi(6) = (4.0*deta - 1.0);
					delphi(9) =  (4.0*deta); delphi(10) = (4.0*dxi);
					
					// form jacobian elements for side
					dJ11 = fx2*delphi(3) + fx5*delphi(9) + fx3*delphi(5);
					dJ12 = fy2*delphi(3) + fy5*delphi(9) + fy3*delphi(5);
					dJ21 = fx2*delphi(4) + fx5*delphi(10) + fx3*delphi(6);
					dJ22 = fy2*delphi(4) + fy5*delphi(10) + fy3*delphi(6);

					// quantity at gauss point 
					dqx = dw*fLoad*dphi(nn)*(dJ12+dJ22);
					dqy = -dw*fLoad*dphi(nn)*(dJ11+dJ21);

					// add loads to structural nodal load vector and sum over gauss points
					
					m_SNF(nVCTri(nn)*2-1) += dqx*0.5*dVEP(1);
					m_SNF(nVCTri(nn)*2)   += dqy*0.5*dVEP(1);
				}

				else if (nSide==3) //361
				{
					dxi = 0.0; // zero because side is in xi dir.only
					// form phi vector elements for use in gauss loop
					dphi(3) = (deta*(2.0*deta-1.0));
					dphi(6) = (4.0*deta*(1.0-dxi-deta));
					dphi(1) = (1.0-dxi-deta)*(1.0-2.0*dxi-2.0*deta);

					// fetch nodal data
					m_NodalData(nVCTri(3)).GetCoords (fVC1); 
					float fx3 = fVC1(1); float fy3 = fVC1(2);

					m_NodalData(nVCTri(6)).GetCoords (fVC1); 
					float fx6 = fVC1(1); float fy6 = fVC1(2);

					m_NodalData(nVCTri(1)).GetCoords (fVC1); 
					float fx1 = fVC1(1); float fy1 = fVC1(2);

					// derivatives of phi for side
					delphi(2) = (-3.0 + 4.0*deta + 4.0*dxi);
				    delphi(6) = (4.0*deta - 1.0);
					delphi(12) = (4.0 - 4.0*dxi - 8.0*deta);
					
					// form jacobian elements for side
					dJ11 = 0.0;
					dJ12 = 0.0;
					dJ21 = fx3*delphi(6) + fx6*delphi(12) + fx1*delphi(2);
					dJ22 = fy3*delphi(6) + fy6*delphi(12) + fy1*delphi(2);

					// quantity at gauss point 
					dqx = dw*fLoad*dphi(nn)*(dJ12+dJ22);
					dqy = -dw*fLoad*dphi(nn)*(dJ11+dJ21);

					// add loads to structural nodal load vector and sum over gauss points
					m_SNF(nVCTri(nn)*2-1) += dqx*0.5*dVEP(1);
					m_SNF(nVCTri(nn)*2)   += dqy*0.5*dVEP(1);
					
				}

			} // end gauss loop

		} // end equivalent load loop


   } // end element load loop

}

void CFrame::ConstructK ()
// ---------------------------------------------------------------------------
// Function: constructs the system stiffness matrix
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	int i,j,k;
    CXSType* pXSGrp;
	std::string szID; // X/S ID tag
	float fDTempS, fDTempE; // temp changes at nodes. 
	float tol = -0.1e-6;
    CVector<float> fVC1(NDIM), fVC2(NDIM);
	CVector<double> dVEP(5);
    CVector<CNode::Fixity> VFC1(DOFPN), VFC2(DOFPN);
	CVector<float> fVD(DOFPN); //known displacements
	CVector<int> nE(12), nVCTri(6);
	CVector<double> delphi(12);
	CMatrix<double> dkl(12,12), dkli(12,12), dkg(6,6); // element local and global SM
    CMatrix<double> dTemp(6,6), dJ(2,2), dJi(2,2), dR(2,2), 
		dO(3,4), dN(4,12), dB(3,12), dBT(12,3), dT(3,3), dD(3,3),
		dBTT(12,3);

	// initialize
	dD.Set(0.0);
	dO.Set(0.0);
	dN.Set(0.0);
	dJ.Set(0.0);
	m_dVL.SetSize(m_nElements);
	int nGaussPts; // number of gauss points to use
	double dxi, deta, dw; // xi, eta mapping values and weights
	double detJ;

	// set gauss quad rule and points
	if (m_nGaussQR == 1)
		nGaussPts = 1;
	if (m_nGaussQR == 2)
		nGaussPts = 3;
	if (m_nGaussQR == 3)
		nGaussPts = 3;
	if (m_nGaussQR == 4)
		nGaussPts = 4;
	
	// begin loop over elements to form k
    for (int i=1; i <= m_nElements; i++)
    {
		dkl.Set(0.0); //set to zero for proper formation of local k
		// loop over gauss pts domain
		for (int j=1; j <= nGaussPts; j++)
		{
			if (m_nGaussQR == 1)
			{
				dxi = 1.0/3.0; 
				deta = dxi;
				dw = 1.0;
			}
			if (m_nGaussQR == 2)
			{
				if (j==1)
				{
					dxi = 2.0/3.0; deta = 1.0/6.0;
				}
				if (j==2)
				{
					dxi = 1.0/6.0; deta = 2.0/3.0;
				}
				if (j==3)
				{
					dxi = 1.0/6.0; deta = dxi;
				}
				dw = 1.0/3.0;
			}
			if (m_nGaussQR == 3)
			{
				if (j==1)
				{
					dxi = 1.0/2.0; deta = dxi;
				}
				if (j==2)
				{
					dxi = 1.0/2.0; deta = 0.0;
				}
				if (j==3)
				{
					dxi = 0.0; deta = 1.0/2.0;
				}
				dw = 1.0/3.0;
			}
			if (m_nGaussQR == 4)
			{
				if (j==1)
				{
					dxi = 1.0/3.0; deta = dxi;
					dw = (-9.0/16.0);
				}
				if (j==2)
				{
					dxi = 3.0/5.0; deta = 1.0/5.0;
				}
				if (j==3)
				{
					dxi = 1.0/5.0; deta = 3.0/5.0;
				}
				if (j==4)
				{
					dxi = 1.0/5.0; deta = 1.0/5.0;
				}
				dw = (25.0/48.0);
			}

			// form delphi1,xi , delphi1,eta , etc...
			delphi(1) = (-3.0 + 4.0*deta + 4.0*dxi);
			delphi(2) = (-3.0 + 4.0*deta + 4.0*dxi);
			delphi(3) = (4.0*dxi - 1.0);
			delphi(4) = 0.0;
			delphi(5) = 0.0;
			delphi(6) = (4.0*deta - 1.0);
			delphi(7) = (4.0 - 8.0*dxi - 4.0*deta);
			delphi(8) = (-4.0*dxi);
			delphi(9) =  (4.0*deta);
			delphi(10) = (4.0*dxi);
			delphi(11) = (-4.0*deta);
			delphi(12) = (4.0 - 4.0*dxi - 8.0*deta);

			// construct Jacobian
			int n = 0;
			int m = 0;
			for (int k=1; k<=6; k++)
			{
				m = 2*k;
				n = m-1;
				// get coord info for element
				m_ElementData(i).GetENodes (nVCTri);
				m_NodalData(nVCTri(k)).GetCoords (fVC1); 

				// form derivatives of spatial varialbes x and y WRT xi, eta
				dJi(1,1) = fVC1(1)*delphi(n); 
				dJi(1,2) = fVC1(2)*delphi(n); 
				dJi(2,1) = fVC1(1)*delphi(m); 
				dJi(2,2) = fVC1(2)*delphi(m); 

				// sum
				dJ(1,1)  = dJi(1,1) + dJ(1,1);
				dJ(1,2)  = dJi(1,2) + dJ(1,2);
				dJ(2,1)  = dJi(2,1) + dJ(2,1);
				dJ(2,2)  = dJi(2,2) + dJ(2,2);
			}

			// determine det(J) 
			detJ = dJ(1,1)*dJ(2,2) - dJ(1,2)*dJ(2,1);
			if (detJ <= tol) // check for < 0 Jacobian
				ErrorHandler (ERRORCODE::ERRORMAPPING);

			// form inverse R
			dR(1,1) = dJ(2,2)/detJ;
			dR(1,2) = -dJ(1,2)/detJ;
			dR(2,1) = -dJ(2,1)/detJ;
			dR(2,2) = dJ(1,1)/detJ;

			// begin forming strain-disp matrix B
			// start with forming O
			dO(1,1) = dR(1,1); dO(1,2) = dR(1,2); dO(2,3) = dR(2,1); dO(2,4) = dR(2,2);
			dO(3,1) = dR(2,1); dO(3,2) = dR(2,2); dO(3,3) = dR(1,1); dO(3,4) = dR(1,2);

			// then N
			dN(1,1) = delphi(1); dN(1,3) = delphi(3); dN(1,5) = delphi(5); dN(1,7) = delphi(7); 
			dN(1,9) = delphi(9); dN(1,11) = delphi(11);
			dN(2,1) = delphi(2); dN(2,3) = delphi(4); dN(2,5) = delphi(6); dN(2,7) = delphi(8); 
			dN(2,9) = delphi(10); dN(2,11) = delphi(12); 

			dN(3,2) = delphi(1); dN(3,4) = delphi(3); dN(3,6) = delphi(5); dN(3,8) = delphi(7); 
			dN(3,10) = delphi(9); dN(3,12) = delphi(11);
			dN(4,2) = delphi(2); dN(4,4) = delphi(4); dN(4,6) = delphi(6); dN(4,8) = delphi(8); 
			dN(4,10) = delphi(10); dN(4,12) = delphi(12); 
			
			// form B
			MTBDP.Multiply (dO, dN, dB);

			// compute product T = w*t*det(J)*D
			m_ElementData(i).GetEProperties(dVEP);

			// Check for plane stress or plane strain config.
			if (dVEP(5) == 1.0)
			{
				dD(1,1) = 1.0; dD(1,2) = dVEP(3); dD(2,1) = dD(1,2); dD(2,2) = dD(1,1);
				dD(3,3) = (1.0-dVEP(3))/2.0;
				MTBDP.Scale(dD,dw*dVEP(1)*detJ*(dVEP(2)/(1.0-dVEP(3)*dVEP(3))));
				dT = dD;
			}
			else
			{
				dD(1,1) = (1.0-dVEP(3)); dD(1,2) = dVEP(3); dD(2,1) = dD(1,2); dD(2,2) = dD(1,1);
				dD(3,3) = (0.5-dVEP(3));
				MTBDP.Scale(dD,dw*dVEP(1)*detJ*(dVEP(2)/((1.0+dVEP(3))*(1.0-2.0*dVEP(3)))));
				dT = dD;
			}

			// form triple product B'TB
			MTBDP.Transpose(dB,dBT);
			MTBDP.Multiply(dBT,dT,dBTT);
			MTBDP.Multiply(dBTT,dB,dkli);
			MTBDP.Add(dkli,dkl,dkl);

		} //end j loop of gauss points
		MTBDP.Scale(dkl,0.5);

        // assemble into structural K (little k into big K)
        nE(1) = nVCTri(1)*2-1; nE(2) = nVCTri(1)*2; nE(3) = nVCTri(2)*2-1;
        nE(4) = nVCTri(2)*2; nE(5) = nVCTri(3)*2-1; nE(6) = nVCTri(3)*2;
		nE(7) = nVCTri(4)*2-1; nE(8) = nVCTri(4)*2; nE(9) = nVCTri(5)*2-1;
        nE(10) = nVCTri(5)*2; nE(11) = nVCTri(6)*2-1; nE(12) = nVCTri(6)*2;

        for (j=1; j <= 12; j++)
        {
            int nRow = nE(j);
            for (k=1; k <= 12; k++)
            {
                int nCol = nE(k);
                m_SSM(nRow, nCol) += dkl(j,k); 
            }
        }

    } // end element loop i
	
} // end function loop

void CFrame::ImposeBC ()
// ---------------------------------------------------------------------------
// Function: imposes the homogenous and non-homogenous EBC
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	int i;
    CVector<CNode::Fixity> VFC(2);
	CVector<float> fVD(2); //known displacements
    // loop thro' all nodes
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
        if (VFC(1) == CNode::Fixity::SPECIFIED) 
        {
            int nGDOF = 2*i-1; 
			m_nCount += 1;
            SuppressDOF (nGDOF, fVD(1));
        }
        if (VFC(2) == CNode::Fixity::SPECIFIED)
        {
            int nGDOF = 2*i;
			m_nCount += 1;
            SuppressDOF (nGDOF, fVD(2));
        }
    }
}

void CFrame::SuppressDOF (const int nEqn, const float fVD)
// ---------------------------------------------------------------------------
// Function: works in conjunction with ImposeBC
// Input:    global equation # to impose EBC & known displacements
// Output:   none
// ---------------------------------------------------------------------------
{
	for (int j=1; j <= m_nDOF; j++)
    {
		// modify RHS based on initial displacement (if applicable)
		if (fVD != 0.0) 	
		{
			m_SNF(j) = m_SNF(j) - m_SSM(j, nEqn)*fVD;
		}

        // zero out the row
        m_SSM(nEqn, j) = 0.0;
        // zero out the column
        m_SSM(j, nEqn) = 0.0;
    }

    // set diagonal to 1
    m_SSM(nEqn, nEqn) = 1.0;

	// set RHS to c (if applicable)
	if (fVD != 0.0) 
	{
		m_SNF(nEqn) = fVD;
	}
	// set RHS to zero
	else
    m_SNF(nEqn) = 0.0; 
}

void CFrame::Solve ()
// ---------------------------------------------------------------------------
// Function: solves for the nodal displacements
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

	// create copy of original matrix for use in norm
	for (i=1; i<=m_nDOF; i++)
	{
		for (j=1; j<=m_nDOF; j++)
		{
			dSSMOG(i,j) = m_SSM(i,j);
		}
	}

	// LDLT Factorization
	if (MTBDP.LDLTFactorization (m_SSM, TOL))
	{
		//LDLT Solve 
		if (MTBDP.LDLTSolve (m_SSM, dVSND, m_SNF))
		{
			// calculate norm
			MTBDP.MatMultVec (dSSMOG, dVSND, dVc);
			MTBDP.Subtract (dVc, m_SNF, dRes);
			m_dNorm = MTBDP.TwoNorm (dRes);
			m_drelNorm = m_dNorm/MTBDP.TwoNorm(dVSNF);
		}
		else
			ErrorHandler (ERRORCODE::UNSTABLETRUSS);
	}
    else
        ErrorHandler (ERRORCODE::UNSTABLETRUSS);

	// populate m_SND back to matrix form (for simplicity...)
	for (i=1; i<=m_nDOF; i++)
	{
		m_SND(i,1) = dVSND(i);
	}


	// populate member variables
	CVector<float> fV(DOFPN);
	for (i=1; i <= m_nNodes; i++)
      {
         fV(1) = static_cast<float>(m_SND(2*i-1,1));
         fV(2) = static_cast<float>(m_SND(2*i,1));
		 
         m_NodalResponseData(i).SetValues (fV);
      }
}

void CFrame::Response ()
// ---------------------------------------------------------------------------
// Function: computes the element nodal forces and support reactions
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	int i,j,k;
    CXSType* pXSGrp;
	std::string szID; // X/S ID tag
	float fDTempS, fDTempE; // temp changes at nodes. 
	float tol = -0.1e-6;
	double dDelTemp; // double value used to store temp over all nodes
	double dScaleTemp; // scale strain temp based on element type.
    CVector<float> fVC1(NDIM), fVC2(NDIM);
	CVector<double> dVEP(5), dphi(6);
	CVector<double> dVstrain(3), dVstress(3);
    CVector<CNode::Fixity> VFC1(DOFPN), VFC2(DOFPN);
	CVector<float> fVD(DOFPN); //known displacements
	CVector<int> nE(12), nVCTri(6);
	CVector<double> delphi(12);
	CMatrix<double> dkl(12,12), dkli(12,12), dkg(6,6); // element local and global SM
    CMatrix<double> dTemp(6,6), dJ(2,2), dJi(2,2), dR(2,2), 
		dO(3,4), dN(4,12), dB(3,12), dBT(12,3), dT(3,3), dD(3,3),
		dBTT(12,3);

	// initialize
	dVstrain.Set(0.0);
	dVstress.Set(0.0);
	dD.Set(0.0);
	dO.Set(0.0);
	dN.Set(0.0);
	dJ.Set(0.0);
	
	m_dVL.SetSize(m_nElements);
	int nGaussPts; // number of gauss points to use
	double dxi, deta, dw; // xi, eta mapping values and weights
	double detJ;
	

	// set gauss quad rule and points
	if (m_nGaussQR == 1)
		nGaussPts = 1;
	if (m_nGaussQR == 2)
		nGaussPts = 3;
	if (m_nGaussQR == 3)
		nGaussPts = 3;
	if (m_nGaussQR == 4)
		nGaussPts = 4;
	
	// set size of member variables strain and stress
	m_dStrain.SetSize(m_nElements*nGaussPts,3);
	m_dStress.SetSize(m_nElements*nGaussPts,3); 	
	m_dStrain.Set(0.0);
	m_dStress.Set(0.0);

	// begin loop over elements to form k
    for (int i=1; i <= m_nElements; i++)
    {
		dDelTemp = 0.0; // set to zero for proper formation
		dkl.Set(0.0); //set to zero for proper formation of local k
		// loop over gauss pts domain
		for (int j=1; j <= nGaussPts; j++)
		{
			if (m_nGaussQR == 1)
			{
				dxi = 1.0/3.0; 
				deta = dxi;
				dw = 1.0;
			}
			if (m_nGaussQR == 2)
			{
				if (j==1)
				{
					dxi = 2.0/3.0; deta = 1.0/6.0;
				}
				if (j==2)
				{
					dxi = 1.0/6.0; deta = 2.0/3.0;
				}
				if (j==3)
				{
					dxi = 1.0/6.0; deta = dxi;
				}
				dw = 1.0/3.0;
			}
			if (m_nGaussQR == 3)
			{
				if (j==1)
				{
					dxi = 1.0/2.0; deta = dxi;
				}
				if (j==2)
				{
					dxi = 1.0/2.0; deta = 0.0;
				}
				if (j==3)
				{
					dxi = 0.0; deta = 1.0/2.0;
				}
				dw = 1.0/3.0;
			}
			if (m_nGaussQR == 4)
			{
				if (j==1)
				{
					dxi = 1.0/3.0; deta = dxi;
					dw = (-9.0/16.0);
				}
				if (j==2)
				{
					dxi = 3.0/5.0; deta = 1.0/5.0;
				}
				if (j==3)
				{
					dxi = 1.0/5.0; deta = 3.0/5.0;
				}
				if (j==4)
				{
					dxi = 1.0/5.0; deta = 1.0/5.0;
				}
				dw = (25.0/48.0);
			}

			// form delphi1,xi , delphi1,eta , etc...
			delphi(1) = (-3.0 + 4.0*deta + 4.0*dxi);
			delphi(2) = (-3.0 + 4.0*deta + 4.0*dxi);
			delphi(3) = (4.0*dxi - 1.0);
			delphi(4) = 0.0;
			delphi(5) = 0.0;
			delphi(6) = (4.0*deta - 1.0);
			delphi(7) = (4.0 - 8.0*dxi - 4.0*deta);
			delphi(8) = (-4.0*dxi);
			delphi(9) =  (4.0*deta);
			delphi(10) = (4.0*dxi);
			delphi(11) = (-4.0*deta);
			delphi(12) = (4.0 - 4.0*dxi - 8.0*deta);

			// construct Jacobian
			int n = 0;
			int m = 0;
			for (int k=1; k<=6; k++)
			{
				m = 2*k;
				n = m-1;
				// get coord info for element
				m_ElementData(i).GetENodes (nVCTri);
				m_NodalData(nVCTri(k)).GetCoords (fVC1); 

				// form derivatives of spatial varialbes x and y WRT xi, eta
				dJi(1,1) = fVC1(1)*delphi(n); 
				dJi(1,2) = fVC1(2)*delphi(n); 
				dJi(2,1) = fVC1(1)*delphi(m); 
				dJi(2,2) = fVC1(2)*delphi(m); 

				// sum
				dJ(1,1)  = dJi(1,1) + dJ(1,1);
				dJ(1,2)  = dJi(1,2) + dJ(1,2);
				dJ(2,1)  = dJi(2,1) + dJ(2,1);
				dJ(2,2)  = dJi(2,2) + dJ(2,2);
			}

			// determine det(J) 
			detJ = dJ(1,1)*dJ(2,2) - dJ(1,2)*dJ(2,1);
			if (detJ <= tol) // check for < 0 Jacobian
				ErrorHandler (ERRORCODE::ERRORMAPPING);

			// form inverse R
			dR(1,1) = dJ(2,2)/detJ;
			dR(1,2) = -dJ(1,2)/detJ;
			dR(2,1) = -dJ(2,1)/detJ;
			dR(2,2) = dJ(1,1)/detJ;

			// begin forming strain-disp matrix B
			// start with forming O
			dO(1,1) = dR(1,1); dO(1,2) = dR(1,2); dO(2,3) = dR(2,1); dO(2,4) = dR(2,2);
			dO(3,1) = dR(2,1); dO(3,2) = dR(2,2); dO(3,3) = dR(1,1); dO(3,4) = dR(1,2);

			// then N
			dN(1,1) = delphi(1); dN(1,3) = delphi(3); dN(1,5) = delphi(5); dN(1,7) = delphi(7); 
			dN(1,9) = delphi(9); dN(1,11) = delphi(11);
			dN(2,1) = delphi(2); dN(2,3) = delphi(4); dN(2,5) = delphi(6); dN(2,7) = delphi(8); 
			dN(2,9) = delphi(10); dN(2,11) = delphi(12); 

			dN(3,2) = delphi(1); dN(3,4) = delphi(3); dN(3,6) = delphi(5); dN(3,8) = delphi(7); 
			dN(3,10) = delphi(9); dN(3,12) = delphi(11);
			dN(4,2) = delphi(2); dN(4,4) = delphi(4); dN(4,6) = delphi(6); dN(4,8) = delphi(8); 
			dN(4,10) = delphi(10); dN(4,12) = delphi(12); 
			
			// form B
			MTBDP.Multiply (dO, dN, dB);

			// compute product T = w*t*det(J)*D
			m_ElementData(i).GetEProperties(dVEP);

			// Check for plane stress or plane strain config.
			if (dVEP(5) == 1.0) // plane stress
			{
				dD(1,1) = 1.0; dD(1,2) = dVEP(3); dD(2,1) = dD(1,2); dD(2,2) = dD(1,1);
				dD(3,3) = (1.0-dVEP(3))/2.0;
				MTBDP.Scale(dD,dw*dVEP(1)*detJ*(dVEP(2)/(1.0-dVEP(3)*dVEP(3))));
				dT = dD;
				dScaleTemp = 1.0;
			}
			else // plane strain
			{
				dD(1,1) = (1.0-dVEP(3)); dD(1,2) = dVEP(3); dD(2,1) = dD(1,2); dD(2,2) = dD(1,1);
				dD(3,3) = (0.5-dVEP(3));
				MTBDP.Scale(dD,dw*dVEP(1)*detJ*(dVEP(2)/((1.0+dVEP(3))*(1.0-2.0*dVEP(3)))));
				dT = dD;
				dScaleTemp = (1.0+dVEP(3));
			}

			// form triple product B'TB
			MTBDP.Transpose(dB,dBT);
			MTBDP.Multiply(dBT,dT,dBTT);
			MTBDP.Multiply(dBTT,dB,dkli);
			MTBDP.Add(dkli,dkl,dkl);

			//COMPUTE STRAIN & STRESS

			// populate matrix SND into vector
			CVector<double> dVSND(18);
			for (int o=1; o<=m_nDOF; o++)  
			{
				dVSND(o) = m_SND(o,1);
			}
			// get nodal displacements for current element
			CVector<double> dVLND(12);
			for (int k=1; k<=6; k++) // remap SND to LND (ISSUE IN K?)
			{
				dVLND(k*2-1) = dVSND(nVCTri(k)*2-1); 
				dVLND(k*2) = dVSND(nVCTri(k)*2); 
			}

			MTBDP.MatMultVec(dB,dVLND,dVstrain);

			//compute strain for temperature loading
			CVector<float> fVLoads(3);
			CVector<double> dBinaryStrain(3); dBinaryStrain.Set(0.0);
			dBinaryStrain(1) = dBinaryStrain(2) = 1.0;
			dphi(1) = (1.0-dxi-deta)*(1.0-2.0*dxi-2.0*deta);
			dphi(2) = (dxi*(2.0*dxi-1.0));
			dphi(3) = (deta*(2.0*deta-1.0));
			dphi(4) = (4.0*dxi*(1.0-dxi-deta));
			dphi(5) = (4.0*deta*dxi);
			dphi(6) = (4.0*deta*(1.0-dxi-deta));

			for (int n=1; n<=m_nNodes; n++)
			{
				for (int k=1; k<=6; k++)
				{
					fVLoads.Set(0.0);
					m_NodalLoadData(n).GetValues(fVLoads);

					if (nVCTri(k) == n)
						dDelTemp += dphi(k)*fVLoads(3);
				}
			}
			
			// form complete scalar value
			dScaleTemp = dScaleTemp*dVEP(4)*dDelTemp;
			MTBDP.Scale(dBinaryStrain,dScaleTemp); // scale the binary matrix (no longer binary!)

			// form the net strain due to loading and temp
			CVector<double> dVstrainNet(3);
			MTBDP.Subtract(dVstrain,dBinaryStrain,dVstrainNet);
			

			//compute stress vector at gauss point
			MTBDP.MatMultVec(dD,dVstrainNet,dVstress);

			// loop through strains and stresses to properly allocate to member variable
			for (int m=1; m<=3; m++)
			{
				int n = 0;
				if (i==1)
				   n = 0;
				else
				   n = i*3-3;
				m_dStrain(j+n,m) = dVstrainNet(m);
				m_dStress(j+n,m) = dVstress(m);
			}

		} //end j loop of gauss points

	} // end i element loop

}

void CFrame::ErrorHandler (ERRORCODE nCode) const //not used
// ---------------------------------------------------------------------------
// Function: displays error messages related to T6 analysis
// Input:    error code
// Output:   none
// ---------------------------------------------------------------------------
{
    std::cerr << '\n';

	if (nCode == ERRORCODE::UNSTABLETRUSS) // invalid number of nodes
        std::cerr << "Unstable truss.";

    std::cerr << std::endl;
    exit (1);
}
