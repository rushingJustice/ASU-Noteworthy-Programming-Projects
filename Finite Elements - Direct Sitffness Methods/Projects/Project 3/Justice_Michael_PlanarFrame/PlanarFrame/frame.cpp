/*********************************************
Planar Frame Analysis Program
Copyright(c) 2000-13, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
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
    // allocate space for material data
    m_MaterialData.SetSize (m_nMatGroups);
    // allocate space for x/s data
    m_EPData.SetSize (m_nEPGroups);
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
    m_SNF.SetSize (m_nDOF, 1);       m_SNF.Set (0.0);
}

void CFrame::ConstructF ()
// ---------------------------------------------------------------------------
// Function: constructs the system load vector
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{ 
	int i,j,k;
	int nSN, nEN, nE;
	float fL, fleft, fright;
	CMatrix<float> fFNL(DOFPE,m_nElements); fFNL.Set(0.0);
	CMatrix<double> dTLG(6,6); dTLG.Set(0.0);
	CMatrix<double> dTGL(6,6); dTGL.Set(0.0); // global-local
	CVector<float> fV1(3), fV2(3), fVC1(2), fVC2(2);
	CVector<int> nD(6);
	CElementLoads::ELType Etype;


	// form local and global element load vectors
	for (i=1; i <= m_nElementLoads; i++)
    {
		// get length
		m_ElementLoadData(i).GetValues(nE, Etype, fleft, fright);
		m_ElementData(nE).GetENodes (nSN, nEN);
	    m_NodalData(nSN).GetCoords (fVC1);
        m_NodalData(nEN).GetCoords (fVC2);
		fL = sqrt((fVC2(1)-fVC1(1))*(fVC2(1)-fVC1(1)) +
                  (fVC2(2)-fVC1(2))*(fVC2(2)-fVC1(2)));
		// get equivalent nodal loads
		m_ElementLoadData(i).GetENF(fV1, fV2, fL);
		// assemble local element load vector
		for (j=1; j<=DOFPN; j++) 
		{
			m_ELL(j,nE) += fV1(j);
			m_ELL(j+3,nE) += fV2(j);
		}
	}

	// update F
	for (i=1; i <= m_nElements; i++)
	{
		m_ElementData(i).GetENodes (nSN, nEN);
	    m_NodalData(nSN).GetCoords (fVC1);
        m_NodalData(nEN).GetCoords (fVC2);
		fL = sqrt((fVC2(1)-fVC1(1))*(fVC2(1)-fVC1(1)) +
                  (fVC2(2)-fVC1(2))*(fVC2(2)-fVC1(2)));
		// local-to-global transformation matrix
        double dl = double((fVC2(1)-fVC1(1))/fL); 
        double dm = double((fVC2(2)-fVC1(2))/fL);
        dTLG(1,1) = dTLG(4,4) = dTLG(2,2) = dTLG(5,5) = dl; 
		dTLG(2,1) = dTLG(5,4) = dm;
        dTLG(1,2) = dTLG(4,5) = -dm;
		dTLG(3,3) = dTLG(6,6) = 1.0;
        // transpose of the T matrix (get global-local)
		MTBDP.Transpose(dTLG, dTGL);
		// transform element local loads to global
	    MTBDP.Multiply(dTLG,m_ELL,m_ELG);
		// update F
		for (j=1; j <= DOFPN; j++)
		{
			fFNL(j,i) =  m_ELG(j,i);
			fFNL(j+3,i) = m_ELG(j+3,i);
		}

		for (j=1; j <= DOFPE; j++)
		{
			nD(1) = 3*nSN-2; nD(2) = 3*nSN-1; nD(3) = 3*nSN;
			nD(4) = 3*nEN-2; nD(5) = 3*nEN-1; nD(6) = 3*nEN;
			int nRow = nD(j);
			m_SNF(nRow,1) += fFNL(j,i); //SNF in read file
		}
	}
}

void CFrame::ConstructK ()
// ---------------------------------------------------------------------------
// Function: constructs the system stiffness matrix
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    int nSN, nEN, nMatGrp;
	int i,j,k;
    CXSType* pXSGrp;
	std::string szID; // X/S ID tag
    float fArea, fIyy, fIzz, fS, fSF;
	float fDTempS, fDTempE; // temp changes at nodes. 
	float fL; // length of element
    CVector<float> fVC1(NDIM), fVC2(NDIM);
    CVector<CNode::Fixity> VFC1(DOFPN), VFC2(DOFPN);
	CVector<float> fVD(DOFPN); //known displacements
	CVector<int> nE(6);
	CMatrix<double> dTLG(6,6); // local-global
	CMatrix<double> dTGL(6,6); // global-local
	CMatrix<double> dkl(6,6), dkg(6,6); // element local and global SM
    CMatrix<double> dTemp(6,6);

	// initialize 
	dkl.Set(0.0);
	dTLG.Set(0.0);
	dTGL.Set(0.0);
	m_dVL.SetSize(m_nElements);

    for (int i=1; i <= m_nElements; i++)
    {
        m_ElementData(i).GetENodes (nSN, nEN);
        m_NodalData(nSN).GetCoords (fVC1);
        m_NodalData(nEN).GetCoords (fVC2);
        m_NodalData(nSN).GetFixity (VFC1,fVD);
        m_NodalData(nEN).GetFixity (VFC2,fVD);
        pXSGrp = m_ElementData(i).GetEPropertyGroup ();
        nMatGrp = m_ElementData(i).GetMatPropertyGroup ();
        pXSGrp->GetProperties(fArea, fIyy, fIzz, fS, fSF, szID);
        float fYM = m_MaterialData(nMatGrp).GetYM ();
        float fCTE = m_MaterialData(nMatGrp).GetCTE ();

		// begin Michael Justice
		fL = sqrt((fVC2(1)-fVC1(1))*(fVC2(1)-fVC1(1)) +
                  (fVC2(2)-fVC1(2))*(fVC2(2)-fVC1(2)));
        // form a,b,c,d (for use in k)
        double da = double(fArea*fYM/fL); 
			double db = double(12.0*fYM*fIzz/(fL*fL*fL));
				double dc = double(6.0*fYM*fIzz/(fL*fL));
					double dd = double(2.0*fYM*fIzz/fL);
        // construct klocal (includes axial deform)
        dkl(1,1) = dkl(4,4) = da; dkl(4,1) = dkl(1,4) = -da;
        dkl(2,2) = dkl(5,5) = db; dkl(5,2) = dkl(2,5) = -db;
        dkl(2,3) = dkl(3,2) = dkl(2,6) = dkl(6,2) = dc;
        dkl(3,5) = dkl(5,3) = dkl(5,6) = dkl(6,5) = -dc;
		dkl(3,6) = dkl(6,3) = dd;
		dkl(3,3) = dkl(6,6) = 2*dd;

        // local-to-global transformation matrix
        double dl = double((fVC2(1)-fVC1(1))/fL); 
        double dm = double((fVC2(2)-fVC1(2))/fL);
        dTLG(1,1) = dTLG(4,4) = dTLG(2,2) = dTLG(5,5) = dl; 
		dTLG(2,1) = dTLG(5,4) = dm;
        dTLG(1,2) = dTLG(4,5) = -dm;
		dTLG(3,3) = dTLG(6,6) = 1.0;
        // transpose of the T matrix (get global-local)
		MTBDP.Transpose(dTLG, dTGL);

        // form k'*T
        MTBDP.Multiply (dkl, dTGL, dTemp);
        // construct kglobal=T(T)*k'*T
		MTBDP.Multiply (dTLG, dTemp, dkg);

        // assemble into structural K (little k into big K) (nE[6])
        nE(1) = 3*nSN-2; nE(2) = 3*nSN-1; nE(3) = 3*nSN;
        nE(4) = 3*nEN-2; nE(5) = 3*nEN-1; nE(6) = 3*nEN;
		// form stiffness matrix for one element
        for (j=1; j <= 6; j++)
        {
            int nRow = nE(j);
            for (k=1; k <= 6; k++)
            {
                int nCol = nE(k);
                m_SSM(nRow, nCol) += dkg(j,k); 
            }
        }

		m_dVL(i) = fL; // store length at current element
    }
}

void CFrame::ImposeBC ()
// ---------------------------------------------------------------------------
// Function: imposes the homogenous and non-homogenous EBC
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	int i;
    CVector<CNode::Fixity> VFC(3);
	CVector<float> fVD(3); //known displacements
    // loop thro' all nodes
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
        if (VFC(1) == CNode::Fixity::SPECIFIED) 
        {
            int nGDOF = 3*i-2; 
			m_nCount += 1;
            SuppressDOF (nGDOF, fVD(1));
        }
        if (VFC(2) == CNode::Fixity::SPECIFIED)
        {
            int nGDOF = 3*i-1;
			m_nCount += 1;
            SuppressDOF (nGDOF, fVD(2));
        }
		if (VFC(3) == CNode::Fixity::SPECIFIED)
        {
            int nGDOF = 3*i;
			m_nCount += 1;
            SuppressDOF (nGDOF, fVD(3));
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
			m_SNF(j,1) = m_SNF(j,1) - m_SSM(j, nEqn)*fVD;
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
		m_SNF(nEqn,1) = fVD;
	}
	// set RHS to zero
	else
    m_SNF(nEqn, 1) = 0.0; 
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

	// create vector of specific load case for use in LDLT
	for (i=1; i<=m_nDOF; i++)
	{
		dVSNF(i) = m_SNF(i,1);
	}

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
		if (MTBDP.LDLTSolve (m_SSM, dVSND, dVSNF))
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

	// populate m_SND back to matrix form (for simplicity...)
	for (i=1; i<=m_nDOF; i++)
	{
		m_SND(i,1) = dVSND(i);
	}

	// populate member variables
	CVector<float> fV(DOFPN);
	for (i=1; i <= m_nNodes; i++)
      {
         fV(1) = static_cast<float>(m_SND(3*i-2,1));
         fV(2) = static_cast<float>(m_SND(3*i-1,1));
		 fV(3) = static_cast<float>(m_SND(3*i,1));
		 
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
	int i,j,k,l;
	CXSType* pXSGrp;
    CVector<int> nE(6); // stores the element dof
	CVector<double> dTf(6); // stores product of trans matrix and local force vector
	CVector<CNode::Fixity> VFC(3); // fixities
	CVector<float> fVD(3); // known displacements
    CVector<float> fVC1(NDIM), fVC2(NDIM); // coordinates
	CVector<float> fVFS(DOFPN), fVFE(DOFPN);
	CVector<double> dLD(6), dND(6); // nodal displacements
	CVector <double> df(6); // stores f'
	CVector<float> fVSE(NSTRESS); // stores max stress at element
	CMatrix<double> dTLG(6,6); // local-global transform
	CMatrix<double> dTGL(6,6); // global-local transform
	CMatrix<double> dkl(6,6); // element local and global SM
	CElementLoads::ELType Etype;

	std::string szID; // X/S ID tag
    float fArea, fL, fIzz, fIyy, fSF, fS;
	float fSTemp1, fSTemp2;
	float fleft, fright; // left and right values for element loads
	float fx; // increments
	int nEle; // element number
    int nSN, nEN, nMatGrp;

	
  	// begin_Michael Justice
	// dynamically allocated arrays based on fixity
	m_dA.SetSize (m_nCount, 6); 
	m_dR.SetSize (m_nCount); 
	m_dRTemp.SetSize (m_nCount);
	m_nfixStore.SetSize (m_nCount);

    // initialize
	dTLG.Set(0.0);
	dTGL.Set(0.0);
	dkl.Set(0.0);
	fVSE.Set(0.0);
	fSTemp1 = 0.0f;
	fSTemp2 = 0.0f;
	m_dA.Set (0.0);
	m_dR.Set(0.0);
	m_nfixStore.Set(0);
	m_nCount = 0;
   
	// compute reactions
	// get equations and count fixities 
	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
        if (VFC(1) == CNode::Fixity::SPECIFIED) 
        {
            int nGDOF = 3*i-2; 
			m_nCount += 1;
            m_nfixStore(m_nCount) = nGDOF;
        }
        if (VFC(2) == CNode::Fixity::SPECIFIED)
        {
            int nGDOF = 3*i-1;
			m_nCount += 1;
            m_nfixStore(m_nCount) = nGDOF;
        }
		if (VFC(3) == CNode::Fixity::SPECIFIED)
        {
            int nGDOF = 3*i;
			m_nCount += 1;
            m_nfixStore(m_nCount) = nGDOF;
        }
    }

    // loop thro' all element
    for (int i=1; i <= m_nElements; i++)
    {
		// initialize
		m_dRTemp.Set (0.0);	m_dA.Set (0.0); 

        // form stress and force in two steps
        m_ElementData(i).GetENodes (nSN, nEN);
        m_NodalData(nSN).GetCoords (fVC1);
        m_NodalData(nEN).GetCoords (fVC2);
        pXSGrp = m_ElementData(i).GetEPropertyGroup ();
        nMatGrp = m_ElementData(i).GetMatPropertyGroup ();
        pXSGrp->GetProperties(fArea, fIyy, fIzz, fS, fSF, szID);
        float fYM = m_MaterialData(nMatGrp).GetYM ();
        float fCTE = m_MaterialData(nMatGrp).GetCTE ();

		fL = sqrt((fVC2(1)-fVC1(1))*(fVC2(1)-fVC1(1)) +
                  (fVC2(2)-fVC1(2))*(fVC2(2)-fVC1(2)));

		// form a,b,c,d (for use in k)
        double da = double(fArea*fYM/fL); 
			double db = double(12.0*fYM*fIzz/(fL*fL*fL));
				double dc = double(6.0*fYM*fIzz/(fL*fL));
					double dd = double(2.0*fYM*fIzz/fL);
        // construct klocal (includes axial deform)
        dkl(1,1) = dkl(4,4) = da; dkl(4,1) = dkl(1,4) = -da;
        dkl(2,2) = dkl(5,5) = db; dkl(5,2) = dkl(2,5) = -db;
        dkl(2,3) = dkl(3,2) = dkl(2,6) = dkl(6,2) = dc;
        dkl(3,5) = dkl(5,3) = dkl(5,6) = dkl(6,5) = -dc;
		dkl(3,6) = dkl(6,3) = dd;
		dkl(3,3) = dkl(6,6) = 2*dd;

        // local-to-global transformation matrix
        double dl = double((fVC2(1)-fVC1(1))/fL); 
        double dm = double((fVC2(2)-fVC1(2))/fL);
        dTLG(1,1) = dTLG(4,4) = dTLG(2,2) = dTLG(5,5) = dl; 
		dTLG(2,1) = dTLG(5,4) = dm;
        dTLG(1,2) = dTLG(4,5) = -dm;
		dTLG(3,3) = dTLG(6,6) = 1.0;
        // transpose of the T matrix (get global-local)
		MTBDP.Transpose(dTLG, dTGL);

        nE(1) = 3*nSN-2; nE(2) = 3*nSN-1; nE(3) = 3*nSN;
        nE(4) = 3*nEN-2; nE(5) = 3*nEN-1; nE(6) = 3*nEN;

		// form binary matrix A for reaction computation
		for (int i=1; i <= m_nCount; i++) // rows of A
		{
			for (int j=1; j <= DOFPE; j++) // columns of A
			{
				if (m_nfixStore(i) == nE(j))
				{
					m_dA(i,j) = 1;
				}
			}
		}

        for (j=1; j <= DOFPE; j++)
        {
            dND(j) = m_SND(nE(j),1);
        }

        // form d'=T*d
        MTBDP.MatMultVec (dTGL, dND, dLD);
		// form f'= k'*d'
		MTBDP.MatMultVec (dkl, dLD, df);
        // remove element loads for eq. system
		for (j=1; j <= DOFPE; j++)
		{
			df(j) = df(j) - m_ELL(j,i);
			if (j > 3)
			{
				fVFE(j-3) = df(j);
			}
			else
			{
				fVFS(j) = df(j);
			}
		}

        // update with the computed values
        m_ElementResponseData(i).SetValues (fVFS, fVFE);

		// form R vector for reactions
		MTBDP.MatMultVec (dTLG, df, dTf); // check this comp
		MTBDP.MatMultVec (m_dA, dTf, m_dRTemp);
		MTBDP.Add (m_dRTemp, m_dR, m_dR);


		// stresses
		fx = 100.0; //number of cuts along element
		float fN, fM, fV, fsigtMax, ftauMax, fsigcMax, fsigMin,
			fNMin, fVMin, fMMin;
		float fPx, fPy, fwL, fwR, fcm, fPydist, fpt;
		float dlslope, sumfPx, sumfPy, 
			sumdl, sumfcm, sumPym, sumdlv, sumdlm;
		fsigtMax = fsigcMax = ftauMax =  0.0;
		fNMin = fVMin = fMMin = 0.0;
		// loop through cuts
		for (j=1; j<=fx; j++) 
		{
			fV = fN = fM = 0.0;
			dlslope = sumfPx = sumfPy = 0.0;
			sumdlm = sumfcm = sumPym = sumdlv = 0.0;
			if (j==1)
			{
				fpt = 0.0;
				j = 1;
			}
			else
				fpt = fL*j/fx;
			// loop through element loads
			for (l=1; l<=m_nElementLoads; l++)
			{
				m_ElementLoadData(l).GetValues(nEle, Etype, fleft, fright);
				if (nEle == i)
				{
					fPx = fPy = fwL = fwR = fcm = fPydist = 0.0;
					if (Etype == CElementLoads::ELType::CONCENTRATED_PX
						&& fleft < fpt)
					{
						fPx = fright;
						sumfPx += fPx;
					}
					if (Etype == CElementLoads::ELType::CONCENTRATED_PY
						&& fleft < fpt)
					{
						fPy = fright;
						fPydist = fleft;
						sumfPy += fPy;
						sumPym += -1.0*fPy*(fpt-fPydist);
					}
					if (Etype == CElementLoads::ELType::DISTRIBUTED)
					{
						fwL = fleft;
						fwR = fright;
						dlslope = (fwR-fwL)/fL;
						sumdlv += fpt*(fwL+(fwL+dlslope*fpt))/2.0;
						sumdlm += -1.0*(fpt*(fwL + (fwL+dlslope*fpt))/2.0)*
							(fpt*(2*fwL+(fwL+dlslope*fpt))/(3.0*(fwL+(fwL+dlslope*fpt))));
					}
					if (Etype == CElementLoads::ELType::CONCENTRATED_M
							&& fleft < fpt)
					{
						fcm = fright;
						sumfcm += fcm;
					} 
				}

				fN = sumfPx;
				fV = sumfPy + sumdlv; 
				fM = sumfcm + sumPym + sumdlm - fpt*fVFS(2);

				// add in forces at start node
				fN = -1.0*(fN + fVFS(1));	
				fV = -1.0*(fV + fVFS(2)); 
				fM = -1.0*(fM + fVFS(3));
			} // end element load loop

			// get max shear stress
			if (abs(fV)/fSF > ftauMax)
				ftauMax = abs(fV)/fSF;
			// get compression and tensile stress
			if ((fN/fArea - fM/fS) < 0 &&
				(fN/fArea - fM/fS) < fsigcMax)
				fsigcMax = (fN/fArea - fM/fS);
			else if ((fN/fArea - fM/fS) > 0 &&
				(fN/fArea - fM/fS) > fsigtMax)
				fsigtMax = (fN/fArea - fM/fS);

			if ((fN/fArea + fM/fS) < 0 &&
				(fN/fArea + fM/fS) < fsigcMax)
				fsigcMax = (fN/fArea + fM/fS);
			else if ((fN/fArea + fM/fS) > 0 &&
				(fN/fArea + fM/fS) > fsigtMax)
				fsigtMax = (fN/fArea + fM/fS);
		} // end cut loop

		fVSE(1) = ftauMax; // max shear stress
		fVSE(2) = fsigcMax; // comp/tensile stress
		fVSE(3) = fsigtMax; // comp/tensile stress
		m_ElementResponseData(i).SetStress (fVSE);

		// create vector for reactions of size DOF (for printing purposes)
		m_dReac.SetSize(m_nDOF);
		m_dReac.Set (0.0);
		for (int i=1; i<=m_nDOF; i++)
		{
			for (int j=1; j<=m_nCount; j++)
			{
				if (i == m_nfixStore(j))
				{
					m_dReac(i) = m_dR(j); 
				}
			}
		}
    } // end element loop
}

void CFrame::ErrorHandler (ERRORCODE nCode) const
// ---------------------------------------------------------------------------
// Function: displays error messages related to frame analysis
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
