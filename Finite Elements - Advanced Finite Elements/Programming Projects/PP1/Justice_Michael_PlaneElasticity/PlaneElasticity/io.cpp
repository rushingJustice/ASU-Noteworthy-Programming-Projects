/*********************************************
Planar Frame Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#include <vector>
#include <iomanip>
#include <sstream>
#include "frame.h"
#include "..\library\getinteractive.h"
#include "..\library\parser.h"
#include "..\library\fileio.h"

void CFrame::Banner (std::ostream& OF)
// ---------------------------------------------------------------------------
// Function: Prints program banner
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    OF << '\n';
    OF << "\t\t--------------------------------------------" << '\n';
    OF << "\t\t         T6 Elements Analysis Program       " << '\n';
    OF << "\t\t         Finite Elements for Engineers      " << '\n';
    OF << "\t\t           (c) 2000-15, S. D. Rajan         " << '\n';
    OF << "\t\t         Enhanced By: Michael Justice       " << '\n';
    OF << "\t\t--------------------------------------------" << '\n';
}

void CFrame::PrepareIO (int argc, char *argv[])
// ---------------------------------------------------------------------------
// Function: Obtains file names and opens input/output files
// Input:    command line arguments
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
    else if (argc == 3) // planarframe input_file output_file
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

void CFrame::ReadProblemSize ()
// ---------------------------------------------------------------------------
// Function: reads the size of the problem from input file but does not
//           store data
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // read the problem description
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    if (m_strVTokens[0] != "*heading")
        IOErrorHandler (ERRORCODE::INVALIDINPUT);

    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);

    // nodal coordinates
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    if (m_strVTokens[0] != "*nodal" && m_strVTokens[1] != "coordinates")
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*nodal" && m_strVTokens[1] == "fixity")
            break; // break if on to next line...otherwise count nodes
        ++m_nNodes;
    }

    // nodal fixity
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*nodal" && m_strVTokens[1] == "loads")
            break;
    }

    // nodal loads
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*element" && m_strVTokens[1] == "data")
            break;
    }

    // element data
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*element" && m_strVTokens[1] == "loads")
            break;
        ++m_nElements;
    }

    // element loads
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (m_strVTokens[0] == "*end")
                break;
        ++m_nElementLoads;
    }

    // check data for validity
    if (m_nNodes <= 1) 
        IOErrorHandler (ERRORCODE::NUMNODES);
    if (m_nElements <= 0) 
        IOErrorHandler (ERRORCODE::NUMELEMENTS);
    if (m_nDebugLevel < 0 || m_nDebugLevel > 1) // not needed?
        IOErrorHandler (ERRORCODE::DEBUGCODE);
}

void CFrame::ReadFrameModel ()
// ---------------------------------------------------------------------------
// Function: reads the rest of the frame data from input file
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    int nTag;                // stores tag
    CVector<float> fVC(2), fVC1(2), fVLoads(3);   // stores float input
	CVector<int> nVCTri(6); // stores the node numbers for T6 element

    // rewind the file to read the input file again
    Rewind (m_FileInput); // allows next line to be read as the first line
    m_nLineNumber = 0;
    
    // header line
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);

    // read nodal coordinates
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    for (int i=1; i <= m_nNodes; i++)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            if (!m_Parse.GetIntValue(m_strVTokens[0], nTag)     ||
                !m_Parse.GetFloatValue(m_strVTokens[1], fVC(1)) ||
                !m_Parse.GetFloatValue(m_strVTokens[2], fVC(2)))
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
        }
        if (nTag <= 0 || nTag > m_nNodes) 
            IOErrorHandler (ERRORCODE::NODENUMBER);
        m_NodalData(nTag).SetCoords (fVC);
    }

    // read nodal fixity conditions
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    CVector<CNode::Fixity> VFC(DOFPN);
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*nodal" && m_strVTokens[1] == "loads")
            break;

        if (!m_Parse.GetIntValue(m_strVTokens[0], nTag))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (!m_Parse.GetIntValue(m_strVTokens[0], nTag)     || 
            !m_Parse.GetFloatValue(m_strVTokens[3], fVC(1)) ||
            !m_Parse.GetFloatValue(m_strVTokens[4], fVC(2))) 
            IOErrorHandler (ERRORCODE::INVALIDINPUT);

        if (nTag <= 0 || nTag > m_nNodes) 
            IOErrorHandler (ERRORCODE::NODENUMBER);
        if (m_strVTokens[1] != "free" && m_strVTokens[1] != "known")
            IOErrorHandler (ERRORCODE::NODALFIXITY);
        if (m_strVTokens[2] != "free" && m_strVTokens[2] != "known")
            IOErrorHandler (ERRORCODE::NODALFIXITY);

        VFC(1) = (m_strVTokens[1] == "known" ? CNode::Fixity::SPECIFIED : CNode::Fixity::FREE);
        VFC(2) = (m_strVTokens[2] == "known" ? CNode::Fixity::SPECIFIED : CNode::Fixity::FREE);
        m_NodalData(nTag).SetFixity (VFC, fVC); 
    }

    // read nodal loads
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*element" && m_strVTokens[1] == "data")
            break;
        if (!m_Parse.GetIntValue(m_strVTokens[0], nTag)     ||
            !m_Parse.GetFloatValue(m_strVTokens[1], fVLoads(1)) || // x force
            !m_Parse.GetFloatValue(m_strVTokens[2], fVLoads(2)) || // y force 
            !m_Parse.GetFloatValue(m_strVTokens[3], fVLoads(3))) // temp change
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (nTag <= 0 || nTag > m_nNodes) 
            IOErrorHandler (ERRORCODE::NODENUMBER);
		m_NodalLoadData(nTag).SetValues (fVLoads);
		
		// check for duplicates THIS ALSO ASSIGNS APPLIED NODAL LOADS TO SNF!
		int nSDOF = (nTag-1)*DOFPN;
        for (int i=1; i <= DOFPN; i++)
			if (m_SNF(nSDOF+i) == 0.0)
				m_SNF(nSDOF+i) = fVLoads(i);
			else
				IOErrorHandler (ERRORCODE::DUPLICATE);
    }

    // read element data
	CElement::EType elementType;
	m_dVL.SetSize(m_nElements);
	CVector<double> dVEP(5);
    for (int i=1; i <= m_nElements; i++)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            if (!m_Parse.GetIntValue(m_strVTokens[0], nTag))
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (nTag <= 0 || nTag > m_nElements)
                IOErrorHandler (ERRORCODE::ELEMENTNUMBER);
			if (m_strVTokens[1] != "plane" && m_strVTokens[2] != "stress"
				&& m_strVTokens[2] != "strain")
				IOErrorHandler (ERRORCODE::ELEMENTTYPE);
            if (!m_Parse.GetIntValue(m_strVTokens[3], nVCTri(1))   ||
                !m_Parse.GetIntValue(m_strVTokens[4], nVCTri(2))   ||
                !m_Parse.GetIntValue(m_strVTokens[5], nVCTri(3))   ||
				!m_Parse.GetIntValue(m_strVTokens[6], nVCTri(4))   ||
                !m_Parse.GetIntValue(m_strVTokens[7], nVCTri(5))   ||
				!m_Parse.GetIntValue(m_strVTokens[8], nVCTri(6))   ||
                !m_Parse.GetDoubleValue(m_strVTokens[9], dVEP(1))  || // t
				!m_Parse.GetDoubleValue(m_strVTokens[10], dVEP(2)) || // E
				!m_Parse.GetDoubleValue(m_strVTokens[11], dVEP(3)) || // v
                !m_Parse.GetDoubleValue(m_strVTokens[12], dVEP(4)))   // CTE
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (nVCTri(1) <= 0 || nVCTri(1) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			if (nVCTri(2) <= 0 || nVCTri(2) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			if (nVCTri(3) <= 0 || nVCTri(3) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			if (nVCTri(4) <= 0 || nVCTri(4) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			if (nVCTri(5) <= 0 || nVCTri(5) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			if (nVCTri(6) <= 0 || nVCTri(6) > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
			
			// get element type and get coords
			if (m_strVTokens[2] == "stress")
			{
				dVEP(5) = 1.0;
				elementType = CElement::EType::PLANESTRESS;
			}
			else if (m_strVTokens[2] == "strain")
			{
				dVEP(5) = 2.0;
				elementType = CElement::EType::PLANESTRAIN;
			}

			m_ElementData(nTag).SetEProperties(dVEP);
			m_ElementData(nTag).SetENodes(nVCTri);
        }
    }
    // read element loads 
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    float fVELoad;
	int nELcount = 0, nSide;
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*end")
            break;
		++nELcount;
        if (!m_Parse.GetIntValue(m_strVTokens[0], nTag))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (nTag <= 0 || nTag > m_nElements) 
            IOErrorHandler (ERRORCODE::ELEMENTNUMBER);
        if (!m_Parse.GetIntValue(m_strVTokens[1], nSide) ||
            !m_Parse.GetFloatValue(m_strVTokens[2], fVELoad))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);

		if (nSide > 3 || nSide <= 0 )
			IOErrorHandler (ERRORCODE::SIDEDNE); //change error code
		m_ElementLoadData(nELcount).SetValues (nTag, 
		nSide, fVELoad);
    }
}

void CFrame::CreateOutput ()
// ---------------------------------------------------------------------------
// Function: creates the output file
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
	int nSN, nEN, nMatGrp;
	int i,j,k;
	CVector<float> fVFS(DOFPN), fVFE(DOFPN), fVSE(NSTRESS);
    CXSType* pXSGrp;
	std::string szID; // X/S ID tag
    float fArea, fIyy, fIzz, fS, fSF;
	float fLtotal; fLtotal = 0.0;
	float fDTempS, fDTempE, fDTemp; // temp changes 
    CVector<float> fVC(NDIM);
    CVector<CNode::Fixity> VFC1(DOFPN), VFC2(DOFPN), VFC(DOFPN);
	CVector<float> fVD(DOFPN); // known displacements
	CVector<float> fV(DOFPN), fVLoads(3);
	CVector<double> dVEP(5);
	CElementLoads::ELType Etype;

	//m_ElementResponseData(i).GetValues (fVFS, fVFE);
	//m_ElementResponseData(i).GetStress (fVSE);
	//pXSGrp = m_ElementData(i).GetEPropertyGroup ();
    //nMatGrp = m_ElementData(i).GetMatPropertyGroup ();
   // pXSGrp->GetProperties(fArea, fIyy, fIzz, fybar, fQ, szID, fb);
	//float fYM = m_MaterialData(nMatGrp).GetYM ();
    //float fCTE = m_MaterialData(nMatGrp).GetCTE ();
    
	//grab total length of elements
	for (i=1; i<=m_nElements; i++)
		fLtotal += m_dVL(i);

    // print the problem size
    m_FileOutput << '\n';
    m_FileOutput << "PROBLEM SIZE" << '\n';
    m_FileOutput << "------------" << '\n';
    m_FileOutput << "                  Number of nodes : " << m_nNodes << '\n';
    m_FileOutput << "               Number of elements : " << m_nElements << '\n';
	m_FileOutput << "          Number of Effective DOF : " << m_nDOF-m_nCount << '\n';

	// begin_Michael Justice
	// print the materials properties 
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT MATERIAL PROPERTIES" << '\n';
    m_FileOutput << "------------------------------------------------" << '\n';
    m_FileOutput << "Element" << "  " << std::setw(14) << "Young's Modulus" 
						   << "  " << std::setw(14) << "Poisson's Ratio"
						   << "  " << std::setw(5) << "CTE" << '\n';
    m_FileOutput << "------------------------------------------------" << '\n';

	for (i=1; i <= m_nElements; i++)
    {   
		m_ElementData(i).GetEProperties(dVEP);
        m_FileOutput << i << "  "
					 << std::setw(13) << dVEP(1) << "  "
					 << std::setw(13) << dVEP(2) << "  " 
					 << std::setw(13) << dVEP(3) << '\n';
    }

    // print the nodal coordinates
    m_FileOutput << '\n';
    m_FileOutput << "NODAL INFORMATION: COORDINATES" << '\n';
    m_FileOutput << "---------------------------------------" << '\n';
    m_FileOutput << "Node" << "  " << std::setw(14) << "X-Coordinate" 
						   << "  " << std::setw(14) << "Y-Coordinate" << '\n';
    m_FileOutput << "---------------------------------------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {   
		m_NodalData(i).GetCoords (fVC);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fVC(1) << "    " 
					 << std::setw(14) << fVC(2) << "    " << '\n';
    }

    // print the nodal fixities
    m_FileOutput << '\n';
    m_FileOutput << "NODAL BOUNDARY DATA" << '\n';
    m_FileOutput << "---------------------------------------------" 
		<< "----------------------------------------------" << '\n';
    m_FileOutput << "Node" << "         " << "X-Fixity"
                           << "         " << "X-Disp" 
						   << "     " << "Y-Fixity"
						   << "         " << "Y-Disp" 
						   << '\n';
    m_FileOutput << "---------------------------------------------" 
		<< "----------------------------------------------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
		m_FileOutput << std::setw(4) << i << "   ";
		if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << fVD(2)  <<'\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) == CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << "" << '\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << fVD(2) << '\n';
		}
		else 
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << ""  << '\n';
		}
			       
    } // end node loop

	// print the nodal loads
    m_FileOutput << '\n';
    m_FileOutput << "NODAL LOADS" << '\n';
    m_FileOutput << "-----------------------" 
		<< "-------------------" << '\n';
    m_FileOutput << "Node" << "  " << std::setw(10) << "X-Force" 
                           << "   " << std::setw(10) <<"Y-Force" 
						   << "   " << std::setw(10) << "Temp Change" << '\n';
    m_FileOutput << "-----------------------" 
		<< "-------------------" << '\n';
	fVLoads.Set(0.0);
    for (i=1; i <= m_nNodes; i++)
    {
	    m_NodalLoadData(i).GetValues (fVLoads);

		if (fVLoads(1) != 0 || fVLoads(2) != 0 || fVLoads(3) != 0) 
		{
			m_FileOutput << std::setw(4) << i << "   "
                         << std::setw(9) << fVLoads(1) << "    " 
						 << std::setw(9) << fVLoads(2) <<  "    "
						 << std::setw(9) << fVLoads(3) << '\n';
		}
    }

	// print element loads
	int nEle, nS;
	float fLoad;
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT LOADS" << '\n';
    m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';
    m_FileOutput << "Element"<< "     " << "Side"
						     << "     " << "Magnitude"  <<'\n';
	m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';

		for (j=1; j<=m_nElementLoads; j++)
		{
			m_ElementLoadData(j).GetValues(nEle, nS, fLoad);
			m_FileOutput << nEle 
						 << std::setw(18) << nS
						 << std::setw(18) << fLoad << '\n';
		}

    // print the nodal displacements
    m_FileOutput << '\n';
	m_FileOutput << "\n====================== FE RESULTS ======================" << '\n';
    m_FileOutput << "\nNODAL DISPLACEMENTS" << '\n';
    m_FileOutput << "--------------------------------------------------------" << '\n';
    m_FileOutput << "Node" << "   " 
				 << "X-Displacement" << "    " 
				 << "Y-Displacement" << "    " << '\n';
    m_FileOutput << "--------------------------------------------------------" << '\n';
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalResponseData(i).GetValues(fV);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fV(1) 
                     << "    " << std::setw(14) << fV(2) << '\n';
    }
    
    // print the element response
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT Strain Data" << '\n';
    m_FileOutput << "--------------------------------------------------------------------" << '\n';
    m_FileOutput << "Element" << "   " 
                 << std::setw(3) << "PT" << "  " 
				 << std::setw(3) << "Strain XX" << "     " 
				 << std::setw(3) << "Strain YY" << "     " 
				 << std::setw(3) << "Strain XY" << "     " << '\n';
	m_FileOutput << "--------------------------------------------------------------------" << '\n';

	 for (i=1; i <= m_nElements; i++)
    {
		m_FileOutput << '\n';
		m_FileOutput << std::setw(0) << i << "   ";

		for (int j=1; j<=3; j++) // points
		{
			m_FileOutput << '\n';
			for (int k=1; k<=3; k++)
			{
				int n=0;
				if (i==1)
					n = 0;
				else
					n = i*3-3;
	             m_FileOutput << std::setw(25) << m_dStrain(j+n,k);
			}
			
		}
    }

	 // print the element response
     m_FileOutput << '\n';
    m_FileOutput << "ELEMENT Stress Data" << '\n';
    m_FileOutput << "--------------------------------------------------------------------" << '\n';
    m_FileOutput << "Element" << "   " 
                 << std::setw(3) << "PT" << "  " 
				 << std::setw(3) << "Stress XX" << "     " 
				 << std::setw(3) << "Stress YY" << "     " 
				 << std::setw(3) << "Stress XY" << "     " << '\n';
	m_FileOutput << "--------------------------------------------------------------------" << '\n';

	 for (i=1; i <= m_nElements; i++)
    {
		m_FileOutput << '\n';
		m_FileOutput << std::setw(0) << i << "   ";

		for (int j=1; j<=3; j++) // points
		{
			m_FileOutput << '\n';
			for (int k=1; k<=3; k++)
			{
				int n=0;
				if (i==1)
					n = 0;
				else
					n = i*3-3;
	             m_FileOutput << std::setw(25) << m_dStress(j+n,k);
			}
			
		}
    }

	 
}

void CFrame::IOErrorHandler (ERRORCODE ECode) const
// ---------------------------------------------------------------------------
// Function: displays error messages related to input data
// Input:    error code
// Output:   none
// ---------------------------------------------------------------------------
{
    std::cerr << '\n';

    if (ECode == ERRORCODE::NUMNODES) // invalid number of nodes
        std::cerr << "Number of nodes must be >= 2.";
    else if (ECode == ERRORCODE::NUMELEMENTS) // invalid number of elements
        std::cerr << "Number of elements must be >= 1.";
    else if (ECode == ERRORCODE::DEBUGCODE) // invalid debug level
        std::cerr << "Debug level must be 0 or 1.";
    else if (ECode == ERRORCODE::NODENUMBER) // invalid node number
        std::cerr << "Invalid node number.";
    else if (ECode == ERRORCODE::ELEMENTNUMBER) // invalid element number
        std::cerr << "Invalid element number.";
    else if (ECode == ERRORCODE::XSAREA) // invalid x/s area
        std::cerr << "Area must be positive.";
    else if (ECode == ERRORCODE::YOUNGSMODULUS) // invalid E
        std::cerr << "Modulus of elasticity must be positive.";
    else if (ECode == ERRORCODE::INVALIDINPUT) // invalid input
        std::cerr << "Invalid input.";
    else if (ECode == ERRORCODE::INVALIDLASTLINE) // invalid input
        std::cerr << "Input file needs *end as last line.";
    else if (ECode == ERRORCODE::NODALFIXITY) // invalid fixity code
        std::cerr << "Nodal fixity code must be 'free' or 'specified'.";
    else if (ECode == ERRORCODE::NUMMATGROUPS) // invalid number of material groups
        std::cerr << "Number of material groups must be >= 1.";
    else if (ECode == ERRORCODE::NUMEPROPGROUPS) // invalid number of property groups
        std::cerr << "Number of element property groups must be >= 1.";
    else if (ECode == ERRORCODE::EPROPNUMBER) // invalid element property group
        std::cerr << "Invalid element property group number.";
    else if (ECode == ERRORCODE::XSDIMENSION) // invalid x/s dimension
        std::cerr << "Invalid cross-section dimension.";
    else if (ECode == ERRORCODE::MATGROUPNUMBER) // invalid material group
        std::cerr << "Invalid material property group number.";
    else if (ECode == ERRORCODE::MATPROPERTY) // invalid material property
        std::cerr << "Invalid material property.";
    else if (ECode == ERRORCODE::ELOADTYPE) // invalid element load type
        std::cerr << "Invalid element load type.";
    else if (ECode == ERRORCODE::XSTYPE) // invalid x/s type
        std::cerr << "Invalid cross-section type.";
	else if (ECode == ERRORCODE::ELOADDLSIGN) // invalid load values
		std::cerr << "Distributed load value at start node must be same sign as end node.";
	else if (ECode == ERRORCODE::ELOADDIST) // invalid load placement
		std::cerr << "Invalid load distance.";
	else if (ECode == ERRORCODE::DUPLICATE) // invalid load placement
		std::cerr << "Duplicate data.";
	else if (ECode == ERRORCODE::SIDEDNE) // invalid load placement
		std::cerr << "Side prescribed for element load does not exist (must be either side 1, 2 or 3).";
	else if (ECode == ERRORCODE::ELEMENTTYPE) // invalid load placement
		std::cerr << "Element must be either of type plane stress or plane strain.";
	else if (ECode == ERRORCODE::ERRORMAPPING) // invalid jacobian
		std::cerr << "Element Jacobian is less than zero.";
	
    else
        std::cerr << "Unknown error ...?";

    std::cerr << '\n' << "Error in input file line : " << m_nLineNumber;
    std::cerr << std::endl;

    exit (1);
}

void CFrame::TerminateProgram ()
// ---------------------------------------------------------------------------
// Function: terminates the program steps by closing the input/output files
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    // close the input and output files
    m_FileInput.close ();
    m_FileOutput.close ();

	std::cout << "\nExecution completed successfully." << std::endl;
}
