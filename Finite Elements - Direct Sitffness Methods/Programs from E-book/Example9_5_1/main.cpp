/* EXAMPLE 9.5.1
   Copyright(c) 2002-08, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate use of C++ exception handling classes
   (2) Note: All compilers currently do NOT support
             all exceptions.

*********************************************/
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

int main ()
{
    // Logical Error 1: Bad Memory Allocation
    double *pBigArray;
    int nSize = 1024;
    bool bError = false;
    do
    {
        try
        {
            pBigArray = new double [nSize];
        }
        catch (std::bad_alloc)
        {
            std::cout << "Cannot allocate : " << nSize << " DP words\n";
            bError = true;
        }
        if (!bError)
        {
            std::cout << "Allocated : " << nSize << " DP words\n";
            delete [] pBigArray;
            nSize *= 2;
        }
    } while (!bError);

    // Logical Error 2: string extraction error
    std::string szFileName = "thisANDthat.txt";
    std::string szExtension = "?";
    try
    {
         size_t nLoc = szFileName.find_last_of('.');
         size_t nLen = szFileName.length();
         szExtension = szFileName.substr (nLen+1, nLen-nLoc+1);
    }
    catch (std::out_of_range e)
    {
        std::cout << "string extraction error (OOR): " << e.what()
                  << std::endl;
    }
    catch (std::length_error e)
    {
        std::cout << "string extraction error (LE): " << e.what()
                  << std::endl;
    }
    std::cout << "File extension is " << szExtension << std::endl;
    
    // Runtime Error 1: Out of range error
    std::vector<int> V(4);
    V[0] = 1; V[1] = 2; V[2] = 3; V[3] = 4;
    size_t nLast = V.size();
    try
    {
        std::cout << "Value at last position is " 
            << V.at(nLast) << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught " << e.what() << std::endl;
        std::cerr << "Type " << typeid(e).name() << std::endl;
    }

    return 0;
}
