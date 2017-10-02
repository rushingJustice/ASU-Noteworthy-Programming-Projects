#ifndef __MATH_H__
#define __MATH_H__

class CMath
{
    public:
        static double Add (double, double);
        static double Subtract (double, double);
        static double Multiply (double, double);
        static double Divide (double, double);
        static void DisplayCounters ();

    private:
        static int nAddCounter;
        static int nSubtractCounter;
        static int nMultiplyCounter;
        static int nDivideCounter;
};

#endif
