/*********************************************
Example 13.3.4
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#include <iostream>
#include "Material.h"

double CMaterialSI::m_dVConvFactor[LASTONE] = {0.00194032, 0.0208865, 0.0208865};
double CMaterialUSC::m_dVConvFactor[LASTONE] = {515.379, 47.8779, 47.8779};

CMaterial::CMaterial()
{
    m_dMassDensity = m_dYieldStrength =
    m_dModulusOfElasticity = 0.0;
}

CMaterial::~CMaterial()
{
}

void CMaterial::Display () const
{
    std::cout << "Name:              " << m_szName         
              << "\nMass Density:      " << m_dMassDensity
              << "\nYield Strength:    " << m_dYieldStrength
              << "\nMod of Elasticity: " << m_dModulusOfElasticity
              << '\n';
}

void CMaterial::SetName (const std::string& szName)
{
    m_szName = szName;
}

void CMaterial::SetMassDensity (double dMassDensity)
{
    m_dMassDensity = dMassDensity;
}

void CMaterial::SetYieldStrength (double dYieldStrength)
{
    m_dYieldStrength = dYieldStrength;
}

void CMaterial::SetModulusOfElasticity (double dModulusOfElasticity)
{
    m_dModulusOfElasticity = dModulusOfElasticity;
}

void CMaterial::GetName (std::string& szName) const
{
    szName = m_szName;
}

double CMaterial::GetMassDensity () const
{
    return m_dMassDensity;
}

double CMaterial::GetYieldStrength () const
{
    return m_dYieldStrength;
}

double CMaterial::GetModulusOfElasticity () const
{
    return m_dModulusOfElasticity;
}

// --------------------------------------------------------------
CMaterialSI::CMaterialSI ()
{
}

CMaterialSI::~CMaterialSI ()
{
}

CMaterialSI::CMaterialSI (const std::string& szName, double dMassDensity, 
                          double dYieldStrength, double dModulusOfElasticity)
{
    SetName (szName);
    SetMassDensity (dMassDensity);
    SetYieldStrength (dYieldStrength);
    SetModulusOfElasticity (dModulusOfElasticity);
}

double CMaterialSI::CConvert::GetValue (const CMaterialSI& MO, 
                                        enum CMaterial::PROPERTY P)
{
    if (P == MASSDENSITY)
        return (MO.GetMassDensity()*m_dVConvFactor[MASSDENSITY]);
    else if (P == YIELDSTRENGTH)
        return (MO.GetYieldStrength()*m_dVConvFactor[YIELDSTRENGTH]);
    else if (P == MODULUSOFELASTICITY)
        return (MO.GetModulusOfElasticity()*m_dVConvFactor[MODULUSOFELASTICITY]);
    else
        return (0.0);
}

// --------------------------------------------------------------
CMaterialUSC::CMaterialUSC ()
{
}

CMaterialUSC::~CMaterialUSC ()
{
}

CMaterialUSC::CMaterialUSC (const std::string& szName, double dMassDensity, 
                            double dYieldStrength, double dModulusOfElasticity)
{
    SetName (szName);
    SetMassDensity (dMassDensity);
    SetYieldStrength (dYieldStrength);
    SetModulusOfElasticity (dModulusOfElasticity);
}

double CMaterialUSC::CConvert::GetValue (const CMaterialUSC& MO, 
                                         enum CMaterial::PROPERTY P)
{
    if (P == MASSDENSITY)
        return (MO.GetMassDensity()*m_dVConvFactor[MASSDENSITY]);
    else if (P == YIELDSTRENGTH)
        return (MO.GetYieldStrength()*m_dVConvFactor[YIELDSTRENGTH]);
    else if (P == MODULUSOFELASTICITY)
        return (MO.GetModulusOfElasticity()*m_dVConvFactor[MODULUSOFELASTICITY]);
    else
        return (0.0);
}
