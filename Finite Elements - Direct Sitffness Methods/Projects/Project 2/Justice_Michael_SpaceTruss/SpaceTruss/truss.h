/*********************************************
Planar Truss Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CTruss class definition.

*********************************************/
#ifndef __RAJAN_TRUSS_H__
#define __RAJAN_TRUSS_H__

#include <fstream>
#include <iostream>
#include "..\library\vectortemplate.h"
#include "..\library\matrixtemplate.h"
#include "node.h"
#include "element.h"
#include "nodalresponse.h"
#include "elementresponse.h"


class CTruss
{
    public:
        // ctor and dtor
        CTruss ();
        ~CTruss ();
        enum class ERRORCODE {INVALIDNUMNODES, INVALIDNUMELEMENTS, INVALIDDEBUGCODE,
                              INVALIDNODENUM,  INVALIDNODALFIXITY, INVALIDELEMENTNUM,
                              INVALIDCSAREA,   INVALIDYM,          MISSINGEND,
                              UNSTABLETRUSS,   INVALIDINPUT,       INVALIDCOMMANDLINE,
		                      CANNOTOPENIFILE, CANNOTOPENOFILE, INVALIDNODALDISP}; // strongly scoped enum type!

        void Banner (std::ostream& OF) const;
        void PrepareIO (int argc, char* argv[]);
        void ReadProblemSize ();
        void ReadTrussModel ();
        void Analyze ();
        void TerminateProgram ();

    private:
        int m_nNodes;		// number of nodes
        int m_nElements;	// number of elements
        int m_nDOF;			// total degrees-of-freedom
        int m_nDebugLevel;	// debugging level
        int m_nLineNumber;	// current line number in input file
		int m_nCount;       // counter for number of fixities
		double m_dNorm;     // error norm of solution
		double m_drelNorm;  // relative error norm of solution

        // data storage for 
        CVector<CNode> m_NodalData;                     // nodal data
        CVector<CElement> m_ElementData;                // element data
        CVector<CNodalResponse> m_NodalResponseData;    // nodal response
        CVector<CElementResponse> m_ElementResponseData;// element response

        std::ifstream m_FileInput;	// File Input
        std::ofstream m_FileOutput;	// File Output

		//Multiple load cases can be added
        CMatrix<double> m_dSSM;	// structural stiffness matrix
        CMatrix<double> m_dSND;	// structural nodal displacements
        CMatrix<double> m_dSNF;	// structural nodal forces.
		CMatrix<double> m_dA; // binary matrix 
		CVector<double> m_dR; // reactions at the truss supports 
		CVector<double> m_dRTemp; // stores reaction contribution at each element
		CVector<double> m_dReac; // stores reactions at each fixity location
		CVector<double> m_de;    // thermal strain due to nodal temp changes
		CVector<int> m_nfixStore; // stores fixity nomenclature

        void SetSize ();
        void ConstructK ();
        void ImposeBC ();
        void Solve ();
        void Response ();
        void CreateOutput ();
        void SuppressDOF (const int, const float);
        void ErrorHandler (ERRORCODE) const; // handles input-related errors
};

#endif