#ifndef __RAJAN_QUADPOLY_H__
#define __RAJAN_QUADPOLY_H__

// quadratic polynomial expressed as
//   a*x^2 + b*x + c
class CQuadPoly
{
    public:
        CQuadPoly ();
        ~CQuadPoly ();
        CQuadPoly (double fa, double fb, double fc);
        CQuadPoly (const CQuadPoly&);
        bool Solve (double dVR[2]) const;

    private:
        double m_fa;
        double m_fb;
        double m_fc;
};

#endif
