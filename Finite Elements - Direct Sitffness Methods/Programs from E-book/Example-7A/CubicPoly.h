#ifndef __RAJAN_CUBICPOLY_H__
#define __RAJAN_CUBICPOLY_H__

// cubic polynomial expressed as
//   a*x^3 + b*x^2 + c*x + d
class CCubicPoly
{
    public:
        CCubicPoly ();
        ~CCubicPoly ();
        CCubicPoly (double fa, double fb, double fc, double fd);
        CCubicPoly (const CCubicPoly&);
        bool Solve (double dVR[3]) const;

    private:
        double m_fa;
        double m_fb;
        double m_fc;
        double m_fd;
};

#endif
