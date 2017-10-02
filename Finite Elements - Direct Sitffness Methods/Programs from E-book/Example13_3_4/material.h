/*********************************************
Example 13.3.4
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis
*********************************************/

#ifndef __RAJAN_MATERIAL_H__
#define __RAJAN_MATERIAL_H__

#include <iostream>
#include <string>

// abstract base class
class CMaterial
{
    public:
        CMaterial();
        ~CMaterial();
        enum PROPERTY {MASSDENSITY=0, YIELDSTRENGTH, MODULUSOFELASTICITY,
                       LASTONE};

        // modifier functions
        void SetName (const std::string& szName);
        void SetMassDensity (double dMassDensity);
        void SetYieldStrength (double dYieldStrength);
        void SetModulusOfElasticity (double dModulusOfElasticity);

        // accessor functions
        void GetName (std::string& szName) const;
        double GetMassDensity () const;
        double GetYieldStrength () const;
        double GetModulusOfElasticity () const;

        // helper function
        void Display () const;

    private:
        std::string m_szName;
        double      m_dMassDensity;
        double      m_dYieldStrength;
        double      m_dModulusOfElasticity;
};

// SI version: kg, m, s
class CMaterialSI : public CMaterial
{
    public:
        class CConvert
        {
            public:
                CConvert () {};
                virtual ~CConvert() {}
                double GetValue (const CMaterialSI&, enum PROPERTY);
        };
        CMaterialSI ();
        ~CMaterialSI ();
        CMaterialSI (const std::string& szName, double dMassDensity, 
                     double dYieldStrength, double dModulusOfElasticity);
    private:
        static double m_dVConvFactor[LASTONE];
};

// SI version: slug, ft, s
class CMaterialUSC : public CMaterial
{
    public:
        class CConvert
        {
            public:
                CConvert () {};
                virtual ~CConvert() {}
                double GetValue (const CMaterialUSC&, enum PROPERTY);
        };
        CMaterialUSC ();
        ~CMaterialUSC ();
        CMaterialUSC (const std::string& szName, double dMassDensity, 
                      double dYieldStrength, double dModulusOfElasticity);
    private:
        static double m_dVConvFactor[LASTONE];
};

#endif