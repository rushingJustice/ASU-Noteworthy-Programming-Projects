#include <iostream>
#include "..\library\matrixtemplate.h"
#include "..\library\clock.h"
/*
program to show difference in timing between debug and release versions
Windows 7-64 bit. Intel Xeon E5440 @2.83 GHz (2-cores). 8 GB RAM.
Debug   : 14 seconds
Release : 0 second
*/

int main ()
{
    const int SIZE  = 100;   // adjust based on your hardware
    const int LOOPS = 200;   // adjust based on your hardware
    CClock timer;

    // mark the time
    timer.MarkTime();

    CMatrix<double> A(SIZE,SIZE); // good placement
    for (int i=1; i <= LOOPS; i++)
    {
        for (int j=1; j <= LOOPS; j++)
        {
            //CMatrix<double> A(SIZE,SIZE); // bad placement but a smart
                                            // compiler will take care of this
            for (int k=1; k <= SIZE; k++)
            {
                for (int l=1; l <= SIZE; l++)
                {
                    A(k,l) = 1.0;
                }
            }
        }
    }

    // get the elapsed time
    std::cout << "Total elapsed wall clock time: " << timer.DiffTime()
              << " seconds\n";

    return 0;
}