/*********************************************
Example 13.1.2
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <assert.h>
#include <fstream>	
#include <iostream>
#include <string>
#include "xsdatabase.h"

CXSDatabase::CXSDatabase ()
{
	// initialize
	m_nSize = 0;
	// open database file
    m_IFile.open ("XSDatabase.dat", std::ios::in); 
	// load database
	if (!m_IFile)
    {
		std::cout << "Unable to open database file."
            << std::endl;
		exit (0);
	}
	// read the data
	std::string szID;
	float fArea, fSyy, fSzz, fWeight;
	CISection ISection;
	for (;;)
    {
		m_IFile >> szID >> fArea >> fSyy >> fSzz >> fWeight;
		if (m_IFile.eof()) break; // end of file reached?
		ISection.SetProperties (szID, fArea, fSyy, fSzz, fWeight);
		Add (ISection);
	}

	m_IFile.close ();
}

CXSDatabase::~CXSDatabase ()
{
}

void CXSDatabase::Add (const CISection& ISection)
{
	if ((m_nSize + 1) >= static_cast<int>(m_ListofISections.capacity()))
		m_ListofISections.resize (m_ListofISections.capacity()+2);
	m_ListofISections[m_nSize] = ISection;
	m_nSize++;
}

int CXSDatabase::Remove (const CISection& ISection)
{
	return (0);
}

CISection CXSDatabase::GetOne (int n) const
{
	assert (n >= 0 && n < m_nSize);
	return (m_ListofISections[n]);
}

int CXSDatabase::GetSize () const
{
	return m_nSize;
}