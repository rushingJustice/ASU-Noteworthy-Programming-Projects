/*********************************************
Program Planar Frame
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis
*********************************************/
#ifndef __RAJAN_MATERIAL_H__
#define __RAJAN_MATERIAL_H__

class CMaterial
{
    public:
        CMaterial ();   // ctor
        ~CMaterial ();  // dtor

        // accessor functions
        float GetYM () const;
        float GetCTE () const;
		float Getv () const;

        // modifier functions
        void SetYM (const float);
        void SetCTE (const float);
		void Setv (const float);

    private:
        float m_fYM;   // young's modulus
        float m_fCTE;  // coef of thermal expansion
		float m_fv;    // poisson's ratio
};

#endif	