/* EXAMPLE 9.6.1
   Copyright(c) 2002-08, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate how to detect and use command line 
       arguments.

*********************************************/
#include <iostream>

int main (int argc, char* argv[])
{
    std::cout << "Number of command line arguments: " 
              << argc << std::endl;

    std::cout << "\nCommand line arguments are as follows\n";
    for (int i=0; i < argc; i++)
    {
        std::cout << i << ": " << argv[i] << std::endl;
    }
    std::cout << std::endl;

    return 0;
}