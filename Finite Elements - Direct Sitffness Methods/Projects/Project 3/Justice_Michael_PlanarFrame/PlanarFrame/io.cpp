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
    OF << "\t\t         Planar Frame Analysis Program      " << '\n';
    OF << "\t\tIntroduction to Structural Analysis & Design" << '\n';
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
        if (m_strVTokens[0] == "*material" && m_strVTokens[1] == "data")
            break;
    }

    // material data
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*cross-sectional" && m_strVTokens[1] == "data")
            break;
        ++m_nMatGroups;
    }

    // cross-sectional data
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*element" && m_strVTokens[1] == "data")
            break;
        ++m_nEPGroups;
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
    if (m_nMatGroups <= 0) 
        IOErrorHandler (ERRORCODE::NUMMATGROUPS); 
    if (m_nEPGroups <= 0) 
        IOErrorHandler (ERRORCODE::NUMEPROPGROUPS); 
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
    CVector<float> fVC(3), fVC1(3), fVC2(3);   // stores float input

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
            !m_Parse.GetFloatValue(m_strVTokens[4], fVC(1)) ||
            !m_Parse.GetFloatValue(m_strVTokens[5], fVC(2)) ||
            !m_Parse.GetFloatValue(m_strVTokens[6], fVC(3)))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);

        if (nTag <= 0 || nTag > m_nNodes) 
            IOErrorHandler (ERRORCODE::NODENUMBER);
        if (m_strVTokens[1] != "free" && m_strVTokens[1] != "specified")
            IOErrorHandler (ERRORCODE::NODALFIXITY);
        if (m_strVTokens[2] != "free" && m_strVTokens[2] != "specified")
            IOErrorHandler (ERRORCODE::NODALFIXITY);
        if (m_strVTokens[3] != "free" && m_strVTokens[3] != "specified")
            IOErrorHandler (ERRORCODE::NODALFIXITY);

        VFC(1) = (m_strVTokens[1] == "specified" ? CNode::Fixity::SPECIFIED : CNode::Fixity::FREE);
        VFC(2) = (m_strVTokens[2] == "specified" ? CNode::Fixity::SPECIFIED : CNode::Fixity::FREE);
        VFC(3) = (m_strVTokens[3] == "specified" ? CNode::Fixity::SPECIFIED : CNode::Fixity::FREE);
        m_NodalData(nTag).SetFixity (VFC, fVC); // here
    }

    // read nodal loads
    for (;;)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (m_strVTokens[0] == "*material" && m_strVTokens[1] == "data")
            break;
        if (!m_Parse.GetIntValue(m_strVTokens[0], nTag)     ||
            !m_Parse.GetFloatValue(m_strVTokens[1], fVC(1)) ||
            !m_Parse.GetFloatValue(m_strVTokens[2], fVC(2)) ||
            !m_Parse.GetFloatValue(m_strVTokens[3], fVC(3)))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (nTag <= 0 || nTag > m_nNodes) 
            IOErrorHandler (ERRORCODE::NODENUMBER);
		m_NodalLoadData(nTag).SetValues (fVC);
		int nSDOF = (nTag-1)*DOFPN;
        for (int i=1; i <= DOFPN; i++)
			if (m_SNF(nSDOF+i, 1) == 0.0)
				m_SNF(nSDOF+i, 1) = fVC(i);
			else
				IOErrorHandler (ERRORCODE::DUPLICATE);
		// formation of SNF moved to ConstructF
    }

    // read material data
    for (int i=1; i <= m_nMatGroups; i++)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            if (!m_Parse.GetIntValue(m_strVTokens[0], nTag)     ||
                !m_Parse.GetFloatValue(m_strVTokens[1], fVC(1)) ||
				!m_Parse.GetFloatValue(m_strVTokens[2], fVC(2)) ||
				!m_Parse.GetFloatValue(m_strVTokens[3], fVC(3)))
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (fVC(1) <= 0.0f || fVC(2) <= 0.0f || fVC(3) <= 0.0f)
                IOErrorHandler (ERRORCODE::MATPROPERTY);
        }
        if (nTag <= 0 || nTag > m_nMatGroups)
            IOErrorHandler (ERRORCODE::MATGROUPNUMBER);
        m_MaterialData(nTag).SetYM (fVC(1));
		m_MaterialData(nTag).Setv (fVC(2));
		m_MaterialData(nTag).SetCTE(fVC(3));
    }

    // read cross-sectional data
    std::string szTag;
    CVector<float> fVXSDims(MAXEPDIM);
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    for (int i=1; i <= m_nEPGroups; i++)
    {
        if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                                m_nTokens, m_strDelimiters, m_strComment))
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        else
        {
            if (!m_Parse.GetIntValue(m_strVTokens[0], nTag))
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (nTag < 0 || nTag > m_nEPGroups) 
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (m_strVTokens[1] == "rects")
            {
                if (!m_Parse.GetFloatValue(m_strVTokens[2], fVXSDims(1)) ||
                    !m_Parse.GetFloatValue(m_strVTokens[3], fVXSDims(2)))
                    IOErrorHandler (ERRORCODE::INVALIDINPUT);
                if (fVXSDims(1) <= 0.0f || fVXSDims(2) <= 0.0f)
                    IOErrorHandler (ERRORCODE::XSDIMENSION);
                m_EPData(nTag) = new CRectSolid (fVXSDims);
            }
            else if (m_strVTokens[1] == "isection")
            {
                if (!m_Parse.GetFloatValue(m_strVTokens[2], fVXSDims(1)) ||
                    !m_Parse.GetFloatValue(m_strVTokens[3], fVXSDims(2)) ||
                    !m_Parse.GetFloatValue(m_strVTokens[4], fVXSDims(3)) ||
                    !m_Parse.GetFloatValue(m_strVTokens[5], fVXSDims(4)))
                    IOErrorHandler (ERRORCODE::INVALIDINPUT);
                if (fVXSDims(1) <= 0.0f || fVXSDims(2) <= 0.0f ||
                    fVXSDims(3) <= 0.0f || fVXSDims(4) <= 0.0f)
                    IOErrorHandler (ERRORCODE::XSDIMENSION);
                m_EPData(nTag) = new CISection (fVXSDims);
            }
			else if (m_strVTokens[1] == "circs")
			{
				 if (!m_Parse.GetFloatValue(m_strVTokens[2], fVXSDims(1)))
					 IOErrorHandler (ERRORCODE::INVALIDINPUT);
				 if (fVXSDims(1) <= 0.0f)
					 IOErrorHandler (ERRORCODE::XSDIMENSION);
				 m_EPData(nTag) = new CCircSolid (fVXSDims);
			}
            else // add circs
                IOErrorHandler (ERRORCODE::XSTYPE);
        }
    }

    // read element data
	m_dVL.SetSize(m_nElements);
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    int nSN, nEN, nMatGrp, nEPGrp;
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
            if (!m_Parse.GetIntValue(m_strVTokens[1], nSN)     ||
                !m_Parse.GetIntValue(m_strVTokens[2], nEN)     ||
                !m_Parse.GetIntValue(m_strVTokens[3], nMatGrp) ||
                !m_Parse.GetIntValue(m_strVTokens[4], nEPGrp)) 
                IOErrorHandler (ERRORCODE::INVALIDINPUT);
            if (nMatGrp <= 0 || nMatGrp > m_nMatGroups)
                IOErrorHandler (ERRORCODE::MATGROUPNUMBER);
            if (nEPGrp <= 0 || nEPGrp > m_nEPGroups)
                IOErrorHandler (ERRORCODE::EPROPNUMBER);
            if (nSN <= 0 || nSN > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
            if (nEN <= 0 || nEN > m_nNodes)
                IOErrorHandler (ERRORCODE::NODENUMBER);
            m_ElementData(nTag).SetMatPropertyGroup (nMatGrp);
            m_ElementData(nTag).SetEPropertyGroup (m_EPData(nEPGrp));
            m_ElementData(nTag).SetENodes (nSN, nEN);
			
			// get element length and store for ELOAD checks
			m_NodalData(nSN).GetCoords(fVC1);
			m_NodalData(nEN).GetCoords(fVC2);
			m_dVL(nTag) = sqrt((fVC2(1)-fVC1(1))*(fVC2(1)-fVC1(1)) +
                  (fVC2(2)-fVC1(2))*(fVC2(2)-fVC1(2)));
        }
    }
    // read element loads 
    if (!m_Parse.GetTokens (m_FileInput, m_nLineNumber, m_strVTokens, 
                            m_nTokens, m_strDelimiters, m_strComment))
        IOErrorHandler (ERRORCODE::INVALIDINPUT);
    std::string szEType;
	CVector<CElementLoads::ELType> Etype(4);
	Etype(1) = CElementLoads::ELType::DISTRIBUTED;
    Etype(2) = CElementLoads::ELType::CONCENTRATED_PX;
	Etype(3) = CElementLoads::ELType::CONCENTRATED_PY;
	Etype(4) = CElementLoads::ELType::CONCENTRATED_M;
    CVector<float> fVELoads(2);
	int nELcount = 0;
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
        szEType = m_strVTokens[1]; // type of load
        if (!m_Parse.GetFloatValue(m_strVTokens[2], fVELoads(1)) ||
            !m_Parse.GetFloatValue(m_strVTokens[3], fVELoads(2))) // one too many..
            IOErrorHandler (ERRORCODE::INVALIDINPUT);
        if (szEType != "dly'" && szEType != "ploadx'" &&
            szEType != "ploady'" && szEType != "cmoment")
            IOErrorHandler (ERRORCODE::ELOADTYPE);
		if (szEType == "dly'")
		{
			if (fVELoads(1) > 0.0 && fVELoads(2) < 0.0 ||
				fVELoads(1) < 0.0 && fVELoads(2) > 0.0)
				IOErrorHandler (ERRORCODE::ELOADDLSIGN);
			m_ElementLoadData(nELcount).SetValues (nTag, 
			Etype(1), fVELoads(1), fVELoads(2));
		}
		else if (szEType == "ploadx'")
		{
			if (fVELoads(1) <= 0.0 || fVELoads(1) >= m_dVL(nTag))
				IOErrorHandler (ERRORCODE::ELOADDIST);
			m_ElementLoadData(nELcount).SetValues (nTag, 
			Etype(2), fVELoads(1), fVELoads(2));
        }
		else if (szEType == "ploady'")
		{
			if (fVELoads(1) <= 0.0 || fVELoads(1) >= m_dVL(nTag))
				IOErrorHandler (ERRORCODE::ELOADDIST);
			m_ElementLoadData(nELcount).SetValues (nTag, 
			Etype(3), fVELoads(1), fVELoads(2));
        }
		else
		{
			if (fVELoads(1) <= 0.0 || fVELoads(1) >= m_dVL(nTag))
				IOErrorHandler (ERRORCODE::ELOADDIST);
			m_ElementLoadData(nELcount).SetValues (nTag, 
			Etype(4), fVELoads(1), fVELoads(2));
        }
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
	CVector<float> fVD(DOFPN); //known displacements
	CVector<float> fV(DOFPN);
	CElementLoads::ELType Etype;
	float fL,fR;

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
	m_FileOutput << "        Number of Material Groups : " << m_nMatGroups << '\n';
	m_FileOutput << "Number of Element Property Groups : " << m_nEPGroups << '\n';
	m_FileOutput << "            Length of all members : " << fLtotal << '\n';
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
		nMatGrp = m_ElementData(i).GetMatPropertyGroup ();
		float fYM = m_MaterialData(nMatGrp).GetYM();
		float fv = m_MaterialData(nMatGrp).Getv();
		float fCTE = m_MaterialData(nMatGrp).GetCTE();
        m_FileOutput << i << "  "
                     << std::setw(15) << fYM << "  " 
					 << std::setw(13) << fv << "  " 
					 << std::setw(13) << fCTE << '\n';
    }

	// print cross-sectional properties
	m_FileOutput << '\n';
    m_FileOutput << "ELEMENT CROSS-SECTIONAL PROPERTIES" << '\n';
    m_FileOutput << "------------------------------------------------"
				 << "-------------------------------------" <<'\n';
    m_FileOutput << "Element" << std::setw(14) << "Type" 
							<< std::setw(12) << "Area" 
						    << std::setw(20) << "Moment of Inertia"
						    << std::setw(14) << "Shear Factor"
						    << std::setw(18) << "Section Modulus" << '\n';
    m_FileOutput << "------------------------------------------------"
				 << "-------------------------------------" <<'\n';

	for (i=1; i <= m_nElements; i++)
    {   
		pXSGrp = m_ElementData(i).GetEPropertyGroup ();
		pXSGrp->GetProperties(fArea, fIyy, fIzz, fS, fSF, szID);
        m_FileOutput << i << "      "
                     << std::setw(15) << szID 
					 << std::setw(10) << fArea 
					 << std::setw(15) << fIzz 
					 << std::setw(15) << fSF 
					 << std::setw(15) << fS << '\n';
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
    m_FileOutput << "NODAL INFORMATION: FIXITIES" << '\n';
    m_FileOutput << "---------------------------------------------" 
		<< "----------------------------------------------" << '\n';
    m_FileOutput << "Node" << "         " << "X-Fixity"
                           << "         " << "X-Disp" 
						   << "     " << "Y-Fixity"
						   << "         " << "Y-Disp" 
						   << "     " << "Z-Fixity" 
						   << "         " << "Z-Rot" << '\n';
    m_FileOutput << "---------------------------------------------" 
		<< "----------------------------------------------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
		m_FileOutput << std::setw(4) << i << "   ";
		if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE 
			&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << fVD(2)  
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << fVD(3)  <<'\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) == CNode::Fixity::FREE 
			&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << "" << '\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE 
			&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << fVD(2) 
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << "" << '\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
			&& VFC(2) == CNode::Fixity::FREE 
			&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << "" 
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << fVD(3)  << '\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) == CNode::Fixity::FREE 
			&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1) 
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2) 
				<< std::setw(14) << "" 
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << fVD(3)  << '\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE 
			&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1)
				<< std::setw(14) << fVD(1) 
				<< std::setw(14) << VFC(2)
				<< std::setw(14) << fVD(2) 
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << "" << '\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE 
			&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << std::setw(14) << VFC(1)
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2)
				<< std::setw(14) << fVD(2) 
				<< std::setw(14) << VFC(3)
				<< std::setw(14) << fVD(3)  << '\n';
		}
		else
			m_FileOutput << std::setw(14) << VFC(1)
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(2)
				<< std::setw(14) << ""
				<< std::setw(14) << VFC(3)
				<<std::setw(14) << "" <<'\n';
			       
    }

	// print the element data
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT INFORMATION" << '\n';
    m_FileOutput << "----------------------------------------------" << '\n';
    m_FileOutput << "Element"<< "     " << "Start Node"
						     << "     " << "End Node" 
							 << "     " << "Length"  <<'\n';
	m_FileOutput << "----------------------------------------------" << '\n';

	for (i=1; i <= m_nElements; i++)
    {
		m_ElementData(i).GetENodes(nSN,nEN);
        m_FileOutput << i 
                     << std::setw(15) << nEN 
                     << std::setw(15)  << nSN 
					 << std::setw(15) << m_dVL(i) 
					 << "           " <<  '\n';
    } 

	// print the nodal loads
    m_FileOutput << '\n';
    m_FileOutput << "NODAL LOADS" << '\n';
    m_FileOutput << "-----------------------" 
		<< "-------------------" << '\n';
    m_FileOutput << "Node" << "  " << std::setw(10) << "X-Force" 
                           << "   " << std::setw(10) <<"Y-Force" 
						   << "   " << std::setw(10) << "Z-Moment" << '\n';
    m_FileOutput << "-----------------------" 
		<< "-------------------" << '\n';
    for (i=1; i <= m_nNodes; i++)
    {
	    m_NodalLoadData(i).GetValues (fV);
		if (fV(1) != 0 || fV(2) != 0 || fV(3) != 0) // add fV(4) != 0 here too (temp change at node)
		{
			m_FileOutput << std::setw(4) << i << "   "
                         << std::setw(9) << fV(1) << "    " 
						 << std::setw(9) << fV(2) <<  "    "
						 << std::setw(9) << fV(3) << '\n';
		}
    }

	// print the ECL
	int nEle;
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT CONCENTRATED LOADS" << '\n';
    m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';
    m_FileOutput << "Element"<< "     " << "Load Type"
						     << "     " << "Dist from Start Node" 
							 << "     " << "Load Intensity"  <<'\n';
	m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';

	for (i=1; i <= m_nElements; i++)
    {
		for (j=1; j<=m_nElementLoads; j++)
		{
			m_ElementLoadData(j).GetValues(nEle, Etype, fL,fR);
			if (nEle != i)
				continue;
			if (Etype == CElementLoads::ELType::CONCENTRATED_PX)
				szID = "LOCAL X";
			else if (Etype == CElementLoads::ELType::CONCENTRATED_PY)
				szID = "LOCAL Y";
			else if (Etype == CElementLoads::ELType::CONCENTRATED_M)
				szID = "MOMENT";
			else
				continue;
			m_FileOutput << i 
						 << std::setw(18) << szID
						 << std::setw(18) << fL 
						<< std::setw(18) << fR << '\n';
		}
    } 

	// print the EDL
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT DISTRIBUTED LOADS" << '\n';
    m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';
    m_FileOutput << "Element"<< "     " << "Load Type"
						     << "     " << "Int at Start Node" 
							 << "     " << "Int at End Node"  <<'\n';
	m_FileOutput << "----------------------------------------------" 
		<< "-------------------" << '\n';

	for (i=1; i <= m_nElements; i++)
    {
		for (j=1; j<=m_nElementLoads; j++)
		{
			m_ElementLoadData(j).GetValues(nEle, Etype, fL,fR);
			if (nEle != i)
				continue;
			if (Etype == CElementLoads::ELType::DISTRIBUTED)
				szID = "LOCAL Y";
			else
				continue;
			m_FileOutput << i 
						 << std::setw(18) << szID
						 << std::setw(18) << fL 
						<< std::setw(18) << fR << '\n';
		}
    } 
	// end_Michael Justice

    // print the nodal displacements
    m_FileOutput << '\n';
	m_FileOutput << "\n====================== FE RESULTS ======================" << '\n';
    m_FileOutput << "\nNODAL DISPLACEMENTS" << '\n';
    m_FileOutput << "--------------------------------------------------------" << '\n';
    m_FileOutput << "Node" << "   " 
				 << "X-Displacement" << "    " 
				 << "Y-Displacement" << "    "
				 << "Z-Rotation" << '\n';
    m_FileOutput << "--------------------------------------------------------" << '\n';
    for (i=1; i <= m_nNodes; i++)
    {
        m_NodalResponseData(i).GetValues(fV);
        m_FileOutput << std::setw(4) << i << "   "
                     << std::setw(14) << fV(1) 
                     << "    " << std::setw(14) << fV(2)
					 << "    " << std::setw(14) << fV(3) << '\n';
    }
    
	// begin_Michael Justice
    // print the element response
    m_FileOutput << '\n';
    m_FileOutput << "ELEMENT NODAL FORCES" << '\n';
    m_FileOutput << "--------------------------------------------------------" << '\n';
    m_FileOutput << "Element" << "   " 
                 << std::setw(14) << "Axial Force" << "  " 
				 << std::setw(14) << "Shear Force" << "  " 
				 << std::setw(14) << "Bending Moment" << '\n';
	m_FileOutput << "--------------------------------------------------------" << '\n';

	 for (i=1; i <= m_nElements; i++)
    {
        m_ElementResponseData(i).GetValues (fVFS, fVFE);
        m_FileOutput << std::setw(3) << i << "   "
                     << std::setw(14) << fVFS(1) << "   "
                     << std::setw(14) << fVFS(2) << "   "
					 << std::setw(14) << fVFS(3) << '\n'
					 << std::setw(20) << fVFE(1) << "   "
					 << std::setw(14) << fVFE(2) << "   "
					 << std::setw(14) << fVFE(3) << '\n';
    }

	  // print max member stresses
    m_FileOutput << '\n';
    m_FileOutput << "MAX MEMBER STRESSES" << '\n';
    m_FileOutput << "-----------------------------------------------"
		"--------------"<< '\n';
    m_FileOutput << "Element" << "    " 
                 << std::setw(14) << "Compressive Stress" << "  " 
				 << std::setw(14) << "Tensile Stress" << "  " 
				 << std::setw(14) << "Shear Stress"  << '\n';
	m_FileOutput << "-----------------------------------------------"
		"--------------"<< '\n';

	 for (i=1; i <= m_nElements; i++)
    {
        m_ElementResponseData(i).GetStress(fV);
        m_FileOutput << i << "         "
                     << std::setw(14) << abs(fV(2)) << "   "
                     << std::setw(14) << fV(3) << "   "
					 << std::setw(14) << fV(1) << '\n';
    }

	// print the nodal reactions
    m_FileOutput << '\n';
    m_FileOutput << "NODAL REACTIONS" << '\n';
    m_FileOutput << "----------------------------------------------" << '\n';
    m_FileOutput << "Node" << "    " 
				 << "X-Reaction" << "    " 
				 << "Y-Reaction" << "    " 
				 << "Z-Reaction" << '\n';
	m_FileOutput << "----------------------------------------------" << '\n';

	for (i=1; i <= m_nNodes; i++)
    {
        m_NodalData(i).GetFixity (VFC, fVD);
        m_FileOutput << std::setw(4) << i;
		if (VFC(1) != CNode::Fixity::FREE 
			&& VFC(2) != CNode::Fixity::FREE 
			&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
				&& VFC(2) == CNode::Fixity::FREE 
				&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
				&& VFC(2) != CNode::Fixity::FREE 
				&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
				&& VFC(2) == CNode::Fixity::FREE 
				&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << ""
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
				&& VFC(2) == CNode::Fixity::FREE 
				&& VFC(3) != CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << ""
			<< "    " << std::setw(9) << m_dReac(3*i) <<'\n';
		}
		else if (VFC(1) != CNode::Fixity::FREE 
				&& VFC(2) != CNode::Fixity::FREE 
				&& VFC(3) == CNode::Fixity::FREE)
		{
			m_FileOutput << "    " << std::setw(10) << m_dReac(3*i-2) 
			<< "    " << std::setw(9) << m_dReac(3*i-1) 
			<< "    " << std::setw(9) << "" <<'\n';
		}
		else if (VFC(1) == CNode::Fixity::FREE 
				&& VFC(2) != CNode::Fixity::FREE 
				&& VFC(3) != CNode::Fixity::FREE)
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
	// end_Michael Justice*/
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
