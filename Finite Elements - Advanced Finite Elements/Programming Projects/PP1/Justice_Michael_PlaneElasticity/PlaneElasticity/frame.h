/*********************************************
Planr Stress/Strain T6 FE Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_FRAME_H__
#define __RAJAN_FRAME_H__

#include <fstream>
#include <iostream>
#include "constants.h"
#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "..\library\parser.h"
#include "node.h"
#include "element.h"
#include "material.h"
#include "xstype.h"
#include "rectsolid.h"
#include "circsolid.h"
#include "isection.h"
#include "nodalloads.h"
#include "elementloads.h"
#include "nodalresponse.h"
#include "elementresponse.h"

class CFrame
{
    public:
        CFrame ();    // ctor
        ~CFrame ();   // dtor
        enum class ERRORCODE {NUMNODES, NUMELEMENTS, DEBUGCODE,
                              NODENUMBER, ELEMENTNUMBER, XSAREA,
                              YOUNGSMODULUS, UNSTABLE, INVALIDINPUT,
                              INVALIDLASTLINE, NODALFIXITY, NUMMATGROUPS,
                              NUMEPROPGROUPS, EPROPNUMBER, XSDIMENSION,
                              MATGROUPNUMBER, MATPROPERTY, ELOADTYPE,
                              XSTYPE, INVALIDCOMMANDLINE,
		                      CANNOTOPENIFILE, CANNOTOPENOFILE, UNSTABLETRUSS,
							  ELOADDLSIGN, ELOADDIST, DUPLICATE, SIDEDNE, ELEMENTTYPE,
							  ERRORMAPPING};

        // helper functions
        void Banner (std::ostream& OF);
        void PrepareIO (int argc, char *argv[]);
        void Analyze ();

    private:
        int m_nNodes;		   // number of nodes
        int m_nElements;       // number of elements
		int m_nGaussQR;        // number of Gauss quad order (1,2,3, or 4)
        int m_nEPGroups;       // number of x/s properties
        int m_nMatGroups;      // number of material groups
        int m_nElementLoads;   // number of element loads

        int m_nDOF;			   // total degrees-of-freedom
        int m_nDebugLevel;	   // debugging level

		int m_nET;             // integer check for element type

        CVector<CNode>				m_NodalData;
        CVector<CElement>			m_ElementData;
        CVector<CMaterial>			m_MaterialData;
        CVector<CXSType*>			m_EPData;
        CVector<CNodalLoads>		m_NodalLoadData;
        CVector<CElementLoads>		m_ElementLoadData;
        CVector<CNodalResponse>		m_NodalResponseData;
        CVector<CElementResponse>	m_ElementResponseData;

        CMatrix<double> m_SSM;	 // structural stiffness matrix
        CMatrix<double> m_SND;	 // structural nodal displacements
        CVector<double> m_SNF;	 // structural nodal forces
        CMatrix<double> m_ELL;	 // element loads (local coor system)
        CMatrix<double> m_ELG;	 // element loads (global coor system)
		CMatrix<double> m_dA;    // binary matrix 
		CVector<double> m_dR;    
		CVector<double> m_dRTemp;
		CVector<double> m_nfixStore;
		CVector<double> m_dReac;
		CVector<double> m_dVL;   // stores lengths of elements 
		CMatrix<double> m_dStress; // stores stresses at each gauss point
		CMatrix<double> m_dStrain; // stores strains at each gauss point

        // input and output
        std::ifstream m_FileInput;	   // File Input
        int m_nLineNumber;	           // current line number in input file
        CParser m_Parse;               // parser for free format read
        int     m_nV;                  // integer value that is read in
        float   m_fV;                  // float value that is read in
        std::string m_strDelimiters;   // delimiters used in input file
        std::string m_strComment;      // characters to signify comment line
        std::vector<std::string> m_strVTokens; // vector to store tokens read
        int     m_nTokens;             // number of tokens read
		int     m_nCount;              // count for progress
		double  m_dNorm;               // absolute norm
		double  m_drelNorm;            // relative norm
        std::ofstream m_FileOutput;	   // File Output

        // FEA-related functions
        void ReadProblemSize ();
        void ReadFrameModel ();
        void ConstructK ();
        void ConstructF ();
        void ImposeBC ();
        void Solve ();
        void Response ();
        void CreateOutput ();
        void TerminateProgram ();
        void SuppressDOF (const int, const float);

        // modifier functions
        void SetSize ();

        // error handlers
        void ErrorHandler (ERRORCODE nCode) const; 
        void IOErrorHandler (ERRORCODE nCode) const;
};

#endif